cmake_minimum_required(VERSION 3.10)
project(MyChessEngine)

# Use C++17 (it has great features for engines)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake where to find your header files
include_directories(include)

# Tell CMake to create an executable from your source files
add_executable(ChessEngine src/main.cpp)