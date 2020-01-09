#include "Node.h"
#include <algorithm>

Node::Node() : mId(0)
{
}

Node::Node(int aNodeId, std::string aActorName, bool aIsMale) : mId(aNodeId), mActorName(aActorName), mIsMale(aIsMale)
{
}

Node::Node(const Node& aNode)
{
    mId = aNode.getId();
    mActorName = aNode.getActorName();
    mIsMale = aNode.isMale();
    for(auto& lNeighbour : aNode.getNeighbours())
    {
        mNeighbours.push_back(lNeighbour);
    }

    mNeighbourIds = aNode.mNeighbourIds;
}

 Node& Node::operator=( const Node& aNode ) {
    mId = aNode.getId();
    mActorName = aNode.getActorName();
    mIsMale = aNode.isMale();
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

void Node::setActorName(std::string aActorName)
{
    mActorName = aActorName;
}

Node::~Node()
{
}

bool Node::operator==(const Node& aNode) const
{
    return mId == aNode.getId();
}

void Node::printNode() const
{
    std::string lSex;
    if(mIsMale)
    {
        lSex = "M";
    }
    else
    {
        lSex = "F";
    }
    
    std::cout << "Id : " << mId << std::endl;
    std::cout << "Name : " << mActorName << " [" << lSex << "]" << std::endl;
}

void Node::addNeighbour(Node* aNode)
{
    //If it has not yet been inserted, add the neighbour (also add this to aNode as a neighbour)
    if(mNeighbourIds.find(aNode->getId()) == mNeighbourIds.end())
    {
        mNeighbourIds.insert(aNode->getId());
        aNode->mNeighbourIds.insert(aNode->getId());

        mNeighbours.push_back(aNode);
        aNode->mNeighbours.push_back(this);
    }
}

const std::vector <Node*>& Node::getNeighbours() const
{
    return mNeighbours;
}

int Node::getId() const
{
    return mId;
}

std::string Node::getActorName() const
{
    return mActorName;
}

bool Node::isMale() const
{
    return mIsMale;
}

bool Node::isFemale() const
{
    return mIsMale == false;
}