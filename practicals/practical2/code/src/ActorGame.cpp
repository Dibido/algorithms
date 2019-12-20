
#include <iostream>
#include <vector>
#include <string>

#include "Solver.h"

int main()
{
    Solver lSolver;

    std::string lSolution = lSolver.compute();
   
    std::cout << lSolution << std::endl;
    return 0;
}