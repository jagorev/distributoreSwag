cmake_minimum_required(VERSION 3.5)
 
project(servomotor)
 
include_directories(include)
 
file(GLOB SOURCES "source/*.cpp")

add_library(servomotor ${SOURCES})

