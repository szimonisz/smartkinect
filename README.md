# smartkinect

Setup:

complete these commands within root of cloned smartkinect repository folder

install OpenNI2 via brew
install OpenCV2 via brew

install libfreenect2 with OpenNI2 support

copy libraries from openni2_libfreenect2_mac_drivers into /usr/local/opt/openni2/lib/ni2/Drivers folder

curl -LO https://bitbucket.org/kaorun55/openni-2.2/raw/2f54272802bfd24ca32f03327fbabaf85ac4a5c4/NITE%202.2%20α/NiTE-MacOSX-x64-2.2.tar.zip

tar xvf NiTE-MacOSX-x64-2.2.tar.zip
tar xvf NiTE-MacOSX-x64-2.2.tar.bz2
mv ./NiTE-MacOSX-x64-2.2 ./Nite2.2

install_name_tool -change libOpenNI2.dylib /usr/local/opt/openni2/lib/ni2/libOpenNI2.dylib ./Nite2.2/Redist/libNiTE2.dylib

mkdir build && cd build
cmake ..
make
install_name_tool -change libNiTE2.dylib @executable_path/../dependencies/Nite2.2/Redist/libNiTE2.dylib KinectFeed
cp -r ../Nite2.2/Redist/NiTE2 ./
./KinectFeed 

#Good to go!


NOTE:

must run make from within the Makefile's directory
