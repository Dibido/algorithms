#include "Node.h"

Node::Node() : mId(0)
{
}

Node::Node(int aNodeId) : mId(aNodeId)
{
}

void Node::setNodeId(const int aNodeId)
{
    mId = aNodeId;
}

Node::~Node()
{
}

void Node::printNode()
{
    std::cout << "Id : " << mId << std::endl;
    for(int i = 0; i < mNeighbours.size(); i++)
    {
        std::cout << "\t" << mId << " - " << mNeighbours.at(i)->mId << std::endl;
    }
}

void Node::addNeighbour(Node* aNode)
{
    mNeighbours.push_back(aNode);
}

std::vector <Node*> Node::getNeighbours()
{
    return mNeighbours;
}

int Node::getId() const
{
    return mId;
}