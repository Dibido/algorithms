#include "Solver.h"
#include <tuple>
#include <chrono>
#include <thread>
#include <map>

Solver::Solver(std::pair <int, std::vector<std::pair<int, int>>> aNodeList)
{
    mNumberOfNodes = aNodeList.first;

    // Add nodes
    for(int i = 0; i < mNumberOfNodes; i++)
    {
        mNodes.push_back(Node(i));
    }
    // Add connections
    for(auto& aPair : aNodeList.second)
    {
        mNodes.at(aPair.first).addNeighbour(&mNodes.at(aPair.second));
        mNodes.at(aPair.second).addNeighbour(&mNodes.at(aPair.first));
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
    for (int i = 0; i < lNodeList.size(); i++)
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

            lClusters.push_back(lCluster);
        }
    }
    int j = 0;
    for(auto& lC : lClusters)
    {
      std::cout << "nodeCount: " << lC.getNodes().size() << std::endl;
      if(lC.getNodes().size() == 2)
      {
        j++;
      }
    }
    std::cout << "J: " << j <<std::endl;
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
      if(lCluster.getNodes().size() != getClusterLength(lCluster))
      {
        std::cout << "lCluster with node " << lCluster.getFirstNode()->getId() << " has a total of " << lCluster.getNodes().size() << " nodes, and longest path: " << getClusterLength(lCluster) << std::endl;
      }
        if (getClusterLength(lCluster) > lLongestCluster.getLongestPathSize())
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

    lClusterLength = findLongestPath(lFirstNode, aCluster.getNumberOfNodes());  
    
    aCluster.setLongestPathSize(lClusterLength);
    
    return lClusterLength;
}

int Solver::findLongestPath(Node* aNode, int aClusterSize)
{
  if(aNode == nullptr)
  {
    throw std::logic_error("aNode is a nullptr");
  }

  // Key will be node id, distance distance from aNode.
  std::map<Node*, int> lDistances;
  lDistances.insert(std::make_pair(aNode, 1));

  std::queue<Node*> lQueue;
  lQueue.push(aNode);

  while(!lQueue.empty())
  {
    Node* lNode = lQueue.front();
    lQueue.pop();

    for(auto& lNeighbour : lNode->getNeighbours())
    {
      // Node not processed earlier
      auto lIt = lDistances.find(lNeighbour);
      if(lIt == lDistances.end())
      {
        lDistances.insert(std::make_pair(lNeighbour, lDistances[lNode] + 1));
        lQueue.push(lNeighbour);
      }
    }
  }

  int lHighestDistance = -1;
  Node* lFurthestNode = nullptr;

  for(auto lIt = lDistances.begin(); lIt != lDistances.end(); lIt++)
  {
    if(lIt->second > lHighestDistance)
    {
      lHighestDistance = lIt->second;
      lFurthestNode = lIt->first;
    }
  }
  
  lDistances.clear();

  lDistances.insert(std::make_pair(lFurthestNode, 1));
  lQueue.push(lFurthestNode);

  while(!lQueue.empty())
  {
    Node* lNode = lQueue.front();
    lQueue.pop();

    for(auto& lNeighbour : lNode->getNeighbours())
    {
      // Node not processed earlier
      auto lIt = lDistances.find(lNeighbour);
      //auto lIt = std::find_if(lDistances.begin(), lDistances.end(), [&lNeighbour](const auto& lElem) {return lNeighbour->getId() == lElem.first->getId();});
      if(lIt == lDistances.end())
      {
        lDistances.insert(std::make_pair(lNeighbour, lDistances[lNode] + 1));

        lQueue.push(lNeighbour);
      }
    }
  }

  lHighestDistance = 0;

  Node* lNode = nullptr;
  for(auto lIt = lDistances.begin(); lIt != lDistances.end(); lIt++)
  {
    if(lIt->second > lHighestDistance)
    {
      lHighestDistance = lIt->second;
      lNode = lIt->first;
    }
  }

  return lHighestDistance;
}
