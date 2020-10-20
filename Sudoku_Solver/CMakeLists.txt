cmake_minimum_required(VERSION 3.12)
project(Sudoku_Solver)

set(CMAKE_CXX_STANDARD 14)

add_executable(Sudoku_Solver main.cpp Backtracking.cpp Backtracking.hpp)