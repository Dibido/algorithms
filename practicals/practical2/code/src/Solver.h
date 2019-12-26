#include <vector>
#include <string>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <map>

#include "Cluster.h"
//#include "Stopwatch.hpp"

class Solver
{
    public:
        Solver();
        virtual ~Solver();
        
        std::string compute();
        
        // Will use mNodePointers to find the clusters.
        std::vector<Cluster> findClusters();

    private:
        int mNumberOfNodes;
        int mNumberOfMovies;
        
        // Should not be changed in any way after constructor, or pointers in mNodePointers would get invalid.
        std::unordered_map<std::string, Node> mNodes;

        // Pointers will point to the nodes in mNodes after constructor, more convenient to iterate over.
        std::vector<Node*> mNodePointers;

        // A list of all the clusters
        std::vector<Cluster> mClusters;
};