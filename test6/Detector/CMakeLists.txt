add_library(Detector Detector.cpp)

target_include_directories(Detector PUBLIC .)

target_link_libraries(Detector Strategy SerialPort GyroScope)