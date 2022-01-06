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

  for (int i=198;i<203;i++){
    ROS_INFO("%.2f, %lu",abc.ranges[i], abc.ranges.size());
    my_vector_.push_back(abc.ranges[i]);
  }
  double m_min = my_vector_[0];
for(int i=0;i<5;i++){
  if(my_vector_[i]<m_min){
    m_min = my_vector_[i]; 
  }
}
  double m_max = my_vector_[0];
for(int i=0;i<5;i++){
  if(my_vector_[i]>m_max){
    m_max = my_vector_[i]; 
  }
}
    ROS_WARN("min:%.2f", m_min);
    ROS_WARN("max:%.2f", m_max);
ROS_ERROR("---------------------------");

  int ii,jj,temp = 0;
  my_vector_1.push_back(1);
  my_vector_1.push_back(7);
  my_vector_1.push_back(2);
  my_vector_1.push_back(5);
  my_vector_1.push_back(9);
for( ii = 0; ii < 5; ii++) {
       for( jj = 0; jj < 5; jj++) {
           if( my_vector_1[jj] > my_vector_1[ii] ) {
               temp = my_vector_1[ii];
               my_vector_1[ii] = my_vector_1[jj];
               my_vector_1[jj] = temp;
           }
       }
   }

  ROS_WARN("small->large:%.2f,%.2f,%.2f,%.2f,%.2f", my_vector_1[0],my_vector_1[1],my_vector_1[2],my_vector_1[3],my_vector_1[4]);

 


  ROS_ERROR("---------------------------");

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


