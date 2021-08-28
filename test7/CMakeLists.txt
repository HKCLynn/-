CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(test7)

find_package(OpenCV REQUIRED)

aux_source_directory(. DIRSRCS)


include_directories(./ ./DealWith ./Identifier ./Prediction)
include_directories(/usr/local/include/opencv4)

add_subdirectory(DealWith)
add_subdirectory(Identifier)
add_subdirectory(Prediction)


add_executable(test7 ${DIRSRCS})


target_link_libraries(test7 ${OpenCV_LIBS} Prediction Identifier DealWith)


