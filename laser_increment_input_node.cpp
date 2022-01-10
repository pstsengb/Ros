
  
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/PoseArray.h"
#include "tf/transform_broadcaster.h"
#include "sensor_msgs/LaserScan.h"

class MySub{

    public:
        MySub(ros::NodeHandle m_nh);
        void cmdvelCb(const sensor_msgs::LaserScan::ConstPtr& msg);
        nav_msgs::Odometry odom_msg_;


    private:
        ros::Subscriber sub_;
        ros::NodeHandle nh_;
        
};

MySub::MySub(ros::NodeHandle m_nh){
    nh_ = m_nh;
    sub_ = nh_.subscribe("scan", 10, &MySub::cmdvelCb, this);
}

void MySub::cmdvelCb(const sensor_msgs::LaserScan::ConstPtr& msg)
{
  sensor_msgs::LaserScan abc = *msg;
  std::vector<double> my_vector_;
  std::vector<double> my_vector_1;
  
  my_vector_1.push_back(abc.angle_min);
  my_vector_1.push_back(abc.angle_max);
  my_vector_1.push_back(abc.angle_increment);


  double input_degree_L = my_vector_1[0]/3.14159*180;
  double input_degree_R = my_vector_1[1]/3.14159*180;
  double Resolution = 1/(my_vector_1[2]/3.14159*180); //1 degree = have two increment
  int Resolution_L = input_degree_L*Resolution;
  int Resolution_R = input_degree_R*Resolution;
  int output_L = Resolution_L+(-Resolution_L);
  int output_R = Resolution_R+(Resolution_R+1);
  ROS_INFO("%d, %d, %d, %d ",output_L , output_R ,Resolution_L,Resolution_R );
  double all_value = 0.0;
  double all_value_avg = 0.0;
  for (int i=output_L;i<output_R ;i++){
    my_vector_.push_back(abc.ranges[i]);
    all_value = all_value + my_vector_[i] ;
    all_value_avg = (all_value / (i+1)) ;
    //ROS_INFO("%.2f, %lu, %.2f, %d , %.2f",abc.ranges[i], abc.ranges.size(),all_value,i,all_value_avg );
  }
  ROS_ERROR("---------------------------");

  // double sum_single;
  //double sum_single_avg;
  //for (int k=1;k<401;k=k+2){
  // sum_single = sum_single + my_vector_[k];
  //sum_single_avg = sum_single / ((k+1) / 2);
  //ROS_INFO("%.2f, %d, %.2f ",sum_single,k,sum_single_avg);
  //}
  //ROS_ERROR("---------------------------");

}




int main(int argc, char **argv)
{

  ros::init(argc, argv, "pubsubtest_node");

  ros::NodeHandle n;

  MySub sub_obj(n);

  ros::Publisher chatter_pub = n.advertise<nav_msgs::Odometry>("odomdata2", 1000);
  ros::Publisher pose_array_pub = n.advertise<geometry_msgs::PoseArray>("pose_array2", 10);
  
  geometry_msgs::PoseArray pose_array_;
  pose_array_.header.frame_id = "base_link";



  ros::Rate loop_rate(10);

  while (ros::ok())
  {

    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}

