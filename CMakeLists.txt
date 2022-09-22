cmake_minimum_required(VERSION 3.23)
project(qtree_self_implementation C)

set(CMAKE_C_STANDARD 99)

add_executable(quadtree-in-c main.c read.c qtree.c)
