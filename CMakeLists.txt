cmake_minimum_required(VERSION 3.5)
project(object_detect)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set(CMAKE_PREFIX_PATH "/home/swearos/Documents/deep_learning/caffe-faster-rcnn-dev/build/install/share")
set(CMAKE_PREFIX_PATH "/home/swearos/Documents/deep_learning/pva-faster-rcnn/caffe-fast-rcnn/build/install/share")
find_package( Caffe REQUIRED )

#include_directories( /home/swearos/Documents/deep_learning/caffe-faster-rcnn/include)
include_directories( ${Caffe_INCLUDE_DIRS} )
include_directories(include/)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)
AUX_SOURCE_DIRECTORY(include/ INCLUDE_DIR)
add_executable(${PROJECT_NAME} ${DIR_SRCS} ${INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${Caffe_LIBRARIES}  -lhdf5_hl )
