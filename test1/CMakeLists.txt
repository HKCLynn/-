CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(test1)

find_package(OpenCV REQUIRED)

ADD_EXECUTABLE(test1 test1.cpp)

target_link_libraries(test1 PRIVATE ${OpenCV_LIBS})
