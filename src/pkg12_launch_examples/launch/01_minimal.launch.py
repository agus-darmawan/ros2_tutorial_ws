"""
01_minimal.launch.py
====================
The smallest useful launch file: start the publisher and subscriber from
package `pkg01_topic_cpp` together.
"""

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='pkg01_topic_cpp',
            executable='publisher_node',
            name='my_publisher',          # rename the node at runtime
            output='screen',              # show logs in this terminal
        ),
        Node(
            package='pkg01_topic_cpp',
            executable='subscriber_node',
            name='my_subscriber',
            output='screen',
        ),
    ])
