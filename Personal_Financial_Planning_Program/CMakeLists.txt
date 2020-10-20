cmake_minimum_required(VERSION 3.15)
project(Assignment_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Assignment_2 main.cpp FinPlanningProgram.cpp FinPlanningProgram.hpp Client.cpp Client.hpp InputReader.cpp InputReader.hpp CSVInputReader.cpp CSVInputReader.hpp FixedWidthInputReader.cpp FixedWidthInputReader.hpp)