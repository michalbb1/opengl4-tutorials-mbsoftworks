echo Creating subdirectories for Assimp libraries...
mkdir "dependencies/assimp-prebuilt/vs2017/libs32/debug"
mkdir "dependencies/assimp-prebuilt/vs2017/libs32/release"
mkdir "dependencies/assimp-prebuilt/vs2017/libs64/debug"
mkdir "dependencies/assimp-prebuilt/vs2017/libs64/release"

echo Downloading Assimp pre-built libraries for VS2017 / 32-bit debug version...
SET "FILENAME=%~dp0\dependencies\assimp-prebuilt\vs2017\libs32\debug\assimp-vc141-mtd.lib"
bitsadmin.exe /transfer "assimp-vs141-mtd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/debug/assimp-vc141-mtd.lib" "%FILENAME%"
SET "FILENAME=%~dp0\dependencies\assimp-prebuilt\vs2017\libs32\debug\IrrXMLd.lib"
bitsadmin.exe /transfer "IrrXMLd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/debug/IrrXMLd.lib" "%FILENAME%"
SET "FILENAME=%~dp0\dependencies\assimp-prebuilt\vs2017\libs32\debug\zlibstaticd.lib"
bitsadmin.exe /transfer "zlibstaticd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/debug/zlibstaticd.lib" "%FILENAME%"

echo Downloading Assimp pre-built libraries for VS2017 / 64-bit debug version...
SET "FILENAME=%~dp0\dependencies\assimp-prebuilt\vs2017\libs64\debug\assimp-vc141-mtd.lib"
bitsadmin.exe /transfer "assimp-vs141-mtd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/debug/assimp-vc141-mtd.lib" "%FILENAME%"
SET "FILENAME=%~dp0\dependencies\assimp-prebuilt\vs2017\libs64\debug\IrrXMLd.lib"
bitsadmin.exe /transfer "IrrXMLd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/debug/IrrXMLd.lib" "%FILENAME%"
SET "FILENAME=%~dp0\dependencies\assimp-prebuilt\vs2017\libs64\debug\zlibstaticd.lib"
bitsadmin.exe /transfer "zlibstaticd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/debug/zlibstaticd.lib" "%FILENAME%"

echo Downloading Assimp pre-built libraries for VS2017 / 32-bit release version...
SET "FILENAME=%~dp0\dependencies\assimp-prebuilt\vs2017\libs32\release\assimp-vc141-mt.lib"
bitsadmin.exe /transfer "assimp-vs141-mt.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/release/assimp-vc141-mt.lib" "%FILENAME%"
SET "FILENAME=%~dp0\dependencies\assimp-prebuilt\vs2017\libs32\release\IrrXML.lib"
bitsadmin.exe /transfer "IrrXML.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/release/IrrXML.lib" "%FILENAME%"
SET "FILENAME=%~dp0\dependencies\assimp-prebuilt\vs2017\libs32\release\zlibstatic.lib"
bitsadmin.exe /transfer "zlibstatic.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/release/zlibstatic.lib" "%FILENAME%"

echo Downloading Assimp pre-built libraries for VS2017 / 64-bit release version...
SET "FILENAME=%~dp0\dependencies\assimp-prebuilt\vs2017\libs64\release\assimp-vc141-mt.lib"
bitsadmin.exe /transfer "assimp-vs141-mt.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/release/assimp-vc141-mt.lib" "%FILENAME%"
SET "FILENAME=%~dp0\dependencies\assimp-prebuilt\vs2017\libs64\release\IrrXML.lib"
bitsadmin.exe /transfer "IrrXML.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/release/IrrXML.lib" "%FILENAME%"
SET "FILENAME=%~dp0\dependencies\assimp-prebuilt\vs2017\libs64\release\zlibstatic.lib"
bitsadmin.exe /transfer "zlibstatic.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/release/zlibstatic.lib" "%FILENAME%"