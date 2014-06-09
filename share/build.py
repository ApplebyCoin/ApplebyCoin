########################################################
#          Python QT Wallet Linux Installer            #
#          									           #
#               USE AT YOUR OWN RISK!                  #
########################################################
import os
url =  raw_input("Enter GitHub URL: ")
if(url[-1:]=="/"):
	url=url[:-1]
split = url.split("/")
arraySize = len(split)
neededElement = (arraySize - 1)
FolderName = split[neededElement]
CurrentPath = os.getcwd()
WalletPath = CurrentPath + "/" + FolderName
os.system("git clone " + url)
os.chdir(WalletPath)
for file in os.listdir(WalletPath):
    if file.endswith(".pro"):
        proFile = file
#Remove lines calling 1.53 boost for windows, making sometimes problems during installation:
proFileContent = open(proFile, "r").read()
SplitProFile = proFileContent.split("CONFIG +=")
arraySizeProSplit = len(SplitProFile)
neededElementProSplit = (arraySizeProSplit - 1)
SecondPart = SplitProFile[neededElementProSplit]
SecondPartSplitted = SecondPart.split("\n")
SecondPartSplitted.pop(0)
SecondPart = "\n".join(SecondPartSplitted)

SplitProFileSecondPart = SecondPart.split("OBJECTS_DIR =")
if(len(SplitProFileSecondPart) > 1):
	LinesForDeleting = SplitProFileSecondPart[0]
	EditedproFileContent = proFileContent.replace(LinesForDeleting, "")
	open(proFile, "w").write(EditedproFileContent)
#Create obj directory in src
os.chdir(WalletPath + r'/src')
os.system("mkdir obj")
#chmod leveldb, which also makes problems during installation on some wallets
os.system("chmod +x leveldb/build_detect_platform")
os.system("make -f makefile.unix")
os.chdir(WalletPath)
os.system(r'qmake-qt4')
os.system("make")
os.system("clear")
print "Wallet compiled!"
#print raw_input("")

