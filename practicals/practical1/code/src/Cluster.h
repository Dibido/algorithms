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
        std::vector<Node> getNodes() const;
        int getMaxLength() const;

    private:
        std::vector<Node> mNodes;
        int mMaxLength;
        Node mMiddleNode;
};