#include <iostream>
#include <vector>

#include "Parser.h"
#include "Solver.h"

int main()
{
    Parser lParser;
    std::pair <int, std::vector<std::pair<int, int>>> lConfiguration = lParser.Parse();
    Solver lSolver(lConfiguration);

    int lSolution = lSolver.compute();
    // As our solver counts the number of nodes and we want the number of hops we substract 2, with a lower limit of 0 ofcourse.
    lSolution = std::max(0, lSolution - 2);
    std::cout << lSolution << std::endl;
    return 0;
}