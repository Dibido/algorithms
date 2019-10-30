#include "Cluster.h"

Cluster::Cluster() : mMaxLength(-1)
{
}

Cluster::~Cluster()
{
}

void Cluster::addNode(Node& aNode)
{
    mNodes.push_back(aNode);
}

void Cluster::setMaxLength(int aMaxLength)
{
    mMaxLength = aMaxLength;    
}

void Cluster::setMiddleNode(Node aNode)
{
    mMiddleNode = aNode;

    for(auto& lNode : mNodes)
    {
        if(lNode.getId() == aNode.getId())
        {
            lNode = aNode;
        }
    }
}

void Cluster::setLongestPath (const int aLongestPath)
{
    mLongestPath = aLongestPath;
}

Node& Cluster::getMiddleNode()
{
    return mMiddleNode;
}

const std::vector<Node>& Cluster::getNodes() const
{
    return mNodes;
}

int Cluster::getMaxLength() const
{
    return mMaxLength;
}

bool Cluster::operator==(const Cluster& aCluster) const
{
    return mNodes == aCluster.getNodes();
}

int Cluster::getLongestPath() const
{
  return mLongestPath;
}