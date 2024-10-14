cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 14)

project(AStar)

set(SOURCE_FILES 
	include/astar.hpp
	include/astar.cpp
	utils/fs.hpp
	utils/fs.cpp
	utils/types.hpp
)

add_executable(${PROJECT_NAME}
	main.cpp
	${SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME}
	PUBLIC ./utils
	PUBLIC ./include
)
