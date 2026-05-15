from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='pkg10_tf2_cpp',
            executable='static_broadcaster',
            name='static_broadcaster',
        ),
        Node(
            package='pkg10_tf2_cpp',
            executable='dynamic_broadcaster',
            name='dynamic_broadcaster',
        ),
        Node(
            package='pkg10_tf2_cpp',
            executable='listener',
            name='tf_listener',
            output='screen',
        ),
    ])
