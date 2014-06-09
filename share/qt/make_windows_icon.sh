#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/applebycoin.png
ICON_DST=../../src/qt/res/icons/applebycoin.ico
convert ${ICON_SRC} -resize 16x16 applebycoin-16.png
convert ${ICON_SRC} -resize 32x32 applebycoin-32.png
convert ${ICON_SRC} -resize 48x48 applebycoin-48.png
convert applebycoin-16.png applebycoin-32.png applebycoin-48.png ${ICON_DST}

