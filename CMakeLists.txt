cmake_minimum_required(VERSION 3.22)
project(NNC
	VERSION 0.0.1
	LANGUAGES CXX C # C is currently unused, C++ 23
)

if(MSVC)
	add_compile_definitions(/D_CRT_SECURE_NO_WARNINGS)
else()
	add_compile_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

#set output directory of the exe to the bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(NNC main.cpp lexer.cpp)
target_include_directories(NNC PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(NNC PRIVATE cxx_std_23)