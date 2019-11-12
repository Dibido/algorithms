#include <iostream>
#include <vector>

#include "Solver.h"

int main()
{
    Solver lSolver;

    int lSolution = lSolver.compute();
 
    // As our solver counts the number of nodes and we want the number of hops we substract 2, with a lower limit of 0 ofcourse.
    lSolution = std::max(0, lSolution - 2);
    std::cout << lSolution << std::endl;
    return 0;
}