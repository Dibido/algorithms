#include "Node.h"

Node::Node() : mId(0)
{
}

Node::Node(int aNodeId, std::string aActorName, bool aIsMale) : mId(aNodeId), mActorName(aActorName), mIsMale(aIsMale), mHasOppositeGenderNeighbour(false)
{
}

Node::Node(const Node& aNode)
{
    mId = aNode.getId();
    mActorName = aNode.getActorName();
    mIsMale = aNode.isMale();
    mHasOppositeGenderNeighbour = aNode.hasOppositeGenderNeighbour();
    for(auto& lNeighbour : aNode.getNeighbours())
    {
        mNeighbours.push_back(lNeighbour);
    }
}

 Node& Node::operator=( const Node& aNode ) {
    mId = aNode.getId();
    mActorName = aNode.getActorName();
    mIsMale = aNode.isMale();
    mHasOppositeGenderNeighbour = aNode.hasOppositeGenderNeighbour();
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

    for(unsigned int i = 0; i < mNeighbours.size(); i++)
    {
        std::cout << "\t" << mId << " - " << mNeighbours.at(i)->mId << std::endl;
    }
}

void Node::addNeighbour(Node* aNode)
{
    if(!mHasOppositeGenderNeighbour)
    {
      if(aNode->isMale() != isMale()) // Check if it is opposite gender
      {
          mHasOppositeGenderNeighbour = true;
      }
    }
    mNeighbours.push_back(aNode);
}

void Node::setOppositeNeighbour()
{
    mHasOppositeGenderNeighbour = true;
}

const std::vector <Node*>& Node::getNeighbours() const
{
    return mNeighbours;
}

void Node::removeNeighbour(Node* aNode)
{
    for(unsigned int i = 0; i < mNeighbours.size(); i++)
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

std::string Node::getActorName() const
{
    return mActorName;
}

bool Node::hasOppositeGenderNeighbour() const
{
    return mHasOppositeGenderNeighbour;
}

bool Node::isMale() const
{
    return mIsMale;
}

bool Node::isFemale() const
{
    return mIsMale == false;
}