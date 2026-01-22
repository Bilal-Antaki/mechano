cmake_minimum_required(VERSION 3.8)
project(mechano_node)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(mechano_msgs REQUIRED)

# Main node executable
add_executable(mechano_node
  src/mechano_node.cpp
  src/hmi.cpp
  src/battery_monitor.cpp
  src/safety_monitor.cpp
)

target_include_directories(mechano_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(mechano_node
  rclcpp
  std_msgs
  std_srvs
  geometry_msgs
  sensor_msgs
  mechano_msgs
)

install(TARGETS
  mechano_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  config
  launch
  include/
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
