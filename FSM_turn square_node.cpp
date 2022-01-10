#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "tf/tf.h"

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
      tf::Quaternion q(
    msg->pose.pose.orientation.x,
    msg->pose.pose.orientation.y,
    msg->pose.pose.orientation.z,
    msg->pose.pose.orientation.w);
  tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);


  //ROS_INFO("X-postion: [%.2f]", msg->pose.pose.position.x);
  //ROS_INFO("Y-postion: [%.2f]", msg->pose.pose.position.y);
  //ROS_INFO("orientation x: [%.2f]", msg->pose.pose.orientation.x);
  //ROS_INFO("orientation y: [%.2f]", msg->pose.pose.orientation.y);
  //ROS_INFO("orientation z: [%.2f]", msg->pose.pose.orientation.z);
  //ROS_INFO("orientation w: [%.2f]", msg->pose.pose.orientation.w);
  //ROS_INFO("rad: [%.2f]", yaw);
  //ROS_INFO("V: [%.2f]", msg->twist.twist.linear.x);
  //ROS_INFO("W: [%.2f]", msg->twist.twist.angular.z);
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "pubsubtest_node");
  ros::NodeHandle n;
  MySub sub_obj(n);

  ros::Publisher chatter_pub = n.advertise<nav_msgs::Odometry>("odomdata2", 1000);
  ros::Publisher chatter1_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);

  ros::Rate loop_rate(10);

  std::string fsm;
  fsm = "initial";
  nav_msgs::Odometry start_odom;
  
  double rad = 3.14;
  double rad1 = 4.71;
  double rad2 = 6.28;
  double rad3 = 0.0;
  double rad4 = 1.57;

  while (ros::ok())
  {
    tf::Quaternion q(
    sub_obj.odom_msg_.pose.pose.orientation.x,
    sub_obj.odom_msg_.pose.pose.orientation.y,
    sub_obj.odom_msg_.pose.pose.orientation.z,
    sub_obj.odom_msg_.pose.pose.orientation.w);
    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);
    yaw = yaw +3.14;
    //double yaw1 = abs(yaw);

    ROS_INFO("x position:%.2f", sub_obj.odom_msg_.pose.pose.position.x);
    ROS_INFO("y position:%.2f", sub_obj.odom_msg_.pose.pose.position.y);
    ROS_INFO("rad%.2f", yaw);
    //ROS_INFO("rad%.2f", yaw1);
    
    if(fsm=="initial"){
      ROS_INFO("System initialized.");
      fsm = "go_straightL";
      start_odom = sub_obj.odom_msg_;

    }
    else if(fsm=="go_straightL"){
      if(sub_obj.odom_msg_.pose.pose.position.x-start_odom.pose.pose.position.x<1.0){
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.3;
        chatter1_pub .publish(msg1);
      }
      else{
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.0;
        chatter1_pub .publish(msg1);      
        fsm = "go_rotate90";
      }
    } 
    else if(fsm=="go_rotate90"){
      if( yaw-rad < 1.58  ){
        geometry_msgs::Twist msg1;
        msg1.angular.z = 0.4;
        chatter1_pub .publish(msg1);
      }
      else{
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.0;
        chatter1_pub .publish(msg1); 
        fsm = "go_straightD";
      }
    }
    else if(fsm=="go_straightD"){
      if( sub_obj.odom_msg_.pose.pose.position.y-start_odom.pose.pose.position.y<1.0){
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.3;
        chatter1_pub .publish(msg1);
      }
      else{
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.0;
        chatter1_pub .publish(msg1);      
        fsm = "go_rotate180";
      }
    } 
    else if(fsm=="go_rotate180"){
      if( yaw-rad1 < 1.54  ){
        geometry_msgs::Twist msg1;
        msg1.angular.z = 0.4;
        chatter1_pub .publish(msg1);
        ROS_INFO(" cur rad  %.2f", yaw-rad1);
      }
      else{
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.0;
        chatter1_pub .publish(msg1); 
        fsm = "go_straightR";
      }
    }
    
    else if(fsm=="go_straightR"){
      if(sub_obj.odom_msg_.pose.pose.position.x-start_odom.pose.pose.position.x>0.0){
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.3;
        chatter1_pub .publish(msg1);
      }
      else{
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.0;
        chatter1_pub .publish(msg1);      
        fsm = "go_rotate270";
      }
    }
    else if(fsm=="go_rotate270"){
      if( yaw-rad3 < 1.58 ){
        geometry_msgs::Twist msg1;
        msg1.angular.z = 0.4;
        chatter1_pub .publish(msg1);
      }
      else{
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.0;
        chatter1_pub .publish(msg1); 
        fsm = "go_straightU";
      }
    }
    else if(fsm=="go_straightU"){
      if(sub_obj.odom_msg_.pose.pose.position.y-start_odom.pose.pose.position.y>0.0){
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.3;
        chatter1_pub .publish(msg1);
      }
      else{
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.0;
        chatter1_pub .publish(msg1);      
        fsm = "go_rotate360";
      }
    }
    else if(fsm=="go_rotate360"){
      if( yaw-rad4 < 1.58 ){
        geometry_msgs::Twist msg1;
        msg1.angular.z = 0.4;
        chatter1_pub .publish(msg1);
      }
      else{
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.0;
        chatter1_pub .publish(msg1); 
        fsm = "go_straightL";
      }
    }
    nav_msgs::Odometry msg;
    msg.header.frame_id = sub_obj.odom_msg_.header.frame_id;
    msg.child_frame_id = sub_obj.odom_msg_.child_frame_id;
    msg.pose.pose.position.x = sub_obj.odom_msg_.pose.pose.position.x;
    msg.pose.pose.position.y = sub_obj.odom_msg_.pose.pose.position.y;
    msg.pose.pose.orientation = sub_obj.odom_msg_.pose.pose.orientation ;
    msg.twist.twist.linear.x = sub_obj.odom_msg_.twist.twist.linear.x;
    msg.twist.twist.angular.z = sub_obj.odom_msg_.twist.twist.angular.z;

    chatter_pub.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
