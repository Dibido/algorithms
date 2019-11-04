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
        std::cout << "lCurrenADDtNode : " << i << std::endl;
        mNodes.push_back(Node(i));
    }
    // Add connections
    int j = 0;
    for(auto& aPair : aNodeList.second)
    {
        j++;
        std::cout << "lCurrentCOUPLENode : " << j << std::endl;
        mNodes.at(aPair.first).addNeighbour(&mNodes.at(aPair.second));
        mNodes.at(aPair.second).addNeighbour(&mNodes.at(aPair.first));
    }

    for(auto& lNode : mNodes)
    {
      printf("Node %i has adress %p and neighbours: \n", lNode.getId(), (void *)&lNode); 
      for(auto& lNeighbour : lNode.getNeighbours())
      {
        printf("Node %i with adress %p\n", lNeighbour->getId(), (void *)lNeighbour); 
      }
      std::cout << std::endl;
    }

    //while(true);
}

std::vector<Cluster> Solver::findClusters(std::vector<Node>& aNodes)
{
    std::cout << "begin findClusters" << std::endl;

    std::vector<Cluster> lClusters;
    std::vector<std::pair<int,bool>> lNodeList; //used for finding the clusters, int for the id and bool for if it has been added to a cluster
    // Put all the nodes in the nodelist
    for(auto& lNode : aNodes)
    {
        lNodeList.push_back(std::pair<int, bool>(lNode.getId(), false)); // By default not in a cluster
    }

    std::cout << "Added all nodes to lNodeList " << std::endl;


    std::cout << "Adresses &aNodes.(i): " << std::endl;
    for(int i = 0; i < aNodes.size(); i++)
    {
      printf("Node with ID %i has address %p \n", aNodes.at(i).getId(), &aNodes.at(i));
    }
    std::cout << std::endl; 

    // Check the neighbours for every node and add them to a cluster
    for (int i = 0; i < lNodeList.size(); i++)
    {
        std::cout << "I: " << i << std::endl;
        // If not yet in cluster
        if(!lNodeList.at(i).second)
        {
            Cluster lCluster;
            // Add the neighbours
            std::queue<Node*> lNodeQueue;
            lNodeQueue.push(&aNodes.at(i));
            std::vector<Node*> lNeighbours = aNodes.at(i).getNeighbours();
            while(!lNodeQueue.empty())
            {
                std::cout << "Queue contains: " << std::endl;


               std::queue<Node*> lTemp = lNodeQueue; //copy the original queue to the temporary queue

              while (!lTemp.empty())
              {
                  auto lElement = lTemp.front();
                  printf("Node %i has address %p \n", lElement->getId(), (void*)lElement);
                  lTemp.pop();
              } 
              std::cout << "End queue contains" << std::endl;

                std::cout << std::endl;
                Node* lCurrentNode = lNodeQueue.front();
                lNodeQueue.pop();
                // Mark the node as handled
                lNodeList.at(lCurrentNode->getId()).second = true;
                lCluster.addNode(*lCurrentNode);
                printf("ADD node %i with address %p to lCluster", lCurrentNode->getId(), (void*)lCurrentNode);
                std::cout << std::endl;

                for(auto& lNode : lCluster.getNodes())
                {
                  printf("lNode %i with address %p in lCluster", lNode.getId(), (void*)&lNode);
                  std::cout << std::endl;
                }

                std::cout << std::endl;
                for(auto lNeighbour : lCurrentNode->getNeighbours())
                {
                    // Node has not been seen yet
                    if(!lNodeList.at(lNeighbour->getId()).second)
                    {
                        lNodeQueue.push(lNeighbour);
                    }
                }
            }

            std::cout << "=== BEFORE lClusters.push_back(lCluster) ==="<< std::endl;
        for(auto& lNode : lCluster.getNodes())
          {
            printf("Node %i has adress %p and neighbours: \n", lNode.getId(), (void *)&lNode); 
            for(auto& lNeighbour : lNode.getNeighbours())
            {
              printf("Node %i with adress %p\n", lNeighbour->getId(), (void *)lNeighbour); 
            }
            std::cout << std::endl;
          }
            lClusters.push_back(lCluster);
            std::cout << "=== AFTER lClusters.push_back(lCluster) ===" << std::endl;
        }
    }

    std::cout << "== Just before end of findClusters == "  << std::endl;

    for(auto& lNode : lClusters.at(0).getNodes())
    {
      printf("Node %i has adress %p and neighbours: \n", lNode.getId(), (void *)&lNode); 
      for(auto& lNeighbour : lNode.getNeighbours())
      {
        printf("Node %i with adress %p\n", lNeighbour->getId(), (void *)lNeighbour); 
      }
      std::cout << std::endl;
    }
    
    std::cout << "== End of findClusters == " << std::endl;

    return lClusters;
}

Solver::~Solver()
{
}

int Solver::compute()
{
    std::cout << "begin compute" << std::endl;
    // Find clusters
    mClusters = findClusters(mNodes);

    for(auto& lCluster : mClusters)
    {
    for(auto& lNode : lCluster.getNodes())
    {
      printf("Node %i has adress %p and neighbours: \n", lNode.getId(), (void *)&lNode); 
      for(auto& lNeighbour : lNode.getNeighbours())
      {
        printf("Node %i with adress %p\n", lNeighbour->getId(), (void *)lNeighbour); 
      }
      std::cout << std::endl;
    }
    }

    std::cout << "FoundClusters =============" << std::endl;

    // for(auto& lCluster : mClusters)
    // {
    //     for(auto& lNode : lCluster.getNodes())
    //     {
    //         std::cout << "NodeId : " << lNode.getId() << std::endl;
    //         for(auto& lNeighbour : lNode.getNeighbours())
    //         {
    //             std::cout << "\t Neighours : " << lNeighbour->getId();
    //         }
    //     }
    // }

    // Find the longest cluster
    Cluster lLongestCluster = findLongestCluster(mClusters);

    std::cout << "FoundLongestCluster =============" << std::endl;

    // Find the center node of the cluster
    // lLongestCluster.setMiddleNode(findMiddleNode(lLongestCluster));

    // Remove the longest cluster
    mClusters.erase(std::remove(mClusters.begin(), mClusters.end(), lLongestCluster), mClusters.end());

    int lLongestPathSize = lLongestCluster.getLongestPathSize();
    std::cout << "lLOngestPtahSize :" << lLongestPathSize << std::endl;

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
            // If there are 2 of equal size it will increase by 3.
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
            int lHalfSize = lLongestPathSize / 2;
            for(auto& lCluster : mClusters)
            {
                if(lCluster.getLongestPathSize() >= lHalfSize)
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
        std::cout << "ELS!" << std::endl;
        int lNumberOfHits = 0;
        int lHalfSize = ceil(lLongestPathSize / 2.0);
        std::cout << "HalfSIZE : " << lHalfSize << std::endl;
        std::cout << "ClustersSize :" << mClusters.size() << std::endl;
        for(auto& lCluster : mClusters)
        {
            if(lCluster.getLongestPathSize() >= lHalfSize)
            {
                lNumberOfHits++;
                std::cout << "lNumberOfHits :" << lNumberOfHits << std::endl;
            }
            if(lNumberOfHits == 2)
            {
                std::cout << "ReturnFIRST : " << (lLongestPathSize + 2) << std::endl;
                return lLongestPathSize + 2;
            }
        }
        std::cout << "ReturnLAST : " << (lLongestPathSize + 1) << std::endl;
        return lLongestPathSize + (lNumberOfHits * 1);
    }

    std::cout << "After all the returns" << std::endl;
}

Cluster Solver::findLongestCluster(std::vector<Cluster>& aClusters)
{
  std::cout << "findLC" << std::endl;
  for(auto& lCluster : aClusters)
  {
    for(auto& lNode : lCluster.getNodes())
    {
      printf("ID: %i, Address of lNode is %p and neighbours adresses:\n", lNode.getId(), (void *)&lNode); 
      for(auto& lNeighbour : lNode.getNeighbours())
      {
        printf("ID: %i, Address of lNeighbour is %p\n", lNeighbour->getId(), (void *)&lNeighbour); 

      }
    }
  }


    Cluster lLongestCluster = aClusters.at(0);
    for(Cluster& lCluster : aClusters)
    {
        std::cout << "FindingLongestPathinCluster" << std::endl;
        if (getClusterLength(lCluster) > lLongestCluster.getLongestPathSize())
        {
            lLongestCluster = lCluster;
        }
    }
    std::cout << "LongestLength : " << lLongestCluster.getLongestPathSize() << std::endl;
    return lLongestCluster;
}

int Solver::getClusterLength(Cluster& aCluster)
{
    int lClusterLength = 0;
    
    std::cout << "Begin GCL " << std::endl;

    for(auto& lNode : aCluster.getNodes())
    {
      printf("Address of lNode is %p\n", (void *)&lNode); 
    }
    std::cout << std::endl;

    Node& lFirstNode = aCluster.getFirstNode();

    std::cout << "before fLP executed " << std::endl;
    lClusterLength = findLongestPath(&lFirstNode, aCluster.getNumberOfNodes());  
    std::cout << "after fLP executed " << std::endl;
    
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
  std::cout << "startFLP, inserting node: " << aNode->getId() << " dist: 1 " << std::endl;

  std::queue<Node*> lQueue;
  lQueue.push(aNode);
  
  std::cout << "A" << std::endl;

  while(!lQueue.empty())
  {
    Node* lNode = lQueue.front();
    lQueue.pop();

    std::cout << "In while queue not empty" << std::endl;
    for(auto& lNeighbour : lNode->getNeighbours())
    {
      printf("Address of lNode is %p\n", (void *)lNode);  
      printf("Address of lNeighbour is %p\n", (void *)lNeighbour);  
      std::cout << "In for each neighbour mainNode(" << lNode->getId() << "), neighbour: " << lNeighbour->getId() << std::endl;
      // Node not processed earlier
      auto lIt = lDistances.find(lNeighbour);
      //auto lIt = std::find_if(lDistances.begin(), lDistances.end(), [&lNeighbour](const auto& lElem) {return lNeighbour->getId() == lElem.first->getId();});
      if(lIt == lDistances.end())
      {
        std::cout << "Adding node to queue" << std::endl;
        lDistances.insert(std::make_pair(lNeighbour, lDistances[lNode] + 1));
        lQueue.push(lNeighbour);
      }
    }

    std::cout << "After loop through neighbours" << std::endl;
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

  std::cout << "Before clearing lDistances: " << std::endl;
  for(auto const& lPair : lDistances)
  {
    std::cout << "Node: " << lPair.first->getId() << " dist: " << lPair.second << std::endl;
  }
  
  lDistances.clear();

  std::cout << "Inserting this in an empty lDistances : ID= " << lFurthestNode->getId() << ", dist=1" << std::endl;
  lDistances.insert(std::make_pair(lFurthestNode, 1));
  lQueue.push(lFurthestNode);

  while(!lQueue.empty())
  {
    Node* lNode = lQueue.front();
    lQueue.pop();

    for(auto& lNeighbour : lNode->getNeighbours())
    {
      std::cout << "2nd while, for lNeighbour (ID: " << lNeighbour->getId() << ")" << std::endl;
      // Node not processed earlier
      auto lIt = lDistances.find(lNeighbour);
      //auto lIt = std::find_if(lDistances.begin(), lDistances.end(), [&lNeighbour](const auto& lElem) {return lNeighbour->getId() == lElem.first->getId();});
      if(lIt == lDistances.end())
      {
        std::cout << "2nd while !queue.empty, adding this in lDistances: <ID: " << lNeighbour->getId() << ", Dist: " << lDistances[lNode] + 1 << std::endl;
        lDistances.insert(std::make_pair(lNeighbour, lDistances[lNode] + 1));

        lQueue.push(lNeighbour);
      }
    }
  }

  std::cout << "after 2nd while queue: " << std::endl;
  for(auto const& lPair : lDistances)
  {
    std::cout << "Node: " << lPair.first->getId() << " dist: " << lPair.second << std::endl;
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

  std::cout << "Returning lHighestDistance: " << lHighestDistance << "for node with ID: " << lNode->getId() << std::endl;
  return lHighestDistance;
}

// DFS
// long Solver::findLongestPath(Node aNode)
// {
//     std::vector<int> lCurrentLongestPath = {aNode.getId()};
//     long lLongestPath = 0;
//     for (auto lNeighbour : aNode.getNeighbours())
//     {
//         std::cout << "currentNeighbour :" <<lNeighbour->getId() << std::endl;
//         lNeighbour->removeNeighbour(&aNode);
//         std::cout << "afterRemove" << std::endl;
//         if (lNeighbour == nullptr)
//         {
//             std::cout << "NULLPTR" << std::endl;
//         }
//         else
//         {
//             std::cout << "NOT NULLPTR" << std::endl;
//         }
        
//         long lNeighbourLength = findLongestPath(*lNeighbour);
//         std::cout << "afterNeighbourLength" << std::endl;
//         if(lNeighbourLength > lLongestPath)
//         {
//             lCurrentLongestPath.push_back(lNeighbour->getId());
//             lLongestPath = lNeighbourLength;
//         }
//     }
//     std::cout << "lCurrentLongestPath" << std::endl;
//     for(int val : lCurrentLongestPath)
//     {
//         std::cout << "n : " << val << std::endl;
//     }
//     return lLongestPath + 1;
// }

// int Solver::findLongestPath(Node aNode)
// {
//     int lLongestPath = 0;
//     std::vector<std::vector<Node>> lPathList = {std::vector<Node>{aNode}};
//     bool lPathWasExpanded = true;
//     while (lPathWasExpanded)
//     {
//         std::cout << "while pathExpanded : " << lPathList.at(0).size() << std::endl;
//         std::cout << "pathListSize : " << lPathList.size() << std::endl;
//         lPathWasExpanded = false;

//         for(auto& lPath : lPathList)
//         {
//             //std::cout << "beforeFor1" << std::endl;
//             //std::cout << "lPathListSize : " << lPathList.size() << std::endl;
//             // std::cout << "lPathSize : " << lPath.size() << std::endl;
//             for(auto& lNeighbour : lPath.at(lPath.size()-1).getNeighbours())
//             {
//                 // std::cout << " Inside forauto " << std::endl;
//                 // Check to prevent a node going back to its predecessor, predecessor is not a valid option.
//                 if(!((lPath.size() >= 2) && lNeighbour->getId() == lPath.at(lPath.size()-2).getId()))
//                 {
//                   //std::cout << "intFor1" << std::endl;
//                   std::vector<Node> lCurrentPath = lPath;
//                   lCurrentPath.push_back(*lNeighbour);
//                   lPathList.push_back(lCurrentPath);
//                   lPathWasExpanded = true;
//                 }
//             }
//         }
//         //std::cout << "b4forLoop2" << std::endl;
//         int lLongestPath = 0;
//         for(auto& lPath : lPathList)
//         {
//             if(lPath.size() > lLongestPath)
//             {
//                 lLongestPath = lPath.size();
//             }
//         }
//         //std::cout << "afterforLoop2" << std::endl;
//         // Remove all the non-longest paths (the are either added in expanded form already or they are simply not the longest path).
//         //std::cout << "b4remove, lPathListSize: " << lPathList.size() << std::endl;
//         auto it = lPathList.begin();
//         while (it != lPathList.end())
//         {
//             if (it->size() != lLongestPath)
//             {
//                 it = lPathList.erase(it);
//             }
//             else
//             {
//                 ++it;
//             }
//         }
//         //std::cout << "afterremove, lPathListSize: " << lPathList.size() << std::endl;
//     }
//     return lLongestPath;
// }

/**
int Solver::findLongestPath(Node aNode)
{
    std::cout << "aNODEID" << aNode.getId() << std::endl;
    int lLongestPathLength = 0;
    
    //Tuple of predecessor, node,  and hopcount.
    std::vector<std::tuple<Node*,Node*,int>> lPaths = {std::tuple<Node*,Node*,int>{&aNode,&aNode,1}};

    for(int i = 0; i < lPaths.size(); i++)
    {
        auto& lCurrentTriplet = lPaths.at(i);
        std::cout << "cNODE : " << std::get<1>(lCurrentTriplet)->getId() << std::endl;
        int j = 0;
        for(auto& lNeighbour : std::get<1>(lCurrentTriplet)->getNeighbours())
        {
            j++;
            std::cout << "J VALUE : " << j << std::endl;
            if(!((lNeighbour->getId()) == std::get<0>(lCurrentTriplet)->getId()))
            {
                lPaths.push_back(std::tuple<Node*,Node*,int>(std::get<1>(lCurrentTriplet), lNeighbour, std::get<2>(lCurrentTriplet)+1));
            }
        }
    }

    for(auto& lTriplet : lPaths)
    {
        if(std::get<2>(lTriplet) > lLongestPathLength)
        {
            lLongestPathLength = std::get<2>(lTriplet);
        }
    }
    std::cout << "LongestPath from Node : " << aNode.getId() << " = " << lLongestPathLength << std::endl;
    return lLongestPathLength;
}*/

// std::vector<Node> Solver::findLongestPath(Node aNode)
// {
//     std::vector<std::vector<Node>> lPathList = {std::vector<Node>{aNode}};
//     bool lPathWasExpanded = true;
//     while (lPathWasExpanded)
//     {
//         std::cout << "while pathExpanded : " << lPathList.at(0).size() << std::endl;
//         std::cout << "pathListSize : " << lPathList.size() << std::endl;
//         lPathWasExpanded = false;

//         for(auto& lPath : lPathList)
//         {
//             //std::cout << "beforeFor1" << std::endl;
//             //std::cout << "lPathListSize : " << lPathList.size() << std::endl;
//             // std::cout << "lPathSize : " << lPath.size() << std::endl;
//             for(auto& lNeighbour : lPath.at(lPath.size()-1).getNeighbours())
//             {
//                 // std::cout << " Inside forauto " << std::endl;
//                 // Check to prevent a node going back to its predecessor, predecessor is not a valid option.
//                 if(!((lPath.size() >= 2) && lNeighbour->getId() == lPath.at(lPath.size()-2).getId()))
//                 {
//                   //std::cout << "intFor1" << std::endl;
//                   std::vector<Node> lCurrentPath = lPath;
//                   lCurrentPath.push_back(*lNeighbour);
//                   lPathList.push_back(lCurrentPath);
//                   lPathWasExpanded = true;
//                 }
//             }
//         }
//         //std::cout << "b4forLoop2" << std::endl;
//         int lLongestPath = 0;
//         for(auto& lPath : lPathList)
//         {
//             if(lPath.size() > lLongestPath)
//             {
//                 lLongestPath = lPath.size();
//             }
//         }
//         //std::cout << "afterforLoop2" << std::endl;
//         // Remove all the non-longest paths (the are either added in expanded form already or they are simply not the longest path).
//         //std::cout << "b4remove, lPathListSize: " << lPathList.size() << std::endl;
//         auto it = lPathList.begin();
//         while (it != lPathList.end())
//         {
//             if (it->size() != lLongestPath)
//             {
//                 it = lPathList.erase(it);
//             }
//             else
//             {
//                 ++it;
//             }
//         }
//         //std::cout << "afterremove, lPathListSize: " << lPathList.size() << std::endl;
//     }
//     return
// }

// Node Solver::findMiddleNode(Cluster aCluster)
// {
//     // Havent found the middle node yet, so we set it.
//     if(aCluster.getMaxLength() == -1)
//     {
//         getClusterLength(aCluster);
//     }
//     else
//     {
//         //std::cout << aCluster.getLongestPath().size() << std::endl;
//         std::cout << "Middle node: " << std::to_string(aCluster.getLongestPath().at(floor(aCluster.getLongestPath().size() / 2.0)).getId()) << std::endl;
//         return aCluster.getLongestPath().at(floor(aCluster.getLongestPath().size() / 2.0)); // Get the middle node based on the length.
//     }
//     return aCluster.getNodes().at(0);
// }
