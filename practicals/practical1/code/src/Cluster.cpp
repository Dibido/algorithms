#include "Cluster.h"

Cluster::Cluster() : mLongestPathSize(-1)
{
}

Cluster::~Cluster()
{
}

void Cluster::addNode(Node* aNode)
{
    mNodes.push_back(aNode);
}

void Cluster::setMiddleNode(Node aNode)
{
    mMiddleNode = aNode;
}

void Cluster::setLongestPathSize (const int aLongestPath)
{
    mLongestPathSize = aLongestPath;
}

Node& Cluster::getMiddleNode()
{
    return mMiddleNode;
}

const std::vector<Node*>& Cluster::getNodes() const
{
    return mNodes;
}

bool Cluster::operator==(const Cluster& aCluster) const
{
    return mNodes == aCluster.getNodes();
}

Cluster::Cluster(const Cluster& aCluster)
{
  mLongestPathSize = aCluster.mLongestPathSize;
  mMiddleNode = aCluster.mMiddleNode;

  for(auto& lNode : aCluster.mNodes)
  {
    mNodes.push_back(lNode);
  }
}


int Cluster::getLongestPathSize() const
{
  return mLongestPathSize;
}

int Cluster::getNumberOfNodes() const
{
  return mNodes.size();
}

Node* Cluster::getFirstNode()
{
  if(mNodes.size() == 0)
  {
    throw std::logic_error("mNodes has no sizes, can't get first node");
  }

  return mNodes.at(0);
}