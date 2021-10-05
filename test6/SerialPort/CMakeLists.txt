find_package(OpenCV REQUIRED)

add_library(SerialPort SerialPort.cpp)

target_include_directories(SerialPort PUBLIC .)
target_include_directories(SerialPort PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries(SerialPort ${OpenCV_LIBS})