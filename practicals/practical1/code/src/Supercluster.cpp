#include <iostream>
#include <vector>

#include "Parser.h"
#include "Solver.h"

int main(int argc, char** argv)
{
    Parser lParser;
    std::pair <int, std::vector<std::pair<int, int>>> lConfiguration = lParser.Parse();
    Solver lSolver(lConfiguration);
    int lSolution = lSolver.compute();
    std::cout << lSolution << std::endl;
    return 0;
}