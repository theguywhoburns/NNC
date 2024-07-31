cmake_minimum_required(VERSION 3.22)
project(NNC
	VERSION 0.0.1
	LANGUAGES C
)

add_executable(main main.c lexer.c)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})