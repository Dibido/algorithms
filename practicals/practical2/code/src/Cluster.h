#include <vector>
#include <set>

#include "Node.h"

class Cluster
{
    public:
        Cluster();
        virtual ~Cluster();

        void renumber();
        void addNode(Node* aNode);
        void setMiddleNode (Node aNode);
        void setLongestPathSize (const int aLongestPath);
        Node& getMiddleNode();
        const std::vector<Node*>& getNodes() const;
        int getLongestPathSize() const;
        int getNumberOfNodes() const;
        Node* getFirstNode();
        

        bool hasPerfectMatching();
        bool isBalanced() const;
        void setBalanced (bool aBalanced);
        bool tryAugmentingPath();

        bool operator==(const Cluster& aCluster) const;

        Cluster(const Cluster& aCluster);

    private:
        // All the nodes in the cluster.
        std::vector<Node*> mNodes;

        // The found matching in the graph
        std::set<Node*> mMatching;

        // A cluster is balanced if it has as many males as females
        bool mIsBalanced;
        // The longest path in the cluster.
        int mLongestPathSize;
        // The middle node of the longest path.
        Node mMiddleNode;
};