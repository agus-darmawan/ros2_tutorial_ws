from setuptools import find_packages, setup

package_name = 'pkg11_tf2_py'

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
    description='ROS2 Tutorial - TF2 in Python',
    license='BSD-3-Clause',
    entry_points={
        'console_scripts': [
            'static_broadcaster = pkg11_tf2_py.static_broadcaster:main',
            'dynamic_broadcaster = pkg11_tf2_py.dynamic_broadcaster:main',
            'listener = pkg11_tf2_py.listener:main',
        ],
    },
)
