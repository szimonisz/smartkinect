#include "KinectFeed.h"

//Constructor
KinectFeed::KinectFeed(void)
{
}

//Deconstructor
KinectFeed::~KinectFeed(void)
{
    // close the device if it is active and open
    if(device.isValid()){
        device.close(); 
    }
}

//Initalize Kinect Device
bool KinectFeed::init(){
  
   // ANY_DEVICE finds any active hardware device compatible with OpenNI
   status = openni::OpenNI::initialize();     
   openni::Array<openni::DeviceInfo> deviceList;
   const char* deviceURI = openni::ANY_DEVICE;
   status = device.open(deviceURI);
   //openni::DeviceInfo dInfo = device.getDeviceInfo();
   //const char * deviceName = dInfo.getUri();
   if(status != openni::STATUS_OK){
      std::cout << "KinectFeed: Device open failed!" << std::endl; 
      std::cout << openni::OpenNI::getExtendedError() << std::endl; 
      openni::OpenNI::shutdown();
      return false;
   }
   status = depthStream.create(device, openni::SENSOR_DEPTH);
   if(status == openni::STATUS_OK){
       status = depthStream.start();
       if( status != openni::STATUS_OK){
           std::cout << openni::OpenNI::getExtendedError() << std::endl; 
           depthStream.destroy();
       }
   }
   status = colorStream.create(device, openni::SENSOR_COLOR);
   if(status == openni::STATUS_OK){
       status = colorStream.start();
       if( status != openni::STATUS_OK){
           std::cout << openni::OpenNI::getExtendedError() << std::endl; 
           colorStream.destroy();
       }
   }
   if (!depthStream.isValid() || !colorStream.isValid()){
       printf("SimpleViewer: No valid streams. Exiting\n");
       openni::OpenNI::shutdown();
       return 2;
   }
   return true;
}
