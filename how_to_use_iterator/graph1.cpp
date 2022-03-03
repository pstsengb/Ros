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
#include "geometry_msgs/PoseArray.h"
#include <queue>

class Mysystem{

  struct vector_ {
     double x;
     double y;
   };


    public:
        Mysystem(ros::NodeHandle in);
        void Astart_Path();
        std::vector<vector_> expan_;

    private:
        ros::NodeHandle n_;

};

Mysystem::Mysystem(ros::NodeHandle in){
    n_ = in;

}


void Mysystem::Astart_Path(){
  std::map<int, std::set<std::pair<int,float>>> vertex;


  std::set<std::pair<int,float>> tmp_set;
  tmp_set.insert(std::make_pair(3, 1.4));
  tmp_set.insert(std::make_pair(4, 2.7));
  vertex[1] = tmp_set;
  tmp_set.clear();
  tmp_set.insert(std::make_pair(3, 1.5));
  vertex[2] = tmp_set;
  tmp_set.clear();
  tmp_set.insert(std::make_pair(1, 1.4));
  tmp_set.insert(std::make_pair(2, 1.5));
  tmp_set.insert(std::make_pair(7, 2.5));
  vertex[3] = tmp_set;  
  tmp_set.clear();
  tmp_set.insert(std::make_pair(1, 2.7));
  tmp_set.insert(std::make_pair(9, 1.6));
  tmp_set.insert(std::make_pair(6, 3.5));
  tmp_set.insert(std::make_pair(7, 0.8));
  vertex[4] = tmp_set;  
  tmp_set.clear();
  tmp_set.insert(std::make_pair(3, 2.1));
  tmp_set.insert(std::make_pair(6, 1.4));
  vertex[5] = tmp_set;  
  tmp_set.clear();
  tmp_set.insert(std::make_pair(5, 1.4));
  tmp_set.insert(std::make_pair(4, 3.5));
  tmp_set.insert(std::make_pair(7, 0.3));
  vertex[6] = tmp_set; 
  tmp_set.clear();
  tmp_set.insert(std::make_pair(6, 0.3));
  tmp_set.insert(std::make_pair(3, 2.5));
  tmp_set.insert(std::make_pair(4, 0.8));
  tmp_set.insert(std::make_pair(8, 0.9));
  vertex[7] = tmp_set; 
  tmp_set.clear();
  tmp_set.insert(std::make_pair(7, 0.9));
  tmp_set.insert(std::make_pair(9, 1.1));
  vertex[8]= tmp_set; 
  tmp_set.clear();
  tmp_set.insert(std::make_pair(4, 1.6));
  tmp_set.insert(std::make_pair(8, 1.1));
  vertex[9]= tmp_set; 

  /*
  int start = 1;
  int goal = 6;
  int current_node_index = start;
  for(int i=0;i<1;i++){

    std::set<std::pair<int,float>> successor = vertex[current_node_index];

    for(auto it=successor.begin();it!=successor.end();it++){
      double g = current_g + (*it).second;
      double h = 0;
      double f = g;
      ROS_ERROR("expand %d from %d, distance = %.2f", (*it).first, start, (*it).second);

    }

    //-->minimum f
    current_node_index = 3;

  }
  */
  std::set<int> a;
  std::map<int, int> b;
  std::priority_queue<std::pair<float, int>> c;
  std::set<std::pair<int,float>> d;

  a.insert(9);
  a.insert(3);
  a.insert(2);
  a.insert(5);
  a.insert(1);

  b[1] = 1;
  b[2] = 2;
  b[3] = 3;
  b[5] = 5;

  for(auto it=a.begin();it!=a.end();it++){
    ROS_ERROR("%d", (*it));
  }

  for(auto it=b.begin();it!=b.end();it++){
    ROS_WARN("%d", (*it).second);
  }
  
  c.push(std::make_pair(-3.2, 1));
  c.push(std::make_pair(-4.2, 3));
  c.push(std::make_pair(-1.3, 4));
  c.push(std::make_pair(-6.3, 7));
  std::pair<float, int> mtmp = c.top();
  c.pop();
  ROS_INFO("%f, %d", mtmp.first, mtmp.second);

  mtmp = c.top();
  ROS_INFO("%f, %d", mtmp.first, mtmp.second);

  d.insert(std::make_pair(1, 2.2));
  d.insert(std::make_pair(4, 2.1));
  d.insert(std::make_pair(2, 3.1));
  d.insert(std::make_pair(5, 1.2));
  d.insert(std::make_pair(1, 2.2));
  for(auto it=d.begin();it!=d.end();it++){
    ROS_WARN("--->%d, %.2f", (*it).first, (*it).second);
}
}

int main(int argc, char **argv)
{ 
 ros::init(argc, argv, "test_node");
 ros::NodeHandle n;
 Mysystem drive(n);
 
 drive.Astart_Path();
 
 ros::Rate loop_rate(20);
 
 while (ros::ok())  
 {
   ros::spinOnce();
   loop_rate.sleep();
 }
 return 0;
}

