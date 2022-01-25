
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
        sensor_msgs::LaserScan abc;
        std::string laserscan;
        bool qq;
         


    private:
        ros::Subscriber sub_;
        ros::NodeHandle nh_;
        
};

MySub::MySub(ros::NodeHandle m_nh){
    qq=false;
    laserscan = "ready";
    nh_ = m_nh;
    sub_ = nh_.subscribe("scan_topic", 10, &MySub::cmdvelCb, this);
}

void MySub::cmdvelCb(const sensor_msgs::LaserScan::ConstPtr& msg)
{
  abc = *msg;
  std::vector<double> my_vector_1;
  my_vector_1.push_back(2.791); //160 degree
  my_vector_1.push_back(3.488);  //200 degree
  my_vector_1.push_back(abc.angle_increment);
  my_vector_1.push_back(abc.ranges.size());
  double input_degree_L = my_vector_1[0]/3.14159*180;
  double input_degree_R = my_vector_1[1]/3.14159*180;
  double Resolution = 1/(my_vector_1[2]/3.14159*180); 
  int Resolution_L = input_degree_L*Resolution;
  int Resolution_R = (input_degree_R*Resolution)+1;

  int x =0 ;
  int y =0 ;
  for (int i=Resolution_L;i<Resolution_R ;i++){
    if(abc.ranges[i]<0.35){
      y = 1;
    }
  }
  if(y>x){
    laserscan = "haveobstacle";
    y = 0;
  }
  else{
    laserscan = "youcango";
    y = 0;
  }
//ROS_WARN("-----------------------------------------------");
//ROS_WARN(" %.2f,  %.2f,  %.2f,  %.2f,  %.2f",abc.ranges[198],abc.ranges[199],abc.ranges[200],abc.ranges[201],abc.ranges[202]);
//ROS_WARN("%s",laserscan.c_str());
//ROS_WARN("-----------------------------------------------");
qq = true;

}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "test_node");
  ros::NodeHandle n;
  MySub sub_obj(n);

  ros::Publisher chatter1_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);


  ros::Rate loop_rate(20);

  std::string fsm;
  fsm = "initial";

  while(ros::ok() && !sub_obj.qq){ //in order to wait callback already full
    ROS_ERROR("-------------------------");
    ros::spinOnce();
    loop_rate.sleep();
  }

  while (ros::ok())
  {
    ROS_INFO("   %.2f,    %.2f,    %.2f,   %.2f,    %.2f",sub_obj.abc.intensities[888],sub_obj.abc.intensities[950],sub_obj.abc.intensities[1000],sub_obj.abc.intensities[1050],sub_obj.abc.intensities[1110]);
    ROS_INFO("   %.2f,    %.2f,    %.2f,   %.2f,    %.2f",sub_obj.abc.ranges[888],sub_obj.abc.ranges[950],sub_obj.abc.ranges[1000],sub_obj.abc.ranges[1050],sub_obj.abc.ranges[1110]);
    ROS_INFO("laser :  %s",sub_obj.laserscan.c_str());
    ROS_INFO("baselink status : %s",fsm.c_str());
    ROS_WARN("--------------------------------------------------------------");


    if(fsm=="initial"){
      ROS_INFO("System initialized.");
      fsm = "go_straight";
    }
    else if(fsm == "go_straight"){
      if(sub_obj.laserscan== "youcango"){
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.1;
        chatter1_pub .publish(msg1);
        ROS_WARN("%.2f",msg1.linear.x);
      }
      else{
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.0;
        chatter1_pub .publish(msg1);     
        fsm = "go_rotate";
      }
    }
    else if(fsm=="go_rotate"){
      if( sub_obj.laserscan == "haveobstacle" ){
        geometry_msgs::Twist msg1;
        msg1.angular.z = 0.5;
        chatter1_pub .publish(msg1);
        ROS_WARN("%.2f",msg1.angular.z);
      }
      else{
        geometry_msgs::Twist msg1;
        msg1.angular.z = 0.0;
        chatter1_pub .publish(msg1); 
        fsm = "go_straight";
      }
    }
    ros::spinOnce();
    loop_rate.sleep();
    
  }
  return 0;
}



