add_library(Strategy Strategy.cpp)

target_include_directories(Strategy PUBLIC .)

target_link_libraries(Strategy Armor SerialPort GyroScope)