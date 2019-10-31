#include <vector>

#include "Node.h"

class Cluster
{
    public:
        Cluster();
        virtual ~Cluster();

        void addNode(Node& aNode);
        void setMiddleNode (Node aNode);
        void setLongestPathSize (const int aLongestPath);
        Node& getMiddleNode();
        const std::vector<Node>& getNodes() const;
        int getLongestPathSize() const;
        int getNumberOfNodes() const;
        Node getFirstNode() const;

        bool operator==(const Cluster& aCluster) const;

    private:
        // All the nodes in the cluster.
        std::vector<Node> mNodes;
        // The longest path in the cluster.
        int mLongestPathSize;
        // The middle node of the longest path.
        Node mMiddleNode;
};