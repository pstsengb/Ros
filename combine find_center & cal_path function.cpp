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
#include "nav_msgs/Path.h"
#include "geometry_msgs/PoseStamped.h"

class PoseController{

   struct beam_t {
     int son_beam_index;
     double son_x;
     double son_y;
     int parent_beam_index;
     double parent_x;
     double parent_y;
   };

   public:
      PoseController(ros::NodeHandle m_nh);

      void laserscanCb(const sensor_msgs::LaserScan::ConstPtr& msg);
      sensor_msgs::LaserScan laser_scan_msg;
      double point_center_x=0;
      double point_center_y=0;
      double center_angle =0;
      std::vector<double> x_;
      std::vector<double> y_;
      std::vector<int> index_;
      std::pair<double, double> a;
      std::map<int, std::pair<double, double>> map_;
      std::vector<beam_t> result_;
      double tf_x ,tf_y,tf_z,tf_w;

      void move2Pose(double x_start, double y_start, double theta_start, double x_goal, double y_goal, double theta_goal);
      
      bool qq;


   private:
      ros::Subscriber sub_;
      ros::NodeHandle nh_;
      ros::Publisher path_pub_;

      void calcControlCommand(double x_diff, double y_diff, double theta, 
                            double theta_goal, double& rho, double& v, double& w);

      double kp_rho_, kp_alpha_, kp_beta_;
      double pi_;
      double max_linear_speed_, max_angular_speed_;
      
};

 
PoseController::PoseController(ros::NodeHandle m_nh){
    qq=false;
    nh_ = m_nh;
    sub_ = nh_.subscribe("scan", 1, &PoseController::laserscanCb, this);
    path_pub_ = nh_.advertise<nav_msgs::Path>("control_path", 2);
    kp_rho_ = 0.06;
    kp_alpha_ = 0.4;
    kp_beta_ = 0.9;
    pi_ = 3.1415926535;
    max_linear_speed_ = 0.08;
    max_angular_speed_ = 0.25;
}

void PoseController::calcControlCommand(double x_diff, double y_diff, 
              double theta, double theta_goal, double& rho, double& v, double& w){

  rho = hypot(x_diff, y_diff);
  double alpha = fmod((atan2(y_diff, x_diff) - theta + pi_), (2 * pi_)) - pi_;
  double beta = fmod((theta_goal - theta - alpha + pi_), (2 * pi_)) - pi_;
  v = kp_rho_ * rho;
  w = kp_alpha_ * alpha - kp_beta_ * beta;
  if(alpha > pi_/ 2 or alpha < -pi_ / 2)
      v = -v;
}

void PoseController::move2Pose(double x_start, double y_start, double theta_start, double x_goal, double y_goal, double theta_goal){
  double dt = 0.1;
  double x = x_start;
  double y = y_start;
  double theta = theta_start;
  double x_diff = x_goal - x;
  double y_diff = y_goal - y;
  double rho = hypot(x_diff, y_diff);
  double v=0.0, w=0.0;
  std::vector<double> v_vec, w_vec;
  nav_msgs::Path m_path;
  m_path.header.frame_id = "laser";
  m_path.header.stamp = ros::Time::now();
  while(rho > 0.001){
    geometry_msgs::PoseStamped a_pose;
    a_pose.pose.position.x = x;
    a_pose.pose.position.y = y;
    tf::Quaternion q;
    q.setRPY(0, 0, theta);
    geometry_msgs::Quaternion odom_quat;
    tf::quaternionTFToMsg(q, odom_quat);
    a_pose.pose.orientation = odom_quat;
    m_path.poses.push_back(a_pose);
    x_diff = x_goal - x;
    y_diff = y_goal - y;
    calcControlCommand(x_diff, y_diff, theta, theta_goal, rho, v, w);
    if(fabs(v) > max_linear_speed_){
      if(v<0)
        v = -max_linear_speed_;
      else
        v = max_linear_speed_;
    }
    if(fabs(w) > max_angular_speed_){
      if(w<0)
        w = -max_angular_speed_;
      else
        w = max_angular_speed_;
    }    
    v_vec.push_back(v);
    w_vec.push_back(w);
    theta = theta + w * dt;
    x = x + v * cos(theta) * dt;
    y = y + v * sin(theta) * dt;
  }
  path_pub_.publish(m_path);
  ROS_WARN("Linear x: %.2f, Angular z: %.2f", v_vec[0], w_vec[0]);
}
void PoseController::laserscanCb(const sensor_msgs::LaserScan::ConstPtr& msg)
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
          tmp_a.son_x = x;
          tmp_a.son_y = y;
          tmp_a.son_beam_index = i;
          tmp_a.parent_beam_index = it->first;
          tmp_a.parent_x = it->second.first;
          tmp_a.parent_y = it->second.second;
          result_.push_back(tmp_a);
        }
      }
    }
  }
  point_center_x = (result_[0].son_x + result_[0].parent_x)/ 2.0 ;
  point_center_y = (result_[0].son_y + result_[0].parent_y)/ 2.0 ; 
  double length = hypot(result_[0].son_x-result_[0].parent_x,result_[0].son_y-result_[0].parent_y);
  center_angle = atan2(result_[0].son_y-result_[0].parent_y,result_[0].son_x-result_[0].parent_x)-1.57;
  tf::Quaternion q;
  q.setRPY(0, 0, center_angle);
  tf_x = q.x();
  tf_y = q.y();
  tf_z = q.z();
  tf_w = q.w();
  qq = true;
}


int main(int argc, char **argv)
{
 
 ros::init(argc, argv, "test_node");
 
 ros::NodeHandle n;
 
 PoseController sub_obj(n);
 
 ros::Rate loop_rate(20);

 tf::TransformBroadcaster broadcaster;
 while(ros::ok() && !sub_obj.qq){ //in order to wait callback already full
   ROS_ERROR("-------------------------");
   ros::spinOnce();
   loop_rate.sleep();
 }
 
 while (ros::ok())
 {
    sub_obj.move2Pose(0,0,0,sub_obj.point_center_x,sub_obj.point_center_y,sub_obj.center_angle);
    broadcaster.sendTransform(tf::StampedTransform(tf::Transform(tf::Quaternion(sub_obj.tf_x,sub_obj.tf_y,sub_obj.tf_z, sub_obj.tf_w), tf::Vector3(sub_obj.point_center_x, sub_obj.point_center_y, 0.0)),ros::Time::now(),"laser", "center"));
    ros::spinOnce();
    loop_rate.sleep();
 }
 
 
 return 0;
}
