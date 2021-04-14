#include "kitti_gt_generator.hpp"

KittiGroundTruthGenerator::KittiGroundTruthGenerator()
{
    tflistener_ego_velodyne = new tf::TransformListener();

    sub_marker_object = nh_.subscribe("/carla/markers", 10, &KittiGroundTruthGenerator::MarkersCallback, this);
    pub_marker_object = nh_.advertise<visualization_msgs::MarkerArray>("/carla/transformed_markers", 10);
}

KittiGroundTruthGenerator::~KittiGroundTruthGenerator()
{

}

void KittiGroundTruthGenerator::MarkersCallback(const visualization_msgs::MarkerArray &inputMarkers)
{
    if(inputMarkers.markers.size()<1)
    {
        return;
    }

    // tf::TransformListener tflistener_ego_velodyne;
    tf::StampedTransform transform_tf;
    tf::StampedTransform transform_tf_lidar;

    try{
        tflistener_ego_velodyne->waitForTransform("ego_vehicle", inputMarkers.markers[0].header.frame_id, ros::Time(0), ros::Duration(3.0));
        tflistener_ego_velodyne->lookupTransform("ego_vehicle", inputMarkers.markers[0].header.frame_id, ros::Time(0), transform_tf);
    }
    catch(tf::TransformException ex){
        ROS_ERROR("%s",ex.what());
        ros::Duration(1.0).sleep();
    }

    visualization_msgs::MarkerArray transformedMarkerArray;
    transformedMarkerArray = inputMarkers;

    int idx = 0;

    for(auto& marker : inputMarkers.markers)
    {
        geometry_msgs::PoseStamped inputPose;
        geometry_msgs::PoseStamped transformedPose;

        inputPose.header = marker.header;
        inputPose.pose = marker.pose;

        tflistener_ego_velodyne->transformPose("ego_vehicle", inputPose, transformedPose);
        
        transformedMarkerArray.markers[idx].header = transformedPose.header;
        transformedMarkerArray.markers[idx].pose = transformedPose.pose;

        ROS_INFO_STREAM(transformedPose.header.frame_id);

        idx++;
    }
    pub_marker_object.publish(transformedMarkerArray);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "KittiGroundTruthGenerator");

  KittiGroundTruthGenerator KittiGroundTruthGenerator;
  ros::spin();

  return 0;
}