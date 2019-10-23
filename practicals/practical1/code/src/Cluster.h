#include <vector>

#include "Node.h"

class Cluster
{
    public:
        Cluster();
        virtual ~Cluster();

        void addNode(Node aNode);
        void setMaxLength(int aMaxLength);
        void setMiddleNode (Node aNode);
        Node& getMiddleNode();
        std::vector<Node> getNodes() const;
        int getMaxLength() const;

        bool operator==(Cluster& aCluster);

    private:
        std::vector<Node> mNodes;
        int mMaxLength;
        Node mMiddleNode;
};