#include "calpath.h"

Mysystem::Mysystem(){

}

void Mysystem::Astart_Path(int a,int b){
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

  int start_point = a;
  int goal_point = b;

  vector_ tmp_current;
  tmp_current.point = start_point;
  tmp_current.point_parent =0;
  tmp_current.G = 0.0;


  std::pair<int,int> start_tmp_ = std::pair<int,int>(start_point,0 );
  close_list.insert(std::make_pair(0, start_tmp_));
  

  bool done = false;
  while(!done){
    std::set<std::pair<int,float>> successor = vertex[tmp_current.point];
    for(auto it=successor.begin();it!=successor.end();it++){
      vector_ tmp_a;
      tmp_a.point = it->first;
      tmp_a.point_parent = tmp_current.point;
      tmp_a.G = tmp_current.G + it->second;
      tmp_a.close = 0.0;
      bool tmp_in_close = false;
      for(auto it=close_list.begin();it!=close_list.end();it++){
        if(tmp_a.point == it->second.first){
          tmp_in_close = true;
          break;
        }
      }
      
      for(auto iit=open_list.begin();iit!=open_list.end();){
        if(tmp_a.point == iit->second.first && tmp_a.G < iit->first){
          open_list.erase(iit++);
        }
        else{
          iit++;
        }
      }
      
      
      if(!tmp_in_close){ 
        std::pair<int,int> tmp_ = std::pair<int,int>(tmp_a.point,tmp_a.point_parent );
        open_list.insert(std::make_pair(tmp_a.G, tmp_));
      }
    }

    for(auto it=close_list.begin();it!=close_list.end();it++){
      if(it->second.first == goal_point ){
        done = true;
      }
    }
    if(done){
      ROS_ERROR("goal:[%d] add to close ",goal_point);
      break;
    }
    
    if(open_list.empty()){
      ROS_ERROR("open-list nothing,break");
      break;
    }
    else{
      ROS_ERROR("open-list still have ,size is [%lu]",open_list.size());
    }
  
    for(auto it=open_list.begin();it!=open_list.end();){
      std::pair<int,int> tmp_ = std::pair<int,int>(it->second.first,it->second.second );
      close_list.insert(std::make_pair(it->first, tmp_));
      tmp_current.point = it->second.first;
      tmp_current.G = it->first;
      open_list.erase(it);
      break;
    }
  }
}


