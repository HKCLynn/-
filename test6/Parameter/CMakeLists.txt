find_package(OpenCV REQUIRED)


add_library(Parameter Parameter.cpp)

target_include_directories(Parameter PUBLIC ${OpenCV_INCLUDE_DIRS})
target_include_directories(Parameter PUBLIC .)

target_link_libraries(Parameter ${OpenCV_LIBS})