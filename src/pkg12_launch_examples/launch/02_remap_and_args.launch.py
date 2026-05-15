"""
02_remap_and_args.launch.py
===========================
Shows the two most common launch tricks:
  1. Remap topic names so two nodes that would otherwise collide work together.
  2. Accept a command-line argument and pass it as a parameter.

Run it:
  ros2 launch pkg12_launch_examples 02_remap_and_args.launch.py rate:=5.0
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    # An argument the user can pass at the command line
    rate_arg = DeclareLaunchArgument(
        'rate',
        default_value='1.0',
        description='Loop rate for the params_node in Hz'
    )

    return LaunchDescription([
        rate_arg,

        # Publisher with a renamed topic
        Node(
            package='pkg01_topic_cpp',
            executable='publisher_node',
            name='pub_a',
            remappings=[('chatter', '/robot_a/chatter')],
        ),
        Node(
            package='pkg01_topic_cpp',
            executable='publisher_node',
            name='pub_b',
            remappings=[('chatter', '/robot_b/chatter')],
        ),

        # pkg08_parameters_cpp node with a value from the launch argument
        Node(
            package='pkg08_parameters_cpp',
            executable='params_node',
            name='params_node',
            parameters=[{
                'loop_rate_hz': LaunchConfiguration('rate'),
            }],
        ),
    ])
