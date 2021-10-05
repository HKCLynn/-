add_library(GyroScope GyroScope.cpp)

target_include_directories(GyroScope PUBLIC .)

target_link_libraries(GyroScope Parameter)