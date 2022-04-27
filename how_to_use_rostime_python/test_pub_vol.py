#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import rospy
import time
import importlib
import tf
from move_base_msgs.msg import MoveBaseAction, MoveBaseFeedback, MoveBaseResult, MoveBaseGoal
import actionlib
from geometry_msgs.msg import PointStamped


def pubvol():
    vol = PointStamped()
    vol.point.x = 10
    pub.publish(vol) 

def my_callback(event):
    rospy.logerr('sent_vol')
    vol = PointStamped()
    vol.point.x = 5
    pub.publish(vol)


if __name__ == '__main__':

    rospy.init_node('pubvol_node', anonymous=True)
    pub = rospy.Publisher('voltage', PointStamped, queue_size=1)
    #time_trigger_vol = rospy.Timer(rospy.Duration(3), my_callback)

    rate = rospy.Rate(10)
    time_stamp = rospy.Time.now()

    while(not rospy.is_shutdown()):
        if(rospy.Time.now().to_time()-time_stamp.to_time() >= 5.0):
            pubvol()
            time_stamp = rospy.Time.now()
        #pubvol()
        #rospy.logerr('hiiiiiii')
        #time_trigger_vol
        rate.sleep()
 