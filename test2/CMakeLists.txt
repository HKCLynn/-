CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(test2)

find_package(OpenCV REQUIRED)

ADD_EXECUTABLE(test2 test2.cpp)

target_link_libraries(test2 PRIVATE ${OpenCV_LIBS})

