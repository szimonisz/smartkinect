# smartkinect

Setup:

complete these commands within root of cloned smartkinect repository folder

Installing libusb / OpenNI2 / OpenCV2
```
$ brew update
$ brew install libusb
$ brew tap brewsci/science
$ brew tap totakke/openni2
$ brew install openni2
$ brew install opencv@2
```

Copy libraries from openni2_libfreenect2_drivers into /usr/local/opt/openni2/lib/ni2/Drivers folder
```
$ cp -r ./openni2_libfreenect2_drivers/. /usr/local/opt/openni2/lib/ni2/Drivers
```

Download, unarchive, and place the NiTE2.2 library into a folder called Nite2.2
```
curl -LO https://bitbucket.org/kaorun55/openni-2.2/raw/2f54272802bfd24ca32f03327fbabaf85ac4a5c4/NITE%202.2%20α/NiTE-MacOSX-x64-2.2.tar.zip

tar xvf NiTE-MacOSX-x64-2.2.tar.zip
tar xvf NiTE-MacOSX-x64-2.2.tar.bz2
mv ./NiTE-MacOSX-x64-2.2 ./Nite2.2
```
Tell libNiTE2 how to find it's dependent dynamic library: libOpenNI2.dylib
```
install_name_tool -change libOpenNI2.dylib /usr/local/opt/openni2/lib/ni2/libOpenNI2.dylib ./Nite2.2/Redist/libNiTE2.dylib
```
Create a build folder, and cmake -> make our executable
```
mkdir build && cd build
cmake ..
make
```
Tell our executable how to find libNiTE2.dylib
```
install_name_tool -change libNiTE2.dylib @executable_path/../dependencies/Nite2.2/Redist/libNiTE2.dylib KinectFeed
```
Copy the NiTE2 data folder into our executable's directory
```
cp -r ../Nite2.2/Redist/NiTE2 ./
```
Run the program!
```
./KinectFeed 
```

NOTE: 
must run make from within the Makefile's directory
depending on how dependent software was installed originally, the absolute paths to these libraries may differ..
(CMakeLists.txt / install process could differ)
