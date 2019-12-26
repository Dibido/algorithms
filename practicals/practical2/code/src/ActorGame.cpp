
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "Solver.h"
//#include "Stopwatch.hpp"

int main()
{
    // auto start = std::chrono::high_resolution_clock::now();
    //namespace sw = stopwatch;

    //sw::Stopwatch lWatch;

    Solver lSolver;

    std::string lSolution = lSolver.compute();

    std::cout << lSolution << std::endl;
    return 0;
}