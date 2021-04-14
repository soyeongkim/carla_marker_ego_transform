#include <string>
#include <pcl/io/pcd_io.h>
#include <pcl_ros/transforms.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <pcl_conversions/pcl_conversions.h>

#include "sensor_msgs/PointCloud2.h"
#include "sensor_msgs/PointCloud.h"
#include <sensor_msgs/point_cloud_conversion.h>

// ROS header
#include <ros/ros.h>
#include <tf/tf.h>
#include <tf/transform_datatypes.h>
#include <tf/transform_listener.h>
#include <ros/transport_hints.h>
#include <tf/LinearMath/Quaternion.h> // tf::quaternion
#include <tf/transform_broadcaster.h>
#include <tf_conversions/tf_eigen.h>

#include <tf2_ros/transform_listener.h>

#include <visualization_msgs/MarkerArray.h>
#include <visualization_msgs/Marker.h>


class KittiGroundTruthGenerator
{
    public:
        KittiGroundTruthGenerator();
        ~KittiGroundTruthGenerator();
    private:
        void MarkersCallback(const visualization_msgs::MarkerArray &inputMarkers);
        
    private:
        ros::NodeHandle nh_;

        ros::Subscriber sub_marker_object;
        ros::Subscriber sub_tf_transform;
        ros::Publisher pub_marker_object;

        tf::TransformListener* tflistener_ego_velodyne;
};