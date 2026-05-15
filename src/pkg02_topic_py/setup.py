from setuptools import find_packages, setup

package_name = 'pkg02_topic_py'

setup(
    name=package_name,
    version='0.1.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='agus-darmawan',
    maintainer_email='darmawandeveloper@gmail.com',
    description='ROS2 Tutorial - Topics in Python',
    license='BSD-3-Clause',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'publisher_node = pkg02_topic_py.publisher_node:main',
            'subscriber_node = pkg02_topic_py.subscriber_node:main',
        ],
    },
)
