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

   struct beam_t {
     int index;
     double x;
     double y;
     int parent_beam_index;
     double parent_x;
     double parent_y;
   };

   public:
       MySub(ros::NodeHandle m_nh);
       void cmdvelCb(const sensor_msgs::LaserScan::ConstPtr& msg);
       nav_msgs::Odometry odom_msg_;
       sensor_msgs::LaserScan laser_scan_msg;
       bool qq;
       double point_center_x=0;
       double point_center_y=0;
       double poitmp_ant_center_y=0;
       nav_msgs::Odometry odom_lasercenter_;
       std::vector<double> x_;
       std::vector<double> y_;
       std::vector<int> index_;
       std::pair<double, double> a;
       std::map<int, std::pair<double, double>> map_;
       std::vector<beam_t> result_;
   private:
       ros::Subscriber sub_;
       ros::NodeHandle nh_;
      
};

 
MySub::MySub(ros::NodeHandle m_nh){
   qq=false;
   nh_ = m_nh;
   sub_ = nh_.subscribe("scan", 1, &MySub::cmdvelCb, this);
}
 
void MySub::cmdvelCb(const sensor_msgs::LaserScan::ConstPtr& msg)
{ 

  laser_scan_msg = *msg;
  x_.clear();
  y_.clear();
  index_.clear();
  double tolerance = 0.02;
  double chg_width = 0.34;
  map_.clear();
  result_.clear();
  
  for(int i=0;i<laser_scan_msg.intensities.size();i++){

    if(laser_scan_msg.intensities[i]>3000){

      double angle = (laser_scan_msg.angle_min + i*laser_scan_msg.angle_increment);
    
      double x = laser_scan_msg.ranges[i] * cos(angle);
      double y = laser_scan_msg.ranges[i] * sin(angle);

      map_[i] = std::pair<double,double>(x,y);

      for(auto it=map_.begin();it!=map_.end();it++){

        double dis = hypot(x - it->second.first, y-it->second.second);
        if(fabs(dis-chg_width)<tolerance){
          beam_t tmp_a;
          tmp_a.x = x;
          tmp_a.y = y;
          tmp_a.index = i;
          tmp_a.parent_beam_index = it->first;
          tmp_a.parent_x = it->second.first;
          tmp_a.parent_y = it->second.second;
          result_.push_back(tmp_a);
        }

      }

    }
  }
for(int ii=0;ii<result_.size();ii++){
  ROS_INFO("%d, %.2f, %.2f ---> %d, %.2f, %.2f", 
      result_[ii].index, result_[ii].x, result_[ii].y, result_[ii].parent_beam_index, result_[ii].parent_x, result_[ii].parent_y);
}
point_center_x = (result_[0].x + result_[0].parent_x)/ 2.0 ;
point_center_y = (result_[0].y + result_[0].parent_y)/ 2.0 ;
double length = hypot(result_[0].x-result_[0].parent_x,result_[0].y-result_[0].parent_y);
double center_angle = atan2(result_[0].y-result_[0].parent_y,result_[0].x-result_[0].parent_x)-1.57;
tf::Quaternion q;
q.setRPY(0, 0, center_angle);
odom_lasercenter_.pose.pose.orientation.x = q.x();
odom_lasercenter_.pose.pose.orientation.y = q.y();
odom_lasercenter_.pose.pose.orientation.z = q.z();
odom_lasercenter_.pose.pose.orientation.w = q.w();

ROS_INFO(" %.2f, %.2f, %.2f, %.2f",point_center_x,point_center_y,length,center_angle);
ROS_WARN("---------------------------------------------------------------------------");

}
 
int main(int argc, char **argv)
{
 
 ros::init(argc, argv, "test_node");
 
 ros::NodeHandle n;
 
 MySub sub_obj(n);
 
 ros::Rate loop_rate(20);
 tf::TransformBroadcaster broadcaster;
 /*
 while(ros::ok() && !sub_obj.qq){ //in order to wait callback already full
   ROS_ERROR("-------------------------");
   ros::spinOnce();
   loop_rate.sleep();
 }
 */
 while (ros::ok())
 {
   broadcaster.sendTransform(tf::StampedTransform(tf::Transform(tf::Quaternion(sub_obj.odom_lasercenter_.pose.pose.orientation.x,sub_obj.odom_lasercenter_.pose.pose.orientation.y,sub_obj.odom_lasercenter_.pose.pose.orientation.z, sub_obj.odom_lasercenter_.pose.pose.orientation.w), tf::Vector3(sub_obj.point_center_x, sub_obj.point_center_y, 0.0)),ros::Time::now(),"laser", "center"));
   ros::spinOnce();
 
   loop_rate.sleep();
 }
 
 
 return 0;
}
