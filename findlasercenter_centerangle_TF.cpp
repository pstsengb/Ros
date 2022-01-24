#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/PoseArray.h"
#include "tf/transform_broadcaster.h"
#include "sensor_msgs/LaserScan.h"
#include "tf/tf.h"
#include <tf/transform_broadcaster.h>
#include <math.h>


class MySub{

    public:
        MySub(ros::NodeHandle m_nh);
        void cmdvelCb(const sensor_msgs::LaserScan::ConstPtr& msg);
        nav_msgs::Odometry odom_msg_;
        sensor_msgs::LaserScan abc;
        bool qq;
        double point_center_x=0;
        double point_center_y=0;
        double center_angle=0;
        nav_msgs::Odometry odom_lasercenter;
    private:
        ros::Subscriber sub_;
        ros::NodeHandle nh_;
        
};

MySub::MySub(ros::NodeHandle m_nh){
    qq=false;
    nh_ = m_nh;
    sub_ = nh_.subscribe("scan", 10, &MySub::cmdvelCb, this);
}

void MySub::cmdvelCb(const sensor_msgs::LaserScan::ConstPtr& msg)
{  
  abc = *msg;
  std::vector<double> my_vector_;
  std::vector<double> my_vector_1;
  std::vector<double> my_vector_2;
  std::vector<double> my_vector_3;
  std::vector<double> my_vector_left;
  std::vector<double> my_vector_right;
  std::vector<double> my_vector_point1;
  std::vector<double> my_vector_point2;

  int count = 0;
  int angleincrement = 0;
  double laserangle = 0;

  
  for (int i=0;i<abc.intensities.size() ;i++){
    count ++;
    if(abc.intensities[i]>3000){
      my_vector_.push_back(abc.intensities[i]);
      my_vector_1.push_back(abc.ranges[i]);
      my_vector_2.push_back(count);
    }
  }
  
  for(int ii=0;ii<my_vector_.size();ii++){
    if (my_vector_2[ii]<500){
      angleincrement = 501-my_vector_2[ii];
      laserangle = angleincrement * 0.25;
    }
    else{
      angleincrement = my_vector_2[ii]-501;
      laserangle = angleincrement * 0.25;
    }
    my_vector_3.push_back(laserangle);
   // ROS_INFO("  intensities:%.2f, lenght:%.2f, L-R increment:%.2f, angle(center is 0):%.2f,",my_vector_[ii] ,my_vector_1[ii],my_vector_2[ii],my_vector_3[ii]);
    
  }

  int min_count =0;
  double m_min = my_vector_3[0];
  for(int i=0;i<my_vector_3.size();i++){
    if(my_vector_3[i]<m_min){
      m_min = my_vector_3[i]; 
      min_count = i;
    }
  }

  int max_count =0;
  double m_max = my_vector_3[0];
  for(int i=0;i<my_vector_3.size();i++){
    if(my_vector_3[i]>m_max){
      m_max = my_vector_3[i]; 
      max_count = i;
    }
  }
  //ROS_WARN("min_angel:%.2f,in my_vector_3 position:%d", m_min,min_count);
  //ROS_WARN("max_angel:%.2f,in my_vector_3 position:%d", m_max,max_count);

  
  //my_vector_left[0] = left vector lenght
  //my_vector_left[1] = left vector angle
  //my_vector_right[0] = right vector lenght
  //my_vector_right[1] = right vector angle
  int positive_or_negative =1;

  for(int iii=0;iii<my_vector_.size();iii++){
    if (my_vector_2[iii]<500){
      my_vector_left.push_back(my_vector_1[max_count]);
      my_vector_left.push_back(my_vector_3[max_count]);
      my_vector_right.push_back(my_vector_1[min_count]);
      my_vector_right.push_back(my_vector_3[min_count]);
      positive_or_negative =-1;
    }
    else{
      my_vector_left.push_back(my_vector_1[min_count]);
      my_vector_left.push_back(my_vector_3[min_count]);
      my_vector_right.push_back(my_vector_1[max_count]);
      my_vector_right.push_back(my_vector_3[max_count]);
      positive_or_negative = 1;
    }
    
  }
  //ROS_WARN("my_vector_left_length:%.2f,my_vector_left_angle:%.2f", my_vector_left[0],my_vector_left[1]);
  //ROS_WARN("my_vector_right_length:%.2f,my_vector_right_angle:%.2f", my_vector_right[0],my_vector_right[1]);
  

  double point1_angle = my_vector_left[1]*(3.14/180);
  double point2_angle = my_vector_right[1]*(3.14/180);
  double point1_length = my_vector_left[0];
  double point2_length = my_vector_right[0];
 // ROS_WARN("point1 length :%.2f,point2 length:%.2f,point1 angle :%.2f,point2 angle:%.2f", point1_length,point2_length,point1_angle,point2_angle);
  
  double point1_x = point1_length * cos(point1_angle);
  double point1_y = positive_or_negative * point1_length * sin(point1_angle);
  double point2_x = point2_length * cos(point2_angle);
  double point2_y = positive_or_negative * point2_length * sin(point2_angle);
  point_center_x = (point1_x+point2_x)/2;
  point_center_y = (point1_y+point2_y)/2;
  double point_length = sqrt((point1_x-point2_x)*(point1_x-point2_x)+(point1_y-point2_y)*(point1_y-point2_y));
  double lengthh = hypot(point1_x-point2_x,point1_y-point2_y);
   //ROS_INFO("point1 x :%.3f,point1 y:%.3f",point1_x,point1_y);
   //ROS_INFO("point2 x :%.3f,point2 y:%.3f",point2_x,point2_y);
   //ROS_INFO("2_point_length :%.3f",point_length);
   //ROS_INFO("lengthhh :%.3f",lengthh);
   //ROS_ERROR("---------------------------");
   //atan2(point_center_y,point_center_x);
  center_angle = atan2(point_center_y,point_center_x);
  tf::Quaternion q;
  q.setRPY(0, 0, center_angle);
  odom_lasercenter.pose.pose.orientation.x = q.x();
  odom_lasercenter.pose.pose.orientation.y = q.y();
  odom_lasercenter.pose.pose.orientation.z = q.z();
  odom_lasercenter.pose.pose.orientation.w = q.w();


  qq = true;
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "test_node");

  ros::NodeHandle n;

  MySub sub_obj(n);

  ros::Rate loop_rate(10);
  tf::TransformBroadcaster broadcaster;

  while(ros::ok() && !sub_obj.qq){ //in order to wait callback already full
    ROS_ERROR("-------------------------");
    ros::spinOnce();
    loop_rate.sleep();
  }

  while (ros::ok())
  {
    ROS_WARN("center x:%.3f", sub_obj.point_center_x);
    ROS_WARN("center y:%.3f", sub_obj.point_center_y);
    ROS_WARN("center angle:%.3f", sub_obj.center_angle);
    //ROS_INFO("orientation.x :%.2f",sub_obj.odom_lasercenter.pose.pose.orientation.x);
    //ROS_INFO("orientation.y :%.2f",sub_obj.odom_lasercenter.pose.pose.orientation.y);
    //ROS_INFO("orientation.z :%.2f",sub_obj.odom_lasercenter.pose.pose.orientation.z);
    //ROS_INFO("orientation.w :%.2f",sub_obj.odom_lasercenter.pose.pose.orientation.w);
    broadcaster.sendTransform(tf::StampedTransform(tf::Transform(tf::Quaternion(0, 0,sub_obj.odom_lasercenter.pose.pose.orientation.z, sub_obj.odom_lasercenter.pose.pose.orientation.w), tf::Vector3(sub_obj.point_center_x, sub_obj.point_center_y, 0.0)),ros::Time::now(),"laser", "center"));
    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}