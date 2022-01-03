#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"

class MySub{

    public:
        MySub(ros::NodeHandle m_nh);
        void cmdvelCb(const nav_msgs::Odometry::ConstPtr& msg);
        nav_msgs::Odometry odom_msg_v;
        nav_msgs::Odometry odom_msg_w;

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
  odom_msg_v = *msg;
  odom_msg_w = *msg;
  ROS_INFO("This is V: [%.2f]", msg->twist.twist.linear.x);
  ROS_INFO("This is W: [%.2f]", msg->twist.twist.angular.z);
}




int main(int argc, char **argv)
{

  ros::init(argc, argv, "pubsubtest_node");

  ros::NodeHandle n;

  MySub sub_obj(n);

  ros::Publisher chatter_pub = n.advertise<nav_msgs::Odometry>("odomdata2", 1000);

  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    nav_msgs::Odometry msg;
    
    msg.twist.twist.linear.x = sub_obj.odom_msg_v .twist.twist.linear.x;
    msg.twist.twist.angular.z = sub_obj.odom_msg_w .twist.twist.angular.z;

    chatter_pub.publish(msg);


    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}

