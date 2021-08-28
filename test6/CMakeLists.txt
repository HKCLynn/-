CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(test6)

find_package(OpenCV REQUIRED)


include_directories (./ ./Deal ./Light ./Predictor ./Armor ${OpenCV_INCLUDE_DIRS})


set(DEAL_SRC Deal/Deal.cpp)
set(LIGHT_SRC Light/Light.cpp)
set(PREDICTOR_SRC Predictor/Predictor.cpp)
set(ARMOR_SRC Armor/Armor.cpp)


add_executable(test6 test6.cpp ${DIRSRCS} ${DEAL_SRC} ${LIGHT_SRC} ${PREDICTOR_SRC} ${ARMOR_SRC})

link_directories(${OpenCV_LIBRARY_DIRS})

target_link_libraries(test6 ${OpenCV_LIBS})
