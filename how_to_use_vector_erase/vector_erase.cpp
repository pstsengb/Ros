std::vector<int> vec;
for(int i=0;i<10;i++){
  vec.push_back(i);
}

for(auto it=vec.begin();it!=vec.end();){

  if(*it%2==0){
    vec.erase(it);
  }
  else{
    it++;
  }
}

for(auto it=vec.begin();it!=vec.end();it++)
  ROS_ERROR("%d",*it);


for(auto it=vec.begin();it!=vec.end();){

  if(*it==2){
    vec.erase(it);
  }
  else if(*it==3){
    *it = 30;
  }
  else{
  it++;
  }
}
