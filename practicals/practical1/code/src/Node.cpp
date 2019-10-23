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

const bool Node::operator==(const Node& aNode)
{
    return mId == aNode.getId();
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

void Node::removeNeighbour(Node* aNode)
{
    for(int i = 0; i < mNeighbours.size(); i++)
    {
        if(mNeighbours.at(i)->getId() == aNode->getId())
        {
            mNeighbours.erase(mNeighbours.begin()+i);
        }
    } 
}

int Node::getId() const
{
    return mId;
}