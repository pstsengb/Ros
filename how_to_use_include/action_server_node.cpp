#include "ros/ros.h"
#include <actionlib/server/simple_action_server.h>
#include <share_code/as_testAction.h>
#include "calpath.h"


class as_test{
  
  typedef actionlib::SimpleActionServer<share_code::as_testAction> TestActionServer;

  public:
    as_test(ros::NodeHandle n);
    
  private:
    TestActionServer* as_;
    void executeCb(const share_code::as_testGoalConstPtr &goal);
};

as_test::as_test(ros::NodeHandle n){

    as_ = new TestActionServer(n, "as_test", boost::bind(&as_test::executeCb, this, _1), false);
    as_->start();

}

void as_test::executeCb(const share_code::as_testGoalConstPtr &goal){

  ROS_ERROR("Receive goal: %s, %d, %d", goal->action_token.c_str(), goal->start_point, goal->end_point);

  

  ros::Rate r(1);
  ros::NodeHandle n;
  int path_start_point =  goal->start_point;
  int path_end_point = goal->end_point;

  while(n.ok())
  {
    ROS_INFO_THROTTLE(5.0, "Spinning");
    std::shared_ptr<Mysystem> Mysystem_ptr_;
    Mysystem_ptr_ = std::make_shared<Mysystem>();
    Mysystem_ptr_->Astart_Path(path_start_point,path_end_point);



    if(as_->isPreemptRequested()){
      if(as_->isNewGoalAvailable()){
        //if we're active and a new goal is available, we'll accept it, but we won't shut anything down
        share_code::as_testGoal new_goal = *as_->acceptNewGoal();
        ROS_INFO("Accept new goal with value: %s, %d,%d", new_goal.action_token.c_str(), new_goal.start_point , new_goal.end_point);
        path_start_point = new_goal.start_point;
        path_end_point = new_goal.end_point;   
      }
      else {
        ROS_WARN("Action has been cancelled");
        as_->setPreempted();
        break;
      }  
    }
   



    share_code::as_testFeedback feedback;
    feedback.status = 1;
    as_->publishFeedback(feedback);

    r.sleep();

  }

}



int main(int argc, char **argv)
{
 
 ros::init(argc, argv, "as_test_node");
 
 ros::NodeHandle n,pn("~");
 
 as_test action_server_test(n);

 ros::Rate loop_rate(20);

 while (ros::ok())  
 {
   ros::spinOnce();
   loop_rate.sleep();
 }
 
 
 return 0;
}
