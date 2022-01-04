#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/PoseArray.h"

class MySub{

    public:
        MySub(ros::NodeHandle m_nh);
        void cmdvelCb(const nav_msgs::Odometry::ConstPtr& msg);
        nav_msgs::Odometry odom_msg_;


    private:
        ros::Subscriber sub_;
        ros::NodeHandle nh_;
};

MySub::MySub(ros::NodeHandle m_nh){
    nh_ = m_nh;
    sub_ = nh_.subscribe("odomdata", 10, &MySub::cmdvelCb, this);
}

void MySub::cmdvelCb(const nav_msgs::Odometry::ConstPtr& msg)
{
  odom_msg_ = *msg;
  ROS_INFO("This is V: [%.2f]", msg->twist.twist.linear.x);
  ROS_INFO("This is W: [%.2f]", msg->twist.twist.angular.z);
  ROS_INFO("This is x postion: [%.2f]", msg->pose.pose.position.x);
  ROS_INFO("This is y postion: [%.2f]", msg->pose.pose.position.y);

}




int main(int argc, char **argv)
{

  ros::init(argc, argv, "pubsubtest_node");

  ros::NodeHandle n;

  MySub sub_obj(n);

  ros::Publisher chatter_pub = n.advertise<nav_msgs::Odometry>("odomdata2", 1000);
  ros::Publisher pose_array_pub = n.advertise<geometry_msgs::PoseArray>("pose_array2", 10);
  
  geometry_msgs::PoseArray pose_array_;
  pose_array_.header.frame_id = "odom";



  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    nav_msgs::Odometry msg;
    
    msg.header.frame_id = sub_obj.odom_msg_.header.frame_id;
    msg.child_frame_id = sub_obj.odom_msg_.child_frame_id;
    msg.pose.pose.position.x = sub_obj.odom_msg_.pose.pose.position.x;
    msg.pose.pose.position.y = sub_obj.odom_msg_.pose.pose.position.y;
    msg.pose.pose.orientation = sub_obj.odom_msg_.pose.pose.orientation ;
    msg.twist.twist.linear.x = sub_obj.odom_msg_.twist.twist.linear.x;
    msg.twist.twist.angular.z = sub_obj.odom_msg_.twist.twist.angular.z;
    
    geometry_msgs::Pose m_pose_center;
    m_pose_center.position.x = sub_obj.odom_msg_.pose.pose.position.x;
    m_pose_center.position.y = sub_obj.odom_msg_.pose.pose.position.y;
    m_pose_center.orientation = sub_obj.odom_msg_.pose.pose.orientation ;
    pose_array_.poses.push_back(m_pose_center);
    geometry_msgs::Pose m_pose_center2;
    m_pose_center2.position.x = sub_obj.odom_msg_.pose.pose.position.x+0.2;
    m_pose_center2.position.y = sub_obj.odom_msg_.pose.pose.position.y;
    m_pose_center2.orientation = sub_obj.odom_msg_.pose.pose.orientation;  
    pose_array_.poses.push_back(m_pose_center2);
    geometry_msgs::Pose m_pose_center3;
    m_pose_center3.position.x = sub_obj.odom_msg_.pose.pose.position.x;
    m_pose_center3.position.y = sub_obj.odom_msg_.pose.pose.position.y+0.2;
    m_pose_center3.orientation = sub_obj.odom_msg_.pose.pose.orientation;  
    pose_array_.poses.push_back(m_pose_center3);
    geometry_msgs::Pose m_pose_center4;
    m_pose_center4.position.x = sub_obj.odom_msg_.pose.pose.position.x;
    m_pose_center4.position.y = sub_obj.odom_msg_.pose.pose.position.y-0.2;
    m_pose_center4.orientation = sub_obj.odom_msg_.pose.pose.orientation;  
    pose_array_.poses.push_back(m_pose_center4);
    geometry_msgs::Pose m_pose_center5;
    m_pose_center5.position.x = sub_obj.odom_msg_.pose.pose.position.x-0.2;
    m_pose_center5.position.y = sub_obj.odom_msg_.pose.pose.position.y;
    m_pose_center5.orientation = sub_obj.odom_msg_.pose.pose.orientation;  
    pose_array_.poses.push_back(m_pose_center5);

    chatter_pub.publish(msg);
    pose_array_pub.publish(pose_array_);


    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}

