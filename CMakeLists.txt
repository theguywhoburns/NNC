cmake_minimum_required(VERSION 3.22)
project(NNC
	VERSION 0.0.1
	LANGUAGES C
)

if(MSVC)
	add_compile_definitions(/D_CRT_SECURE_NO_WARNINGS)
else()
	add_compile_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

#set output directory of the exe to the bin 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(NNC main.c lexer.c)
target_include_directories(NNC PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})