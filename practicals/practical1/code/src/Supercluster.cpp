#include <iostream>
#include <vector>

#include "Parser.h"
#include "Solver.h"
#include "Stopwatch.h"

int main()
{
    system_stopwatch stopwatch;
    Parser lParser;
    std::pair <int, std::vector<std::pair<int, int>>> lConfiguration = lParser.Parse();
 
    // Times are in ms.
    unsigned int lTime1 = stopwatch.elapsed_time<unsigned int, std::chrono::microseconds>();
    std::cout << "After parsing time used: " << lTime1 << std::endl;

    Solver lSolver(lConfiguration);

    unsigned int lTime2 = stopwatch.elapsed_time<unsigned int, std::chrono::microseconds>();
    std::cout << "After constructing lSolver(lConfiguration) time used: " << lTime2 << std::endl;

    int lSolution = lSolver.compute();
  
    unsigned int lTime3 = stopwatch.elapsed_time<unsigned int, std::chrono::microseconds>();
    std::cout << "After compute time used: " << lTime3 << std::endl;

    // As our solver counts the number of nodes and we want the number of hops we substract 2, with a lower limit of 0 ofcourse.
    lSolution = std::max(0, lSolution - 2);
    std::cout << lSolution << std::endl;
    return 0;
}