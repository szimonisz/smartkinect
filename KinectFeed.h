#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <time.h>
#include <signal.h>
#include <opencv2/opencv.hpp>
#include <ni2/OpenNI.h>
#include <NiTE.h>

class KinectFeed{
  public:
      openni::Status status;
      openni::Device device;
      openni::VideoStream depthStream, colorStream;
      openni::VideoFrameRef depthFrame, colorFrame;
      cv::Mat depthImage, colorImage; 
  public:    
      KinectFeed(void);
      bool init();
      ~KinectFeed(void);
};

