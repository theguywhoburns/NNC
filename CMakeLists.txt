cmake_minimum_required(VERSION 3.22)
project(NNC
	VERSION 0.0.1
	LANGUAGES CXX C # C is currently unused, C++ 23
)

set(NNC_MAJOR 0)
set(NNC_MINOR 0)
set(NNC_PATCH 1)

if(MSVC)
	add_compile_definitions(/D_CRT_SECURE_NO_WARNINGS)
	add_compile_definitions(/DNNC_VERSION_MAJOR=${NNC_MAJOR})
	add_compile_definitions(/DNNC_VERSION_MINOR=${NNC_MINOR})
	add_compile_definitions(/DNNC_VERSION_PATCH=${NNC_PATCH})
	
else()
	add_compile_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_compile_definitions(-DNNC_VERSION_MAJOR=${NNC_MAJOR})
	add_compile_definitions(-DNNC_VERSION_MINOR=${NNC_MINOR})
	add_compile_definitions(-DNNC_VERSION_PATCH=${NNC_PATCH})
endif()

#set output directory of the exe to the bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

file(
	GLOB_RECURSE NNC_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(NNC ${NNC_SOURCES})
target_include_directories(NNC PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_features(NNC PRIVATE cxx_std_23)
