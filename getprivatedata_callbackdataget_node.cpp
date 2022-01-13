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
        bool qq;
        std::string getA();
         


    private:
        std::string laserscan;
        ros::Subscriber sub_;
        ros::NodeHandle nh_;
        
};

MySub::MySub(ros::NodeHandle m_nh){
    qq=false;
    laserscan = "ready";
    nh_ = m_nh;
    sub_ = nh_.subscribe("scan", 10, &MySub::cmdvelCb, this);
}
std::string MySub::getA(){
  return laserscan;
}

void MySub::cmdvelCb(const sensor_msgs::LaserScan::ConstPtr& msg)
{
   abc = *msg;
  std::vector<double> my_vector_1;

  my_vector_1.push_back(-0.3489); //-20 degree
  //my_vector_1.push_back(0.3489);  //20 degree
  //my_vector_1.push_back(abc.angle_increment);
  //my_vector_1.push_back(abc.ranges.size());


  //double input_degree_L = my_vector_1[0]/3.14159*180;
  //double input_degree_R = my_vector_1[1]/3.14159*180;
  //double Resolution = 1/(my_vector_1[2]/3.14159*180); //1 degree = have two increment
  //int Resolution_L = input_degree_L*Resolution;
 // int Resolution_R = input_degree_R*Resolution;
 // int size = my_vector_1[3];
 // int output_L = (my_vector_1[3]/2) - (-Resolution_L);
 // int output_R = (my_vector_1[3]/2) + Resolution_R+1;
 // ROS_INFO("%d, %d ,%d",output_L , output_R  ,size);

  int x =0 ;
  int y =0 ;
  laserscan = "ready";
  for (int i=198;i<203 ;i++){
    if(abc.ranges[i]<2.0){
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

//ROS_INFO(" %.2f,  %.2f,  %.2f,  %.2f,  %.2f",abc.ranges[198],abc.ranges[199],abc.ranges[200],abc.ranges[201],abc.ranges[202]);
ROS_INFO(" %.2f",my_vector_1[0]);
ROS_INFO("%s",laserscan.c_str());
ROS_WARN("-----------------------------------------------");
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
  while(ros::ok() && !sub_obj.qq){
    ROS_ERROR("-------------------------");
    ros::spinOnce();
    loop_rate.sleep();
  }

  
  ROS_INFO(" %.2f",sub_obj.abc.ranges[198]);
  ROS_INFO(" %s",sub_obj.getA().c_str());

  while (ros::ok())
  {
    if(fsm=="initial"){
      ROS_INFO("System initialized.");
      fsm = "go_straight";
    }
    else if(fsm=="go_straight"){
      //if(laserscan== "youcango"){
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.1;
        chatter1_pub .publish(msg1);
      }
      else{
        geometry_msgs::Twist msg1;
        msg1.linear.x = 0.0;
        chatter1_pub .publish(msg1);     
        fsm = "go_rotate";
      }
    

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}

