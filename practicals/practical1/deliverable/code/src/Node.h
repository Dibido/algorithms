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
        const std::vector <Node*>& getNeighbours() const;
        void removeNeighbour(Node* aNode);

        int getId() const;

        bool operator==(const Node& aNode) const;

        Node(const Node& aNode);

        Node& operator=(const Node& aNode);

    private:

        int mId;
        std::vector<Node*> mNeighbours;
};