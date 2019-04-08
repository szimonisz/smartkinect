echo "Installing smartkinect"
echo "Discovering or installing required dependencies via Homebrew for Mac OS X..."

brew update
brew install libusb
brew tap brewsci/science
brew tap totakke/openni2
brew install openni2
brew install opencv@2

echo "Copying openni2_libfreenect2_drivers into openni2's system Drivers folder"
cp -r ./openni2_libfreenect2_drivers/. /usr/local/opt/openni2/lib/ni2/Drivers

echo "Downloading NiTE 2.2 (almost 200MB will take a second)"
curl -LO https://bitbucket.org/kaorun55/openni-2.2/raw/2f54272802bfd24ca32f03327fbabaf85ac4a5c4/NITE%202.2%20α/NiTE-MacOSX-x64-2.2.tar.zip

echo "Unarchiving NiTE 2.2"
tar xvf NiTE-MacOSX-x64-2.2.tar.zip
tar xvf NiTE-MacOSX-x64-2.2.tar.bz2
mv ./NiTE-MacOSX-x64-2.2 ./Nite2.2
rm NiTE-MacOSX-x64-2.2.tar.bz2
rm NiTE-MacOSX-x64-2.2.tar.zip

echo "install_name_tool helper for libNiTE2.dylib->libOpenNI2.dylib"
install_name_tool -change libOpenNI2.dylib /usr/local/opt/openni2/lib/ni2/libOpenNI2.dylib ./Nite2.2/Redist/libNiTE2.dylib

echo "Copying OpenNI2 header files to Nite2.2/Include folder"
cp /usr/local/opt/openni2/include/ni2/{OniCAPI.h,OniPlatform.h,OniCTypes.h,OpenNI.h} ./Nite2.2/Include/

echo "Creating a build directory"
mkdir build
cd build

echo "Moving NiTE2.2 Data folder to executable's root directory"
cp -r ../Nite2.2/Redist/NiTE2 ./

echo "Building a Makefile via CMake"
cmake ..

echo "Building the ./compile.sh helper script for developing"
cat > compile.sh <<'EOF'
make
#install_name_tool helper for KinectFeed->libNiTE2.dylib
install_name_tool -change libNiTE2.dylib @executable_path/../Nite2.2/Redist/libNiTE2.dylib KinectFeed
EOF
chmod +x compile.sh

echo "Installation is Complete. cd build && ./compile.sh && ./KinectFeed to run"
