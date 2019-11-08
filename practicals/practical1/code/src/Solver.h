#include <vector>
#include <queue>
#include <cmath>

#include "Cluster.h"

class Solver
{
    public:
        Solver();
        virtual ~Solver();
        
        /**
         * 
         */
        int compute();

        /**
         *
         */
        std::vector<Cluster> findClusters(std::vector<Node>& aNodes);
        
        /**
         *
         */
        Cluster findLongestCluster(std::vector<Cluster>& aClusters);

        /**
         * 
         */
        int getClusterLength(Cluster& aCluster);

        /**
         * 
         */
        int findLongestPath(Node* aNode, int aClusterSize);

    private:

        int mNumberOfNodes;
        std::vector<Node> mNodes;
        std::vector<Cluster> mClusters;
};