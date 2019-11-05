#include <iostream>
#include <vector>

#include "Parser.h"
#include "Solver.h"

int main(int argc, char** argv)
{
    Parser lParser;
    std::pair <int, std::vector<std::pair<int, int>>> lConfiguration = lParser.Parse();
    std::cout << "Parsed " << std::endl;
    Solver lSolver(lConfiguration);
    std::cout << "Initiated lSolver" << std::endl;
    int lSolution = lSolver.compute();
    std::cout << "Compute returns: " << lSolution << std::endl;
    // As our solver counts the number of nodes and we want the number of hops we do (-2)
    // When we have only one node we want to limit it to 0.
    lSolution = std::max(0, lSolution - 2);
    std::cout << "Solution : " << lSolution << std::endl;
    return 0;
}