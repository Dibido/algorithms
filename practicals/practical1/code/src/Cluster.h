#include <vector>

#include "Node.h"

class Cluster
{
    public:
        Cluster();
        virtual ~Cluster();

        void addNode(Node aNode);

    private:
        std::vector<Node> mNodes;
};