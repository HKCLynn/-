find_package(OpenCV REQUIRED)

add_library(RMVideoCapture RMVideoCapture.cpp)

target_include_directories(RMVideoCapture PUBLIC .)
target_include_directories(RMVideoCapture PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries(RMVideoCapture Parameter SerialPort libMVSDK.so ${OpenCV_LIBS})
