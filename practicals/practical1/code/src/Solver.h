#include <vector>
#include <queue>

#include "Cluster.h"

class Solver
{
    public:
        Solver(std::pair <int, std::vector<std::pair<int, int>>> aNodeList);
        virtual ~Solver();
        
        /**
         * 
         */
        int compute();

        /**
         *
         */
        std::vector<Cluster> findClusters(std::vector<Node> aNodes);

    private:
        int mNumberOfNodes;
        std::vector<Node> mNodes;
        std::vector<Cluster> mClusters;
};