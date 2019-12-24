
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "Solver.h"

int main()
{
    // auto start = std::chrono::high_resolution_clock::now();

    Solver lSolver;
    
    // auto end = std::chrono::high_resolution_clock::now(); 
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << duration.count() << " millis to read input" << std::endl;
    

    std::string lSolution = lSolver.compute();
   
    std::cout << lSolution << std::endl;
    return 0;
}