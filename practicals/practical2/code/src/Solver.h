#include <vector>
#include <string>
#include <cmath>
#include <queue>
#include <unordered_map>

#include "Cluster.h"

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

        // std::vector<std::pair<std::string, Node>> mNodes;

        // Pointers will point to the nodes in mNodes after constructor, more convenient to iterate over.
        std::vector<Node*> mNodePointers;




        std::vector<Cluster> mClusters;
};