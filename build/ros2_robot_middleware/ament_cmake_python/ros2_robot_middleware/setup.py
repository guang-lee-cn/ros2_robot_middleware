from setuptools import find_packages
from setuptools import setup

setup(
    name='ros2_robot_middleware',
    version='0.3.0',
    packages=find_packages(
        include=('ros2_robot_middleware', 'ros2_robot_middleware.*')),
)
