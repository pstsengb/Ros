#include "ros/ros.h"
#include "std_srvs/Empty.h"
#include "std_srvs/SetBool.h"
#include "share_code/asdf.h"
class MySrv{

   public:
       MySrv(ros::NodeHandle m_nh, ros::NodeHandle m_pnh);
       bool rosService(share_code::asdf::Request &req, share_code::asdf::Response &resp);
   private:
       ros::ServiceServer srvROS;
       ros::NodeHandle nh_;
       ros::NodeHandle pnh_;
};

 
MySrv::MySrv(ros::NodeHandle m_nh,ros::NodeHandle m_pnh){

  nh_ = m_nh;
  pnh_ = m_pnh;
  srvROS = pnh_.advertiseService("my_srv", &MySrv::rosService, this);

}
 
bool MySrv::rosService(share_code::asdf::Request &req, share_code::asdf::Response &resp)
{ 
  ROS_ERROR("%d,%d", req.a, req.b);
  return true;
}
 
int main(int argc, char **argv)
{
 
 ros::init(argc, argv, "test_node");
 
 ros::NodeHandle n,pn("~");
 
 MySrv sub_obj(n,pn);
 
 ros::Rate loop_rate(20);

 while (ros::ok())  
 {
   ros::spinOnce();
   loop_rate.sleep();
 }
 
 
 return 0;
}
