#include <vector>

#include "Node.h"

class Cluster
{
    public:
        Cluster();
        virtual ~Cluster();

        void addNode(Node& aNode);
        void setMaxLength(int aMaxLength);
        void setMiddleNode (Node aNode);
        void setLongestPath (const int aLongestPath);
        Node& getMiddleNode();
        const std::vector<Node>& getNodes() const;
        int getMaxLength() const;
        int getLongestPath() const;

        bool operator==(const Cluster& aCluster) const;

    private:
        // All the nodes in the cluster.
        std::vector<Node> mNodes;
        // The longest path in the cluster.
        int mLongestPath;
        // The length of the longest path.
        int mMaxLength;
        // The middle node of the longest path.
        Node mMiddleNode;
};