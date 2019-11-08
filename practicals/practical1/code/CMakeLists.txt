cmake_minimum_required (VERSION 2.6)
project (Supercluster)

#if(NOT CMAKE_BUILD_TYPE)
#  set(CMAKE_BUILD_TYPE Release)
#endif()

#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
#set(CMAKE_CXX_FLAGS_DEBUG "-g")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(Supercluster src/Supercluster.cpp src/Parser.cpp src/Node.cpp src/Solver.cpp src/Cluster.cpp src/Stopwatch.h)
