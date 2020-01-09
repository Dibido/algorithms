#include "Cluster.h"
#include "HopcroftKarp.h"

Cluster::Cluster() : mIsBalanced(true), mLongestPathSize(-1)
{
}

Cluster::~Cluster()
{
}

bool Cluster::hasPerfectMatching()
{
  // Uneven number of nodes -> perfect matching never possible
  if(mNodes.size() % 2 == 1)
  {
    return false;
  }

  // Transforming our current data structures/node numbering so that it fits the structure our hopcroftkarp expects:

  int lNumberOfMales = 0;
  int lNumberOfFemales = 0;

  int lMaleId = 1;
  int lFemaleId = 1;

  for(auto& lNode : mNodes)
  {
    if(lNode->isMale())
    {
      lNode->setNodeId(lMaleId);
      lMaleId++;
      lNumberOfMales++;
    }
    else
    {
      lNode->setNodeId(lFemaleId);
      lFemaleId++;
      lNumberOfFemales++;
    }
  }

  // No maximal matching if unequal number of males/females
  if(lNumberOfFemales != lNumberOfMales)
  {
    return false;
  }
  
  HopcroftKarp lHopcroftKarp(lNumberOfMales, lNumberOfFemales);

  // We will consider the males on left side of hopcroft karp. Adding neighbours for the males:
  for(auto& lNode : mNodes)
  {
    if(lNode->isMale())
    {
      for(auto& lNeighbour : lNode->getNeighbours())
      {
        lHopcroftKarp.addEdge(lNode->getId(), lNeighbour->getId());
      }
    }
  }
  
  // Each male (and female) should be matched if perfect matching exists.
  return (lHopcroftKarp.computeMaximumMatching() == lNumberOfMales);
}

void Cluster::renumber()
{
  for (unsigned int i = 0; i < mNodes.size(); i++)
  {
    mNodes.at(i)->setNodeId(i);
  }
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
  mIsBalanced = aCluster.mIsBalanced;

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

void Cluster::setBalanced(bool aBalanced)
{
  mIsBalanced = aBalanced;
}

bool Cluster::isBalanced() const
{
  return mIsBalanced;
}