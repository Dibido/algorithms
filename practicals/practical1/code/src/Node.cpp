#include "Node.h"

Node::Node() : mId(0)
{
}

Node::Node(int aNodeId) : mId(aNodeId)
{
}

Node::Node(const Node& aNode)
{
    mId = aNode.getId();
    for(auto& lNeighbour : aNode.getNeighbours())
    {
        mNeighbours.push_back(lNeighbour);
    }
}

 Node& Node::operator=( const Node& aNode ) {
    mId = aNode.getId();
    mNeighbours.clear();
    for(auto& lNeighbour : aNode.getNeighbours())
    {
        mNeighbours.push_back(lNeighbour);
    }
    return *this;
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
    // Does not exist yet.
    if(std::find (mNeighbours.begin(), mNeighbours.end(), aNode) == mNeighbours.end())
    {
        mNeighbours.push_back(aNode);
    }
}

std::vector <Node*> Node::getNeighbours() const
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