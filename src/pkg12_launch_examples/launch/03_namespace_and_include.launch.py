"""
03_namespace_and_include.launch.py
==================================
Runs the TF demo from package pkg10_tf2_cpp under a ROS namespace,
showing how to compose launch files.

Run it:
  ros2 launch pkg12_launch_examples 03_namespace_and_include.launch.py
"""

import os
from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, GroupAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import PushRosNamespace


def generate_launch_description():
    tf_launch = os.path.join(
        get_package_share_directory('pkg10_tf2_cpp'),
        'launch',
        'tf_demo.launch.py'
    )

    return LaunchDescription([
        GroupAction([
            PushRosNamespace('robot1'),     # everything below lives under /robot1
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(tf_launch)
            ),
        ]),
    ])
