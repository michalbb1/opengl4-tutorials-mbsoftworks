echo "Updating all submodules..."
git submodule update --init --recursive

echo "Creating subdirectories for Assimp libraries..."
mkdir -p "dependencies/assimp-prebuilt/vs2017/libs32/debug"
mkdir -p "dependencies/assimp-prebuilt/vs2017/libs32/release"
mkdir -p "dependencies/assimp-prebuilt/vs2017/libs64/debug"
mkdir -p "dependencies/assimp-prebuilt/vs2017/libs64/release"

echo Downloading Assimp pre-built libraries for VS2017 / 32-bit debug version...
curl -o "dependencies/assimp-prebuilt/vs2017/libs32/debug/assimp-vc141-mtd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/debug/assimp-vc141-mtd.lib"
curl -o "dependencies/assimp-prebuilt/vs2017/libs32/debug/IrrXMLd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/debug/IrrXMLd.lib"
curl -o "dependencies/assimp-prebuilt/vs2017/libs32/debug/zlibstaticd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/debug/zlibstaticd.lib"

echo Downloading Assimp pre-built libraries for VS2017 / 64-bit debug version...
curl -o "dependencies/assimp-prebuilt/vs2017/libs64/debug/assimp-vc141-mtd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/debug/assimp-vc141-mtd.lib"
curl -o "dependencies/assimp-prebuilt/vs2017/libs64/debug/IrrXMLd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/debug/IrrXMLd.lib"
curl -o "dependencies/assimp-prebuilt/vs2017/libs64/debug/zlibstaticd.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/debug/zlibstaticd.lib"

echo Downloading Assimp pre-built libraries for VS2017 / 32-bit release version...
curl -o "dependencies/assimp-prebuilt/vs2017/libs32/release/assimp-vc141-mt.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/release/assimp-vc141-mt.lib"
curl -o "dependencies/assimp-prebuilt/vs2017/libs32/release/IrrXML.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/release/IrrXML.lib"
curl -o "dependencies/assimp-prebuilt/vs2017/libs32/release/zlibstatic.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs32/release/zlibstatic.lib"

echo Downloading Assimp pre-built libraries for VS2017 / 64-bit release version...
curl -o "dependencies/assimp-prebuilt/vs2017/libs64/release/assimp-vc141-mt.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/release/assimp-vc141-mt.lib"
curl -o "dependencies/assimp-prebuilt/vs2017/libs64/release/IrrXML.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/release/IrrXML.lib"
curl -o "dependencies/assimp-prebuilt/vs2017/libs64/release/zlibstatic.lib" "http://www.mbsoftworks.sk/meta/prebuilt/assimp/vs2017/libs64/release/zlibstatic.lib"