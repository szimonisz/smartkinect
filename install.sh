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

echo "Downloading opencv@2"
curl -LO https://homebrew.bintray.com/bottles/opencv@2-2.4.13.7_2.mojave.bottle.tar.gz

echo "Unarchiving OpenCV@2.tar"
tar xvf opencv@2-2.4.13.7_2.mojave.bottle.tar.gz
#unarchived OpenCV2 lives in folder called opencv@2/2.4.13.7_2
rm opencv@2-2.4.13.7_2.mojave.bottle.tar.gz

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
#helper for KinectFeed->libopencv_imgproc
install_name_tool -change @@HOMEBREW_PREFIX@@/opt/opencv@2/lib/libopencv_imgproc.2.4.dylib @executable_path/../opencv@2/2.4.13.7_2/lib/libopencv_imgproc.2.4.dylib KinectFeed

#helper for KinectFeed->libopencv_core
install_name_tool -change @@HOMEBREW_PREFIX@@/opt/opencv@2/lib/libopencv_core.2.4.dylib @executable_path/../opencv@2/2.4.13.7_2/lib/libopencv_core.2.4.dylib KinectFeed

#helper for KinectFeed->libopencv_highgui
install_name_tool -change @@HOMEBREW_PREFIX@@/opt/opencv@2/lib/libopencv_highgui.2.4.dylib @executable_path/../opencv@2/2.4.13.7_2/lib/libopencv_highgui.2.4.dylib KinectFeed
EOF
chmod +x compile.sh

echo "Installation is Complete. cd build && ./compile.sh && ./KinectFeed to run"
