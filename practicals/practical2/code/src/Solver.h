#include <vector>
#include <string>
#include <cmath>

#include "Cluster.h"

class Solver
{
    public:
        Solver();
        virtual ~Solver();
        
        std::string compute();
        std::vector<Cluster> findClusters(std::vector<Node>& aNodes);

    private:
        int mNumberOfNodes;
        int mNumberOfMovies;
        std::vector<Node> mNodes;
        std::vector<Cluster> mClusters;
};