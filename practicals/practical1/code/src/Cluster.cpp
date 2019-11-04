#include "Cluster.h"

Cluster::Cluster() : mLongestPathSize(-1)
{
}

Cluster::~Cluster()
{
}

void Cluster::addNode(Node& aNode)
{
    printf("ADDING node(%i, %p) \n", aNode.getId(), &aNode);
    mNodes.push_back(aNode);
    printf("After add, in mNodes its node(%i, %p) \n", mNodes.at(mNodes.size()-1).getId(), &(mNodes.at(mNodes.size()-1)));
    
    for(auto& lNeighbour : mNodes.at(mNodes.size()-1).getNeighbours())
    {
      printf("lNEIGH: %i , %p \n", lNeighbour->getId(), (void*)lNeighbour);
    }
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

void Cluster::setLongestPathSize (const int aLongestPath)
{
    mLongestPathSize = aLongestPath;
}

Node& Cluster::getMiddleNode()
{
    return mMiddleNode;
}

const std::vector<Node>& Cluster::getNodes() const
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
  std::cout << "Copy constructor CLUSTER used" << std::endl;
}


int Cluster::getLongestPathSize() const
{
  return mLongestPathSize;
}

int Cluster::getNumberOfNodes() const
{
  return mNodes.size();
}

Node& Cluster::getFirstNode()
{
  if(mNodes.size() == 0)
  {
    throw std::logic_error("mNodes has no sizes, can't get first node");
  }

  return mNodes.at(0);
}