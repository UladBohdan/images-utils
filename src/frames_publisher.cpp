#include <algorithm>
#include <iostream>

#include <ros/ros.h>
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <boost/filesystem.hpp>

int main(int argc, char** argv) {
    ros::init(argc, argv, "image_publisher");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise("camera/image", 1);

    std::vector<std::string> images;

    std::set<std::string> available_extensions{".png", ".PNG", ".jpg", ".JPG", ".jpeg", ".JPEG"};

    ros::NodeHandle namesH;
    ros::Publisher pubNames = namesH.advertise<std_msgs::String>("frames/names", 1);

    boost::filesystem::path path(argv[1]);
    boost::filesystem::directory_iterator iter{path};
    while (iter != boost::filesystem::directory_iterator{}) {
        if (available_extensions.find(iter->path().extension().string()) != available_extensions.end()) {
            images.push_back(iter->path().string());
        }
        iter++;
    }

    std::cout << "Number of images found: " << images.size() << std::endl;
    if (images.empty()) {
        return 0;
    }
    std::sort(images.begin(), images.end());
    // TODO(uladbohdan): to check if conversion is successful.
    ros::Rate loop_rate(atoi(argv[2]));
    int pos = 0;
    while (nh.ok()) {
        pos %= images.size();
        cv::Mat image = cv::imread(images[pos], CV_LOAD_IMAGE_COLOR);
        sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();

        pub.publish(msg);
        ros::spinOnce();

        std_msgs::String msg2;
        msg2.data = images[pos];
        std::cout << "IMAGE " << images[pos] << " was pushed." << std::endl;
        pubNames.publish(msg2);
        ros::spinOnce();

        loop_rate.sleep();
        // Temporary solution: to post photos one by one, manually.
        // std::cout << "awaiting for any key to be pressed..." << std::endl;
        // getchar();

        pos++;

        // Comment condition below to publish photos indefinitely.
	      if (pos >= images.size()) {
	         return 0;
         }
    }
}
