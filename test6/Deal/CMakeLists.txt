find_package(OpenCV REQUIRED)

add_library(Deal Deal.cpp)


target_include_directories(Deal PUBLIC ${OpenCV_INCLUDE_DIRS})
target_include_directories(Deal PUBLIC .)

target_link_libraries(Deal ${OpenCV_LIBS})
