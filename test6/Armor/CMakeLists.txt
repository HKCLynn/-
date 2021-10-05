find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

add_library(Armor Armor.cpp)

target_include_directories(Armor PUBLIC .)
target_include_directories(Armor PUBLIC ${OpenCV_INCLUDE_DIRS})
target_include_directories(Armor PUBLIC ${Eigen3_INCLUDE_DIRS})

target_link_libraries(Armor Parameter Light Deal  ${OpenCV_LIBS})