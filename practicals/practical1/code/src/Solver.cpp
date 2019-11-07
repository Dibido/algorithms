#include "Solver.h"
#include <tuple>
#include <chrono>
#include <thread>
#include <map>
#include "Stopwatch.h"

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
    system_stopwatch lComputeWatch;
    std::cout << "Begin compute, time = 0" << std::endl;
    // Find clusters
    mClusters = findClusters(mNodes);

    unsigned int lTime1 = lComputeWatch.elapsed_time<unsigned int, std::chrono::microseconds>();
    std::cout << "After findclusters, MS since start compute: " << lTime1 << std::endl;

    // Find the longest cluster
    Cluster lLongestCluster = findLongestCluster(mClusters);

    unsigned int lTime2 = lComputeWatch.elapsed_time<unsigned int, std::chrono::microseconds>();
    std::cout << "After findLongestCluster(mClusters), MS since start compute: " << lTime2 << std::endl;
    
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
    
    system_stopwatch lFindLongestStopwatch;
    unsigned int lTime1 = lFindLongestStopwatch.elapsed_time<unsigned int, std::chrono::microseconds>();
    std::cout << "After parsing time used: " << lTime1 << std::endl;

    Cluster lLongestCluster = aClusters.at(0);

    std::cout << "Starting loop through clusters in findLongestCluster, time = 0" << std::endl;

    for(Cluster& lCluster : aClusters)
    {
      int lClusterLength = getClusterLength(lCluster);

      lTime1 = lFindLongestStopwatch.elapsed_time<unsigned int, std::chrono::microseconds>();
      std::cout << "Found clusterlength, time expired: " << lTime1 << std::endl;

        if (lClusterLength > lLongestCluster.getLongestPathSize())
        {
            lLongestCluster = lCluster;
        }
    }

    std::cout << "Done looping through clusters in findLongestCluster" << std::endl;
    return lLongestCluster;
}

int Solver::getClusterLength(Cluster& aCluster)
{
    int lClusterLength = 0;

    Node* lFirstNode = aCluster.getFirstNode();

    lClusterLength = findLongestPath(lFirstNode);  
    
    aCluster.setLongestPathSize(lClusterLength);
    
    return lClusterLength;
}

int Solver::findLongestPath(Node* aNode)
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
      if(lDistances.find(lNeighbour) == lDistances.end())
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
      if(lDistances.find(lNeighbour) == lDistances.end())
      {
        lDistances.insert(std::make_pair(lNeighbour, lDistances[lNode] + 1));

        lQueue.push(lNeighbour);
      }
    }
  }

  lHighestDistance = 0;

  for(auto lIt = lDistances.begin(); lIt != lDistances.end(); lIt++)
  {
    if(lIt->second > lHighestDistance)
    {
      lHighestDistance = lIt->second;
    }
  }

  return lHighestDistance;
}
