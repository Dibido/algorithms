#include "Cluster.h"

Cluster::Cluster() : mMatching(),  mIsBalanced(true), mLongestPathSize(-1)
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
  else
  {
    // Try to find perfect matching
    while (tryAugmentingPath()) // Find an augmenting path
    {
    }
    // No more augmenting path
    return (mMatching.size() == mNodes.size()); // Whether we have found a perfect matching
  }
}

bool Cluster::tryAugmentingPath()
{
  // Try to find an augmenting path
  std::vector<Node*> lMen;
  std::vector<Node*> lWomen;

  for(auto& lNode : mNodes)
  {
    if(lNode->isMale())
    {
      lMen.push_back(lNode);
    }
    else
    {
      lWomen.push_back(lNode);
    }
  }

  for(auto& lWoman : lWomen)
  {
    std::vector<Node*> lTree;
    if(mMatching.find(lWoman) != mMatching.end()) // Check if the node is in the matching
    {
      // Add the nodes to the tree
      lTree.push_back(lWoman);
      // Add the neighbours to the tree
    }
  }

  return false;
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

bool Cluster::isBalanced() const
{
  return mIsBalanced;
}

void Cluster::setBalanced (bool aBalanced)
{
  mIsBalanced = aBalanced;
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