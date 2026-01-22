cmake_minimum_required(VERSION 3.8)
project(mechano_viz)

find_package(ament_cmake REQUIRED)

install(DIRECTORY
  rviz
  launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
