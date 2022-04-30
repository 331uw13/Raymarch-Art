cmake_minimum_required(VERSION 3.18.2)
project(RaymarchArtRenderer C)

#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wunused-function -lglfw -lGLEW -lGL -lz -lm")
file(GLOB_RECURSE SOURCE_FILES "./src/*.c" "./src/views/*.c")

add_executable(
	art_renderer
	${SOURCE_FILES}
)
