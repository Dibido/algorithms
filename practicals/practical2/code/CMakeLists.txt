cmake_minimum_required (VERSION 2.6)
project (ActorGame)

#if(NOT CMAKE_BUILD_TYPE)
#  set(CMAKE_BUILD_TYPE Release)
#endif()

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(ActorGame src/ActorGame.cpp src/Solver.cpp src/Node.cpp src/Cluster.cpp src/HopcroftKarp.cpp)
