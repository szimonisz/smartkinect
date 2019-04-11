make

#install_name_tool helper for KinectFeed->libNiTE2.dylib
install_name_tool -change libNiTE2.dylib @executable_path/../Nite2.2/Redist/libNiTE2.dylib KinectFeed
#helper for KinectFeed->libopencv_imgproc
install_name_tool -change @@HOMEBREW_PREFIX@@/opt/opencv@2/lib/libopencv_imgproc.2.4.dylib @executable_path/../opencv@2/2.4.13.7_2/lib/libopencv_imgproc.2.4.dylib KinectFeed

#helper for KinectFeed->libopencv_core
install_name_tool -change @@HOMEBREW_PREFIX@@/opt/opencv@2/lib/libopencv_core.2.4.dylib @executable_path/../opencv@2/2.4.13.7_2/lib/libopencv_core.2.4.dylib KinectFeed

#helper for KinectFeed->libopencv_highgui
install_name_tool -change @@HOMEBREW_PREFIX@@/opt/opencv@2/lib/libopencv_highgui.2.4.dylib @executable_path/../opencv@2/2.4.13.7_2/lib/libopencv_highgui.2.4.dylib KinectFeed
