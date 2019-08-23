#include <ros/ros.h>
// PCL specific includes
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/statistical_outlier_removal.h>

ros::Publisher pub;

void 
cloud_cb (const pcl::PCLPointCloud2ConstPtr& cloud)
{
  pcl::PCLPointCloud2 cloud_filtered;

  ROS_INFO("I recieved a cloud");
  
  // Perform the voxel filtering
  pcl::VoxelGrid<pcl::PCLPointCloud2> vox;
  vox.setInputCloud (cloud);
  vox.setLeafSize (0.013, 0.013, 0.013);
  vox.filter (cloud_filtered);
  
  //outlier filter
//  pcl::StatisticalOutlierRemoval<pcl::PCLPointCloud2> sor;
//  sor.setInputCloud(cloud_filtered);
//  sor.setMeanK (50);
//  sor.setStddevMulThresh (1.0);
//  sor.filter (cloud_filtered);

  // Publish the data
  pub.publish (cloud_filtered);
}

int
main (int argc, char** argv)
{
  // Initialize ROS
  ros::init (argc, argv, "segmentation_cpp");
  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe ("/kinect2/sd/points", 1, cloud_cb);

  // Create a ROS publisher for the output point cloud
  pub = nh.advertise<sensor_msgs::PointCloud2> ("output", 1);

  // Spin
  ros::spin ();
}
