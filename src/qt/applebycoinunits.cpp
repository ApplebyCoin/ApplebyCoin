#include "applebycoinunits.h"

#include <QStringList>

applebycoinUnits::applebycoinUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<applebycoinUnits::Unit> applebycoinUnits::availableUnits()
{
    QList<applebycoinUnits::Unit> unitlist;
    unitlist.append(ACC);
    unitlist.append(mFRC);
    unitlist.append(uFRC);
    return unitlist;
}

bool applebycoinUnits::valid(int unit)
{
    switch(unit)
    {
    case ACC:
    case mFRC:
    case uFRC:
        return true;
    default:
        return false;
    }
}

QString applebycoinUnits::name(int unit)
{
    switch(unit)
    {
    case ACC: return QString("ACC");
    case mFRC: return QString("mFRC");
    case uFRC: return QString::fromUtf8("μFRC");
    default: return QString("???");
    }
}

QString applebycoinUnits::description(int unit)
{
    switch(unit)
    {
    case ACC: return QString("applebycoins");
    case mFRC: return QString("Milli-applebycoins (1 / 1,000)");
    case uFRC: return QString("Micro-applebycoins (1 / 1,000,000)");
    default: return QString("???");
    }
}

mpq applebycoinUnits::factor(int unit)
{
    switch(unit)
    {
    case uFRC: return mpq("100/1");
    case mFRC: return mpq("100000/1");
    default:
    case ACC:  return mpq("100000000/1");
    }
}

int applebycoinUnits::amountDigits(int unit)
{
    switch(unit)
    {
    case ACC: return 8; // <100,000,000 (# digits, without commas)
    case mFRC: return 11; // <100,000,000,000
    case uFRC: return 14; // <100,000,000,000,000
    default: return 0;
    }
}

int applebycoinUnits::decimals(int unit)
{
    switch(unit)
    {
    case ACC: return 8;
    case mFRC: return 5;
    case uFRC: return 2;
    default: return 0;
    }
}

QString applebycoinUnits::format(int unit, const mpq& n, bool fPlus)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(!valid(unit))
        return QString(); // Refuse to format invalid unit
    mpq q = n * COIN / factor(unit);
    std::string str = FormatMoney(q, fPlus);
    int diff = 8 - decimals(unit);
    if(diff > 0)
        str.erase(str.length() - diff, diff);
    return QString::fromStdString(str);
}

QString applebycoinUnits::formatWithUnit(int unit, const mpq& amount, bool plussign)
{
    return format(unit, amount, plussign) + QString(" ") + name(unit);
}

bool applebycoinUnits::parse(int unit, const QString &value, mpq *val_out)
{
    mpq ret_value;
    if (!ParseMoney(value.toStdString(), ret_value))
        return false;
    if(val_out)
    {
        *val_out = ret_value * factor(unit) / COIN;
    }
    return true;
}

int applebycoinUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant applebycoinUnits::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < unitlist.size())
    {
        Unit unit = unitlist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}
