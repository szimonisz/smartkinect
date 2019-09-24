#include "KinectFeed.h"

void drawJoint(const nite::UserTracker &userTracker, const nite::SkeletonJoint &joint, const cv::Mat &frame)
{
    float outX;
    float outY;
    const nite::Point3f &pos = joint.getPosition();
    userTracker.convertJointCoordinatesToDepth(pos.x, pos.y, pos.z, &outX, &outY);
    cv::circle(frame, cv::Point((int)outX, (int)outY), 20, cv::Scalar(100, 100, 0), 3);
}

void drawLimb(const nite::UserTracker &userTracker,
              const nite::SkeletonJoint &from,
              const nite::SkeletonJoint &to,
              const cv::Mat &frame)
{
    float sX, sY, eX, eY;
    const nite::Point3f &pos = from.getPosition();
    userTracker.convertJointCoordinatesToDepth(pos.x, pos.y, pos.z, &sX, &sY);
    const nite::Point3f &end = to.getPosition();
    userTracker.convertJointCoordinatesToDepth(end.x, end.y, end.z, &eX, &eY);
    cv::line(frame, cv::Point((int)sX, (int)sY), cv::Point((int)eX, (int)eY), cv::Scalar(200, 0, 0), 3);
}

nite::JointType pairs[] = {
    // Each pair is drawn as a line.
    nite::JOINT_LEFT_SHOULDER, nite::JOINT_RIGHT_SHOULDER,
    nite::JOINT_LEFT_SHOULDER, nite::JOINT_LEFT_ELBOW,
    nite::JOINT_LEFT_ELBOW, nite::JOINT_LEFT_HAND,
    nite::JOINT_RIGHT_SHOULDER, nite::JOINT_RIGHT_ELBOW,
    nite::JOINT_RIGHT_ELBOW, nite::JOINT_RIGHT_HAND,
    nite::JOINT_LEFT_SHOULDER, nite::JOINT_TORSO,
    nite::JOINT_RIGHT_SHOULDER, nite::JOINT_TORSO,
    nite::JOINT_TORSO, nite::JOINT_LEFT_HIP,
    nite::JOINT_TORSO, nite::JOINT_RIGHT_HIP,
    nite::JOINT_LEFT_HIP, nite::JOINT_RIGHT_HIP,
    nite::JOINT_RIGHT_HIP, nite::JOINT_RIGHT_KNEE,
    nite::JOINT_RIGHT_KNEE, nite::JOINT_RIGHT_FOOT,
    nite::JOINT_TORSO, nite::JOINT_LEFT_HIP,
    nite::JOINT_LEFT_HIP, nite::JOINT_LEFT_KNEE,
    nite::JOINT_LEFT_KNEE, nite::JOINT_LEFT_FOOT,
};

void drawSkeleton(const nite::UserTracker &userTracker,
                  const nite::Skeleton *skeleton,
                  const cv::Mat &frame)
{
    nite::SkeletonJoint head = skeleton->getJoint(nite::JOINT_HEAD);
    drawJoint(userTracker, head, frame);

    drawJoint(userTracker, skeleton->getJoint(nite::JOINT_LEFT_HAND), frame);
    drawJoint(userTracker, skeleton->getJoint(nite::JOINT_RIGHT_HAND), frame);
    drawJoint(userTracker, skeleton->getJoint(nite::JOINT_RIGHT_FOOT), frame);
    drawJoint(userTracker, skeleton->getJoint(nite::JOINT_LEFT_FOOT), frame);

    for (int i = 0; i < 14; i++)
    {
        drawLimb(userTracker,
                 skeleton->getJoint(pairs[2 * i]),
                 skeleton->getJoint(pairs[2 * i + 1]),
                 frame);
    }
}

int main()
{

    nite::UserTracker userTracker;
    nite::Status niteRc;

    nite::UserTrackerFrameRef userTrackerFrame;
    openni::Device device;
    openni::VideoStream color;
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
    for (int i = 0; i < videoModes->getSize(); i++)
    {
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
    cv::Mat framesk;
    const nite::Skeleton *skeleton;
    bool found = false;

    while (true)
    {
        niteRc = userTracker.readFrame(&userTrackerFrame);

        const nite::Array<nite::UserData> &users = userTrackerFrame.getUsers();
        if (users.getSize() == 0)
        {
            found = false;
        }
        for (int i = 0; i < users.getSize(); i++)
        {
            const nite::UserData &user = users[i];
            if (user.isNew())
            {
                userTracker.startSkeletonTracking(user.getId());
            }
            else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
            {
                const nite::SkeletonJoint &head = user.getSkeleton().getJoint(nite::JOINT_HEAD);
                if (head.getPositionConfidence() > .5)
                {
                    printf("%d. (%5.2f, %5.2f, %5.2f)\n", user.getId(), head.getPosition().x, head.getPosition().y, head.getPosition().z);
                }
                found = true;
                skeleton = &(user.getSkeleton());
            }
        }
        color.readFrame(&colorFrame);
        const openni::RGB888Pixel *imageBuffer = (const openni::RGB888Pixel *)colorFrame.getData();

        cv::Mat(colorFrame.getHeight(), colorFrame.getWidth(), CV_8UC3, (uchar *)colorFrame.getData()).copyTo(framecolor);
        memcpy(framecolor.data, imageBuffer, colorFrame.getDataSize());

        //cv::cvtColor(framecolor, framecolor, CV_BGR2RGB); //this will put colors right
        cv::cvtColor(framecolor, framecolor, cv::COLOR_BGR2RGB); //this will put colors right

        //DEPTH

        depth.readFrame(&depthFrame);
        const openni::DepthPixel *depthPixels = new openni::DepthPixel[depthFrame.getHeight() * depthFrame.getWidth()];
        memcpy((uchar *)depthPixels, depthFrame.getData(), depthFrame.getHeight() * depthFrame.getWidth() * sizeof(uint16_t));
        cv::Mat(depthFrame.getHeight(), depthFrame.getWidth(), CV_16U, (uchar *)depthPixels).copyTo(framedepth);

        //images are 8-bit (1 byte)
        framedepth.convertTo(framedepth, CV_8U);

        //IR
        openni::Status status = ir.readFrame(&irFrame);
        // Kinect v1 can not simultaneously steam IR and Video
        if (status == openni::STATUS_OK)
        {
            const openni::Grayscale16Pixel *imageBuffer3 = (const openni::Grayscale16Pixel *)irFrame.getData();
            cv::Mat(irFrame.getHeight(), irFrame.getWidth(), CV_16U, (uchar *)irFrame.getData()).copyTo(irmat);
            //DISPLAY
            cv::imshow("IR", irmat);
        }
        if (framesk.size().height == 0 || framesk.size().width == 0)
        {
            framesk = cv::Mat(colorFrame.getHeight(), colorFrame.getWidth(), CV_8UC3, cv::Scalar(0, 0, 0));
        }
        else
        {
            framesk = cv::Scalar(0, 0, 0);
        }
        if (found)
        {
            drawSkeleton(userTracker, skeleton, framesk);
        }
        cv::imshow("color", framecolor);
        cv::imshow("depth", framedepth);
        cv::imshow("skeleton", framesk);

        // in order to concatenate matrices, both matrix must be represented with same amount of bits (8 bits)
        // however, IR in 8-bit image display is extremely bright as opposed to 16-bit IR
        //irmat.convertTo(irmat, CV_8U);
        //cv::hconcat(framedepth, irmat,depthmatANDirmat);
        //cv::imshow("depth and ir", depthmatANDirmat);
        if (cv::waitKey(30) >= 0)
        {
            break;
        }
    }
    nite::NiTE::shutdown();
    cv::destroyAllWindows();
    return 0;
}
