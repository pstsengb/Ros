#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import rospy
import actionlib
import time
import sys
from share_code.msg import as_testAction, as_testFeedback, as_testResult, as_testGoal


# Called once when the goal completes
def doneCb(goal_status, data):
    pass
    
# Called once when the goal becomes active
def activeCb():
    pass

# Called every time feedback is received for the goal
def feedbackCb(feedback):
    pass
    #print(feedback.status)

rospy.init_node('trigger_as_test_node')
as_client = actionlib.SimpleActionClient("as_test", as_testAction)
as_client.wait_for_server()

m_goal = as_testGoal()
m_goal.action_token = "111"
m_goal.start_point = 99.9
m_goal.end_point = 88.8
as_client.send_goal(m_goal, doneCb, activeCb, feedbackCb)
time.sleep(5.0)
m_goal.action_token = "222"
m_goal.start_point = 77.7
m_goal.end_point = 66.6
as_client.send_goal(m_goal, doneCb, activeCb, feedbackCb)
time.sleep(5.0)
as_client.cancel_goal()
