import os
from glob import glob
from setuptools import find_packages, setup

package_name = 'pkg09_parameters_py'

setup(
    name=package_name,
    version='0.1.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'config'), glob('config/*.yaml')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='agus-darmawan',
    maintainer_email='darmawandeveloper@gmail.com',
    description='ROS2 Tutorial - Parameters in Python',
    license='BSD-3-Clause',
    entry_points={
        'console_scripts': [
            'params_node = pkg09_parameters_py.params_node:main',
        ],
    },
)
