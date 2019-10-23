#include <vector>
#include <iostream>

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

        int getId() const;

            
    private:

        int mId;
        std::vector<Node*> mNeighbours;
};