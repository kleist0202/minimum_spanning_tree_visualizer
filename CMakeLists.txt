cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)

project(minimum_spanning_tree)

find_package(SDL2 REQUIRED)
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

add_executable(${PROJECT_NAME} window.cpp main.cpp classes.cpp kruskal.cpp prim.cpp helperFunctions.cpp) # here, add files to compile

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES})
