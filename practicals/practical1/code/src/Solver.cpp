#include "Solver.h"
#include <tuple>
#include <chrono>
#include <thread>
#include <map>
#include <set>
#include <iostream>
#include <fstream>

Solver::Solver()
{
    auto& input = std::cin;

    int p, l;
    input >> p >> l;

    mNumberOfNodes = p;

    for(int i = 0; i < mNumberOfNodes; i++)
    {
        mNodes.push_back(Node(i));
    }

    for (int i = 0; i < l; i++)
    {
        int a, b;
        input >> a >> b;

        mNodes.at(a).addNeighbour(&mNodes.at(b));
        mNodes.at(b).addNeighbour(&mNodes.at(a));
    }
}

std::vector<Cluster> Solver::findClusters(std::vector<Node>& aNodes)
{
    std::vector<Cluster> lClusters;
    std::vector<std::pair<int,bool>> lNodeList; //used for finding the clusters, int for the id and bool for if it has been added to a cluster

    // Put all the nodes in the nodelist
    for(auto& lNode : aNodes)
    {
        lNodeList.push_back(std::pair<int, bool>(lNode.getId(), false)); // By default not in a cluster
    }

    // Check the neighbours for every node and add them to a cluster
    for (unsigned int i = 0; i < lNodeList.size(); i++)
    {
        // If not yet in cluster
        if(!lNodeList.at(i).second)
        {
            Cluster lCluster;
            // Add the neighbours
            std::queue<Node*> lNodeQueue;
            lNodeQueue.push(&aNodes.at(i));

            while(!lNodeQueue.empty())
            {
                Node* lCurrentNode = lNodeQueue.front();
                lNodeQueue.pop();
                // Mark the node as handled
                lNodeList.at(lCurrentNode->getId()).second = true;
                lCluster.addNode(lCurrentNode);

                for(auto lNeighbour : lCurrentNode->getNeighbours())
                {
                    // Node has not been seen yet
                    if(!lNodeList.at(lNeighbour->getId()).second)
                    {
                        lNodeQueue.push(lNeighbour);
                    }
                }
            }
            
            lCluster.renumber();

            lClusters.push_back(lCluster);
        }
    }
    return lClusters;
}

Solver::~Solver()
{
}

int Solver::compute()
{
    // Find clusters
    mClusters = findClusters(mNodes);

    // Find the longest cluster
    Cluster lLongestCluster = findLongestCluster(mClusters);

    // Remove the longest cluster
    mClusters.erase(std::remove(mClusters.begin(), mClusters.end(), lLongestCluster), mClusters.end());

    int lLongestPathSize = lLongestCluster.getLongestPathSize();

    //Evenly sized longest path
    if(lLongestPathSize % 2 == 0)
    {
        int lNumberOfHits = 0;
        for(auto& lCluster : mClusters)
        {
            // If there is a cluster of equal size
            if(lCluster.getLongestPathSize() == lLongestPathSize)
            {
                lNumberOfHits++;
            }
            // If there are 2 (or more) of equal size it will increase by 3.
            if(lNumberOfHits == 2)
            {
                return lLongestPathSize + 3;
            }
        }
        // If there is only one of equal size than it will increase by 2.
        if(lNumberOfHits == 1)
        {
            return lLongestPathSize + 2;
        }
        // If there is no cluster of equal size
        else
        {
            for(auto& lCluster : mClusters)
            {
                if((lLongestPathSize - lCluster.getLongestPathSize()) <= 2 )
                {
                    return lLongestPathSize + 1;
                }
            }
            return lLongestPathSize; 
        }
    }
    // Unevenly sized longest path
    else
    {
        int lNumberOfHits = 0;
    
        for(auto& lCluster : mClusters)
        {
            if((lLongestPathSize - lCluster.getLongestPathSize()) < 2)
            {
                lNumberOfHits++;
            }
            if(lNumberOfHits == 2)
            {
                return lLongestPathSize + 2;
            }
        }
        return lLongestPathSize + (lNumberOfHits * 1);
    }
}


Cluster Solver::findLongestCluster(std::vector<Cluster>& aClusters)
{
    if(static_cast<int>(aClusters.size()) == 0)
    {
      throw std::out_of_range("aClusters in findLongestCluster is of invalid size 0");
    }

    Cluster lLongestCluster = aClusters.at(0);

    for(Cluster& lCluster : aClusters)
    {
      int lClusterLength = getClusterLength(lCluster);
     
      if (lClusterLength > lLongestCluster.getLongestPathSize())
      {
          lLongestCluster = lCluster;
      }
    }

    return lLongestCluster;
}

int Solver::getClusterLength(Cluster& aCluster)
{
    int lClusterLength = 0;

    Node* lFirstNode = aCluster.getFirstNode();

    lClusterLength = findLongestPath(lFirstNode, aCluster.getNodes().size());

    aCluster.setLongestPathSize(lClusterLength);
    
    return lClusterLength;
}

int Solver::findLongestPath(Node* aNode, int aClusterSize)
{ 
  if(aNode == nullptr)
  {
    throw std::logic_error("aNode is a nullptr");
  }
  
  Node* lFurthestNode = nullptr;

  std::queue<Node*> lQueue;

  bool lSeen[aClusterSize] {false};
  lSeen[aNode->getId()] = true;

  lQueue.push(aNode);

  while(!lQueue.empty())
  {
      Node* lNode = lQueue.front();
      lQueue.pop();

      for(auto& lNeighbour : lNode->getNeighbours())
      {
        // Node not processed earlier
        if(!lSeen[lNeighbour->getId()])
        {
            lQueue.push(lNeighbour);
            lSeen[lNeighbour->getId()] = true;
        }
      }

      // If this is the last node processed with BFS, it is per definition a node with max distance to aNode.
      if(lQueue.empty())
      {
        lFurthestNode = lNode;
      }
  }

  int lDistances [aClusterSize] = {0};

  lDistances[lFurthestNode->getId()] = 1;
  lQueue.push(lFurthestNode);

  // Will contain pointer to the end node of the longest path, starting from lFurthestNode
  Node* lEndNode = nullptr;
  while(!lQueue.empty())
  {
    Node* lNode = lQueue.front();
    lQueue.pop();

    for(auto& lNeighbour : lNode->getNeighbours())
    {
      // Node not processed earlier
      if(lDistances[lNeighbour->getId()] == 0)
      {
        lDistances[lNeighbour->getId()] = lDistances[lNode->getId()] + 1;
        lQueue.push(lNeighbour);
      }
    }

    if(lQueue.empty())
    {
      lEndNode = lNode;
    }
  }

  return lDistances[lEndNode->getId()];
}