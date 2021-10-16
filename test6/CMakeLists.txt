cmake_minimum_required (VERSION 3.10)

project (test6)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(Light)
add_subdirectory(Armor)
add_subdirectory(Deal)
add_subdirectory(Detector)
add_subdirectory(GyroScope)
add_subdirectory(Parameter)
add_subdirectory(RMVideoCapture)
add_subdirectory(SerialPort)
add_subdirectory(Strategy)
add_subdirectory(Visioncontrol)

add_executable(test6 test6.cpp)

target_link_libraries(test6 Visioncontrol)
target_link_libraries(test6 Detector)
