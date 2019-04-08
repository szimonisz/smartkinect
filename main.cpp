#include "KinectFeed.h"

int main()
{
    
    nite::UserTracker userTracker;
    nite::Status niteRc;

    nite::UserTrackerFrameRef userTrackerFrame;
    openni::Device device;
    openni::VideoStream  color;
    openni::VideoStream depth;
    openni::VideoStream ir;

    openni::VideoFrameRef depthFrame;
    openni::VideoFrameRef colorFrame;
    openni::VideoFrameRef irFrame;

    openni::Status rc = openni::STATUS_OK;

    rc = openni::OpenNI::initialize();
    nite::NiTE::initialize();
    rc = device.open(openni::ANY_DEVICE);
    niteRc = userTracker.create(&device);

    rc = color.create(device, openni::SENSOR_COLOR);
    rc = color.start();
    rc = depth.create(device, openni::SENSOR_DEPTH);
    const openni::SensorInfo *sensorInfo = device.getSensorInfo(openni::SENSOR_DEPTH);
    const openni::Array<openni::VideoMode> *videoModes = &sensorInfo->getSupportedVideoModes();
    for(int i = 0; i < videoModes->getSize(); i++){
        int resolutionX = (*videoModes)[i].getResolutionX();        
        int resolutionY = (*videoModes)[i].getResolutionY();        
    }
    // Ensure that depth camera resolution is: 512x424
    depth.setVideoMode((*videoModes)[0]);

    rc = depth.start();
    rc = ir.create(device, openni::SENSOR_IR);
    rc = ir.start();

    cv::Mat framecolor;
    cv::Mat framedepth;
    cv::Mat irmat;
    cv::Mat depthmatANDirmat;


    while (true)
    {
        niteRc = userTracker.readFrame(&userTrackerFrame);
        
        const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();
            for(int i = 0; i < users.getSize(); i++){
                const nite::UserData& user = users[i];
                if(user.isNew()){
                    userTracker.startSkeletonTracking(user.getId());
                }
                else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED){
                    const nite::SkeletonJoint& head = user.getSkeleton().getJoint(nite::JOINT_HEAD);
                    if (head.getPositionConfidence() > .5){
                        printf("%d. (%5.2f, %5.2f, %5.2f)\n", user.getId(), head.getPosition().x, head.getPosition().y, head.getPosition().z);
                    }
                }
            }
        
        color.readFrame(&colorFrame);
        const openni::RGB888Pixel* imageBuffer = (const openni::RGB888Pixel*)colorFrame.getData();

        cv::Mat(colorFrame.getHeight(),colorFrame.getWidth(),CV_8UC3, (uchar*)colorFrame.getData()).copyTo(framecolor);
        memcpy(framecolor.data, imageBuffer, colorFrame.getDataSize());

        cv::cvtColor(framecolor, framecolor, CV_BGR2RGB); //this will put colors right
         
        //DEPTH

        depth.readFrame(&depthFrame);

        const openni::DepthPixel* depthPixels = new openni::DepthPixel[depthFrame.getHeight() * depthFrame.getWidth()];
        memcpy((uchar *)depthPixels, depthFrame.getData(), depthFrame.getHeight() * depthFrame.getWidth()* sizeof(uint16_t));
        cv::Mat(depthFrame.getHeight(),depthFrame.getWidth(),CV_16U, (uchar *)depthPixels).copyTo(framedepth);

        //images are 8-bit (1 byte)
        framedepth.convertTo(framedepth, CV_8U);


        //IR
        ir.readFrame(&irFrame);
        const openni::Grayscale16Pixel* imageBuffer3 = (const openni::Grayscale16Pixel*)irFrame.getData();
        cv::Mat(irFrame.getHeight(),irFrame.getWidth(),CV_16U, (uchar*)irFrame.getData()).copyTo(irmat);
 
        //DISPLAY
        cv::imshow("IR", irmat);
        cv::imshow("color", framecolor);
        cv::imshow("depth", framedepth);
  

        // in order to concatenate matrices, both matrix must be represented with same amount of bits (8 bits)
        // however, IR in 8-bit image display is extremely bright as opposed to 16-bit IR
        //irmat.convertTo(irmat, CV_8U);
        //cv::hconcat(framedepth, irmat,depthmatANDirmat);
        //cv::imshow("depth and ir", depthmatANDirmat);
        if (cv::waitKey(30)>=0)
        {
            break;
        }
    }
    nite::NiTE::shutdown();
    cv::destroyAllWindows();
   return 0;
}


