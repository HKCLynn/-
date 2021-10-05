find_package(OpenCV REQUIRED)

add_library(Light Light.cpp)

target_include_directories(Light PUBLIC .)

target_link_libraries(Light Armor)