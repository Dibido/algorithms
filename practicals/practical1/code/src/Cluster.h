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
        void setLongestPath (const std::vector<Node>& aLongestPath);
        Node& getMiddleNode();
        std::vector<Node>& getNodes();
        int getMaxLength() const;
        std::vector<Node> getLongestPath() const;

        bool operator==(const Cluster& aCluster);

    private:
        // All the nodes in the cluster.
        std::vector<Node> mNodes;
        // The longest path in the cluster.
        std::vector<Node> mLongestPath;
        // The length of the longest path.
        int mMaxLength;
        // The middle node of the longest path.
        Node mMiddleNode;
};