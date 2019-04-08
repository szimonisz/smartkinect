# smartkinect

Easy Setup:
```
#######################################################
git clone https://github.com/szimonisz/smartkinect.git
cd smartkinect
./install.sh
cd build
make
./KinectFeed
#######################################################
```

How ./install.sh works:

complete these commands within root of cloned smartkinect repository folder

NOTE: 'make' must be executed while inside of the build directory

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
Copy dependent header files from OpenNI2 to our Nite2.2 include folder
```
cp /usr/local/opt/openni2/include/ni2/{OniCAPI.h,OniPlatform.h,OniCTypes.h,OpenNI.h} ./Nite2.2/Include/
```
Create a build folder
```
mkdir build && cd build
```
Copy the NiTE2 data folder into our executable's directory
```
cp -r ../Nite2.2/Redist/NiTE2 ./
```
cmake -> make (compile the program)
```
cmake ..
make
```
Tell our executable how to find libNiTE2.dylib
```
install_name_tool -change libNiTE2.dylib @executable_path/../Nite2.2/Redist/libNiTE2.dylib KinectFeed
```
Run the program!
```
./Build/KinectFeed 
```

NOTE: 
must run make from within the Makefile's directory

depending on how dependent software was installed originally, the absolute paths to these libraries may differ..

(CMakeLists.txt / install process could differ)
