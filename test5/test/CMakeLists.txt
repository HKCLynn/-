CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(test5)

find_package(OpenCV REQUIRED)

ADD_EXECUTABLE(test5 test5.cpp)

target_link_libraries(test5 PRIVATE ${OpenCV_LIBS})
