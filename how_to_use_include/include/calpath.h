#ifndef AUTOCHARGING_POSE_TRACKIG_H_
#define AUTOCHARGING_POSE_TRACKIG_H_

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
     int point;
     int point_parent;
     double G;
     double H;
     double F;
     double close;
   };

    public:
        Mysystem();
        void Astart_Path(int a,int b);
        std::vector<vector_> notuse_;
        std::set<std::pair<float,std::pair<int,int>>> min_list;   //G value,point,parent
        std::set<std::pair<float,std::pair<int,int>>> open_list; //G value,point,parent
        std::set<std::pair<float,std::pair<int,int>>> close_list;
        std::set<std::pair<float,std::pair<int,int>>> path_list; //G value,point,parent
        

    private:

};

#endif  // AUTOCHARGING_POSE_TRACKIG_H_
