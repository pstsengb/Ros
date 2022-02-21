#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/PoseArray.h"
#include "tf/transform_broadcaster.h"
#include "sensor_msgs/LaserScan.h"
#include "tf/tf.h"
#include "tf/transform_broadcaster.h"
#include "math.h"
#include "nav_msgs/Path.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/PoseArray.h"


class CalPath{

    public:
        CalPath(ros::NodeHandle i_n);
        void odomCb(const nav_msgs::Odometry::ConstPtr& msg);
        nav_msgs::Odometry odom_msg_;
    private:
        ros::NodeHandle n_;
        ros::Subscriber sub_;
        ros::Publisher pub_;
        ros::Publisher pub1_;
};

CalPath::CalPath(ros::NodeHandle i_n){
    n_ = i_n;
    sub_ = n_.subscribe("odom",2, &CalPath::odomCb, this); //if publish very fast,such as 1000msg /1s ,but we only tackle 10 item 
    pub_ = n_.advertise<nav_msgs::Path>("control_path", 2);
    pub1_ = n_.advertise<geometry_msgs::PoseArray>("control_path1", 2);
}

void CalPath::odomCb(const nav_msgs::Odometry::ConstPtr& msg){
  odom_msg_ = *msg;
  ROS_INFO("linear X Velocity: [%.2f]", odom_msg_.twist.twist.linear.x);
  ROS_INFO("angular Z velocity: [%.2f]", odom_msg_.twist.twist.angular.z);
  ROS_INFO(" x postion: [%.2f]", odom_msg_.pose.pose.position.x);
  ROS_INFO(" y postion: [%.2f]", odom_msg_.pose.pose.position.y);
  double t_monitor = 3.0;
  double t_ration = 10.0; //1 second to ten times samller 
  double dt_range = (t_monitor*t_ration) ;
  double dt = t_monitor /dt_range ;

  double v = odom_msg_.twist.twist.linear.x;
  double w = odom_msg_.twist.twist.angular.z;
  nav_msgs::Path m_path;
  m_path.header.frame_id = "base_link";
  m_path.header.stamp = ros::Time::now();
  geometry_msgs::PoseArray pose_array_;
  pose_array_.header.frame_id = "base_link";
  pose_array_.header.stamp = ros::Time::now();
  for(double iii=-1.0;iii<1.1;iii+=0.1){
    for(double ii=-0.4;ii<0.5;ii+=0.1){
      double x = 0;
      double y = 0;
      double theta = 0;
      for (int i=1.0;i<(dt_range+1);i++){
        x = x + (v+ii) * cos(theta) * dt;
        y = y + (v+ii) * sin(theta) * dt;
        theta = theta + (w+iii)* dt;
        geometry_msgs::PoseStamped a_pose;
        a_pose.pose.position.x = x ;
        a_pose.pose.position.y = y ;
        geometry_msgs::Pose m_pose_center;
        m_pose_center.position.x = x;
        m_pose_center.position.y = y;
        tf::Quaternion q;
        q.setRPY(0, 0, theta);
        geometry_msgs::Quaternion odom_quat;
        tf::quaternionTFToMsg(q, odom_quat);
        a_pose.pose.orientation = odom_quat;
        m_pose_center.orientation.x = q.x() ;
        m_pose_center.orientation.y = q.y() ;
        m_pose_center.orientation.z = q.z() ;
        m_pose_center.orientation.w = q.w() ;
        m_path.poses.push_back(a_pose);
        pose_array_.poses.push_back(m_pose_center);
      }
    }
  }
  pub_.publish(m_path);
  pub1_.publish(pose_array_);
}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "calpath_node");
  ros::NodeHandle n;

  CalPath driver(n);

  ros::Rate loop_rate(5);

  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
 
  return 0;
}


