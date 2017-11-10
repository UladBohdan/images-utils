cmake_minimum_required(VERSION 2.8)
project(images_utils)

set (CMAKE_CXX_STANDARD 11)

find_package(catkin REQUIRED COMPONENTS cv_bridge image_transport)

catkin_package(CATKIN_DEPENDS cv_bridge image_transport)

find_package(OpenCV)

include_directories(include ${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

add_executable(frames_publisher src/frames_publisher.cpp)
add_dependencies(frames_publisher ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(frames_publisher ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

install(TARGETS frames_publisher
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
