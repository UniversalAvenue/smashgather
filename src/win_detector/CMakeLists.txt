cmake_minimum_required(VERSION 2.8)
project( WinDetector )
find_package( OpenCV REQUIRED )
file(GLOB WinDetector_SRC
  "*.h"
  "*.cpp"
)
add_executable( WinDetector ${WinDetector_SRC} )
target_link_libraries( WinDetector ${OpenCV_LIBS} )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
