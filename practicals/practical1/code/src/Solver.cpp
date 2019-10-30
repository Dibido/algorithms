#include "Solver.h"
#include <tuple>
#include <chrono>
#include <thread>

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
}

std::vector<Cluster> Solver::findClusters(std::vector<Node> aNodes)
{
    std::cout << "begin findClusters" << std::endl;

    std::vector<Cluster> lClusters;
    std::vector<std::pair<int,bool>> lNodeList; //used for finding the clusters, int for the id and bool for if the 
    // Put all the nodes in the nodelist
    for(auto lNode : aNodes)
    {
        lNodeList.push_back(std::pair<int, bool>(lNode.getId(), false)); // By default not in a cluster
    }

    std::cout << "Added all nodes to lNodeList " << std::endl;

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
                Node* lCurrentNode = lNodeQueue.front();
                lNodeQueue.pop();
                // Mark the node as handled
                lNodeList.at(lCurrentNode->getId()).second = true;
                lCluster.addNode(*lCurrentNode);
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
    // Find clusters
    mClusters = findClusters(mNodes);

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

    int lLongestPathSize = lLongestCluster.getLongestPath();
    std::cout << "lLOngestPtahSize :" << lLongestPathSize << std::endl;

    //Evenly sized longest path
    if(lLongestPathSize % 2 == 0)
    {
        int lNumberOfHits = 0;
        for(auto& lCluster : mClusters)
        {
            // If there is a cluster of equal size
            if(lCluster.getLongestPath() == lLongestPathSize)
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
                if(lCluster.getLongestPath() >= lHalfSize)
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
            if(lCluster.getLongestPath() >= lHalfSize)
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
    Cluster lLongestCluster = aClusters.at(0);
    for(Cluster& lCluster : aClusters)
    {
        std::cout << "FindingLongestPathinCluster" << std::endl;
        if (getClusterLength(lCluster) > lLongestCluster.getMaxLength())
        {
            lLongestCluster = lCluster;
        }
    }
    std::cout << "LongestLength : " << lLongestCluster.getMaxLength() << std::endl;
    return lLongestCluster;
}

int Solver::getClusterLength(Cluster& aCluster)
{
    int lClusterLength = 0;
    // Find the leaf nodes
    std::queue<Node> lLeafQueue;
    for(auto& lNode : aCluster.getNodes())
    {
        std::cout << "getNode : " << lNode.getId() << std::endl;
        if(lNode.getNeighbours().size() == 1)
        {
            lLeafQueue.push(lNode);
        }
        else if(lNode.getNeighbours().size() == 0)
        {
            aCluster.setLongestPath(1);
        }
    }
    
    while(!lLeafQueue.empty())
    {
        std::cout << "LeafQueue : " << lLeafQueue.size() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        int lLongestPath = findLongestPath(lLeafQueue.front());
        if (lLongestPath > lClusterLength)
        {
            lClusterLength = lLongestPath;
            aCluster.setLongestPath(lLongestPath);
        }
        lLeafQueue.pop();
    }
    aCluster.setMaxLength(lClusterLength);
    return lClusterLength;
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
}

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
