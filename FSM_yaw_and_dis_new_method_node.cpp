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

  double last_yaw = 0.0 ;
  double inc_yaw = 0.0 ;

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

    double d_yaw = 0.0;
    d_yaw = yaw - last_yaw ;

    if(fabs(d_yaw)>5){
      if(d_yaw < 0.0){
        d_yaw = 6.28 + d_yaw ; 
      }
      else{
        d_yaw = -6.28 + d_yaw ;
      }
    }
    double x1 = sub_obj.odom_msg_.pose.pose.position.x;
    double x2 = start_odom.pose.pose.position.x;
    double y1 = sub_obj.odom_msg_.pose.pose.position.y;
    double y2 = start_odom.pose.pose.position.y;
    double dis_s = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

    ROS_INFO("start x position:%.2f", start_odom.pose.pose.position.x);
    ROS_INFO("start y position:%.2f", start_odom.pose.pose.position.y);
    ROS_INFO("current x position:%.2f", sub_obj.odom_msg_.pose.pose.position.x);
    ROS_INFO("current y position:%.2f", sub_obj.odom_msg_.pose.pose.position.y);
    ROS_INFO("rad%.2f", yaw);
    //ROS_INFO(" d_yaw  %.2f", d_yaw);
    //ROS_INFO(" inc_yaw  %.2f", inc_yaw);
    //ROS_INFO(" last_yaw  %.2f", last_yaw);
    //ROS_ERROR("-------------------------------");
    ROS_INFO(" move dis: %.2f", dis_s);
    //ROS_INFO(" last %.2f", last_dis);
    //ROS_INFO(" d_dis %.2f", d_dis);
    ROS_INFO("satus: %s",fsm.c_str());
    ROS_ERROR("-------------------------------");


    if(fsm=="initial"){
      ROS_INFO("System initialized.");
      fsm = "go_straightL";
      start_odom = sub_obj.odom_msg_;

    }
    
    else if(fsm=="go_straightL"){
      if(dis_s <1.0){
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.1;
        chatter1_pub .publish(msg1);
      }
      else{
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.0;
        chatter1_pub .publish(msg1);     
        fsm = "go_rotate90";
        inc_yaw = 0.0;
      }
    } 
    else if(fsm=="go_rotate90"){
      inc_yaw = inc_yaw + d_yaw;
      if( inc_yaw  < 1.57  ){
        geometry_msgs::Twist msg1;
        msg1.angular.z = 0.2;
        chatter1_pub .publish(msg1);
      }
      else{
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.0;
        chatter1_pub .publish(msg1); 
        inc_yaw = 0.0 ;
        fsm = "go_straightD";
        start_odom.pose.pose.position.x = sub_obj.odom_msg_.pose.pose.position.x;
        start_odom.pose.pose.position.y = sub_obj.odom_msg_.pose.pose.position.y;

      }
    }
    else if(fsm=="go_straightD"){
      if( dis_s <1.0){
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.1;
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
      inc_yaw = inc_yaw + d_yaw;
      if( inc_yaw  < 1.57  ){
        geometry_msgs::Twist msg1;
        msg1.angular.z = 0.2;
        chatter1_pub .publish(msg1);
      }
      else{
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.0;
        chatter1_pub .publish(msg1); 
        inc_yaw = 0.0;
        fsm = "go_straightR";
        start_odom.pose.pose.position.x = sub_obj.odom_msg_.pose.pose.position.x;
        start_odom.pose.pose.position.y = sub_obj.odom_msg_.pose.pose.position.y;
      }
    }
    
    else if(fsm=="go_straightR"){
      if(dis_s <1.0){
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.1;
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
      inc_yaw = inc_yaw + d_yaw;
      if( inc_yaw  < 1.57  ){
        geometry_msgs::Twist msg1;
        msg1.angular.z = 0.2;
        chatter1_pub .publish(msg1);
      }
      else{
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.0;
        chatter1_pub .publish(msg1); 
        inc_yaw = 0.0;
        fsm = "go_straightU";
        start_odom.pose.pose.position.x = sub_obj.odom_msg_.pose.pose.position.x;
        start_odom.pose.pose.position.y = sub_obj.odom_msg_.pose.pose.position.y;
      }
    }
    else if(fsm=="go_straightU"){
      if(dis_s <1.0){
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.1;
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
      inc_yaw = inc_yaw + d_yaw;
      if( inc_yaw  < 1.57  ){
        geometry_msgs::Twist msg1;
        msg1.angular.z = 0.2;
        chatter1_pub .publish(msg1);
      }
      else{
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.0;
        chatter1_pub .publish(msg1); 
        inc_yaw = 0.0;
        fsm = "go_straightL";
        start_odom.pose.pose.position.x = sub_obj.odom_msg_.pose.pose.position.x;
        start_odom.pose.pose.position.y = sub_obj.odom_msg_.pose.pose.position.y;
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

    last_yaw = yaw;

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}