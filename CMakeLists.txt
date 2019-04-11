cmake_minimum_required( VERSION 2.8.1 )

project( KinectFeed )

set( CMAKE_BUILD_TYPE Debug )
set( SRC KinectFeed.h KinectFeed.cpp main.cpp)

set( NITE2_DIR "${CMAKE_SOURCE_DIR}/Nite2.2" )
include_directories( "${NITE2_DIR}/Include" )
link_directories( "${NITE2_DIR}/Redist/" )

set( OPENCV_DIR "${CMAKE_SOURCE_DIR}/opencv@2/2.4.13.7_2")
include_directories( "${OPENCV_DIR}/include" )
link_directories( "${OPENCV_DIR}/lib/" )

#include_directories(${OPENNI2_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

set( OPENNI2_DIR "/usr/local/opt/openni2" )
include_directories( "${OPENNI2_DIR}/include" )
link_directories( "${OPENNI2_DIR}/lib/ni2/" )

list( APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}" )

add_executable( ${PROJECT_NAME} ${SRC} )
target_link_libraries (${PROJECT_NAME} ${OPENNI2_LIBRARIES} OpenNI2 ${OPENCV_LIBRARIES} opencv_core opencv_imgproc opencv_highgui  ${NITE2_LIBRARIES} NiTE2)
