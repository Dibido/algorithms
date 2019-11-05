#include <vector>
#include <queue>
#include <cmath>

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
        int findLongestPath(Node* aNode);

    private:

        int mNumberOfNodes;
        std::vector<Node> mNodes;
        std::vector<Cluster> mClusters;
};