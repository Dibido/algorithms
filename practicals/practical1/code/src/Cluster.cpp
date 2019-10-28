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

void Cluster::setLongestPath (const std::vector<Node>& aLongestPath)
{
    mLongestPath = aLongestPath;
}

Node& Cluster::getMiddleNode()
{
    return mMiddleNode;
}

std::vector<Node>& Cluster::getNodes()
{
    return mNodes;
}

int Cluster::getMaxLength() const
{
    return mMaxLength;
}

bool Cluster::operator==(const Cluster& aCluster)
{
    return mMiddleNode == aCluster.mMiddleNode;
}

std::vector<Node> Cluster::getLongestPath() const
{
  return mLongestPath;
}