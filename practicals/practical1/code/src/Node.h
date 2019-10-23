#include <vector>
#include <iostream>
#include <algorithm>

class Node
{
    public:
        Node();
        Node (int aNodeId);
        virtual ~Node();

        void setNodeId(const int aNodeid);
        
        void printNode();
        void addNeighbour(Node* aNode);
        std::vector <Node*> getNeighbours();
        void removeNeighbour(Node* aNode);

        int getId() const;

        const bool operator==(const Node& aNode);

    private:

        int mId;
        std::vector<Node*> mNeighbours;
};