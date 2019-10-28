#include "Solver.h"

Solver::Solver(std::pair <int, std::vector<std::pair<int, int>>> aNodeList)
{
    mNumberOfNodes = aNodeList.first;

    
    // Add nodes
    for(int i = 0; i < mNumberOfNodes; i++)
    {
        mNodes.push_back(Node(i));
    }
    // Add connections
    for(auto aPair : aNodeList.second)
    {
        mNodes.at(aPair.first).addNeighbour(&mNodes.at(aPair.second));
        mNodes.at(aPair.second).addNeighbour(&mNodes.at(aPair.first));
    }
}

std::vector<Cluster> Solver::findClusters(std::vector<Node> aNodes)
{
    std::vector<Cluster> lClusters;
    std::vector<std::pair<int,bool>> lNodeList; //used for finding the clusters, int for the id and bool for if the 
    // Put all the nodes in the nodelist
    for(auto lNode : aNodes)
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

    // Find the longest cluster
    Cluster lLongestCluster = findLongestCluster(mClusters);

    // Find the center node of the cluster
    lLongestCluster.setMiddleNode(findMiddleNode(lLongestCluster));

    std::cout << "Central Middlenode : " << lLongestCluster.getMiddleNode().getId() << std::endl;

    // For the remaining clusters:
    for (auto& aCluster : mClusters)
    {
        // Find the center of the cluster
        aCluster.setMiddleNode(findMiddleNode(aCluster));
    }

    for(auto lCluster : mClusters)
    {
        if(lCluster == lLongestCluster)
        {
            std::cout << "lCluster == lLongestCluster" << std::endl;
        }
        else
        {
            std::cout << "lCluster NOT equal lLongestCluster" << std::endl;
        }
        
    }
    // Remove the longest cluster
    mClusters.erase(std::remove(mClusters.begin(), mClusters.end(), lLongestCluster), mClusters.end());
    std::cout << "We just removed yes" << std::endl;
        for(auto lCluster : mClusters)
    {
        if(lCluster == lLongestCluster)
        {
            std::cout << "lCluster == lLongestCluster" << std::endl;
        }
        else
        {
            std::cout << "lCluster NOT equal lLongestCluster" << std::endl;
        }
        
    }




    // Add the clusters to the largest cluster
    for (auto& aCluster : mClusters)
    {
        // Connect the center node from the biggest cluster to the center node of the smaller cluster.
        Node& lNode = lLongestCluster.getMiddleNode();
        lNode.addNeighbour(&aCluster.getMiddleNode());
        lLongestCluster.setMiddleNode(lNode);

        // Connect the center node from the smaller cluster to the center node of the biggest cluster.
        lNode = aCluster.getMiddleNode();
        lNode.addNeighbour(&lLongestCluster.getMiddleNode());
        aCluster.setMiddleNode(lNode);

        // Copy the nodes from the smaller cluster to the biggest cluster.
        for(auto& lNode : aCluster.getNodes())
        {
            lLongestCluster.addNode(lNode);
        }
    }

    for(auto& lNode : lLongestCluster.getNodes())
    {
        std::cout << "Node : " << lNode.getId() << " = " << lNode.getNeighbours().size() << std::endl;
        for(auto lNeighBour : lNode.getNeighbours())
        {
            std::cout << "Neighbor :" << lNeighBour->getId() << std::endl;
        }
    }

    for(auto& lNode : mNodes)
    {
        for(auto& lNeighbour : lNode.getNeighbours())
        {
            lNeighbour->addNeighbour(&lNode);
        }
    }

    // Find the longest path in the remaining cluster.
    // We do (-2) because we want the number of hops so we exclude the beginning and end node.
    // When we have only one node we want to limit it to 0.
    auto lLength = getClusterLength(lLongestCluster);
    std::cout << "Final clusterLength : " << lLength << std::endl;
    return std::max(0, getClusterLength(lLongestCluster) - 2);
}

Cluster Solver::findLongestCluster(std::vector<Cluster>& aClusters)
{
    Cluster lLongestCluster = aClusters.at(0);
    for(Cluster& lCluster : aClusters)
    {
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
        std::cout << "NeighbourSize : " << lNode.getNeighbours().size() << std::endl;
        if(lNode.getNeighbours().size() == 1)
        {
            lLeafQueue.push(lNode);   
        }
        else if(lNode.getNeighbours().size() == 0)
        {
            aCluster.setMaxLength(0);
            aCluster.setLongestPath(std::vector<Node>{lNode});
        }
    }
    
    while(!lLeafQueue.empty())
    {
        std::vector<Node> lLongestPath = findLongestPath(lLeafQueue.front());
        if (lLongestPath.size() > lClusterLength)
        {
            lClusterLength = lLongestPath.size();
            aCluster.setLongestPath(lLongestPath);
        }
        lLeafQueue.pop();
    }
    aCluster.setMaxLength(lClusterLength);
    return lClusterLength;
}

// DFS
// int Solver::findLongestPath(Node aNode)
// {
//     std::vector<int> lCurrentLongestPath = {aNode.getId()};
//     int lLongestPath = 0;
//     for (auto lNeighbour : aNode.getNeighbours())
//     {
//         lNeighbour->removeNeighbour(&aNode);
//         int lNeighbourLength = findLongestPath(*lNeighbour);
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

std::vector<Node> Solver::findLongestPath(Node aNode)
{
    std::vector<std::vector<Node>> lPathList = {std::vector<Node>{aNode}};
    bool lPathWasExpanded = true;
    std::cout << "begin findLongestPath, startnode ID: " << aNode.getId() << std::endl;

    while (lPathWasExpanded)
    {
        //std::cout << "while pathExpanded" << std::endl;
        lPathWasExpanded = false;

        for(auto& lPath : lPathList)
        {
            //std::cout << "beforeFor1" << std::endl;
            //std::cout << "lPathListSize : " << lPathList.size() << std::endl;
            // std::cout << "lPathSize : " << lPath.size() << std::endl;
            for(auto& lNeighbour : lPath.at(lPath.size()-1).getNeighbours())
            {
                // std::cout << " Inside forauto " << std::endl;
                // Check to prevent a node going back to its predecessor, predecessor is not a valid option.
                if(!((lPath.size() >= 2) && lNeighbour->getId() == lPath.at(lPath.size()-2).getId()))
                {
                  //std::cout << "intFor1" << std::endl;
                  std::vector<Node> lCurrentPath = lPath;
                  lCurrentPath.push_back(*lNeighbour);
                  lPathList.push_back(lCurrentPath);
                  lPathWasExpanded = true;
                }
            }
        }
        //std::cout << "b4forLoop2" << std::endl;
        int lLongestPath = 0;
        for(auto& lPath : lPathList)
        {
            if(lPath.size() > lLongestPath)
            {
                lLongestPath = lPath.size();
            }
        }
        //std::cout << "afterforLoop2" << std::endl;
        // Remove all the non-longest paths (the are either added in expanded form already or they are simply not the longest path).
        //std::cout << "b4remove, lPathListSize: " << lPathList.size() << std::endl;
        auto it = lPathList.begin();
        while (it != lPathList.end())
        {
            if (it->size() != lLongestPath)
            {
                it = lPathList.erase(it);
            }
            else
            {
                ++it;
            }
        }
        //std::cout << "afterremove, lPathListSize: " << lPathList.size() << std::endl;
    }
    return lPathList.at(0);
}

Node Solver::findMiddleNode(Cluster aCluster)
{
    // Havent found the middle node yet, so we set it.
    if(aCluster.getMaxLength() == -1)
    {
        getClusterLength(aCluster);
    }
    else
    {
        //std::cout << aCluster.getLongestPath().size() << std::endl;
        std::cout << "Middle node: " << std::to_string(aCluster.getLongestPath().at(floor(aCluster.getLongestPath().size() / 2.0)).getId()) << std::endl;
        return aCluster.getLongestPath().at(floor(aCluster.getLongestPath().size() / 2.0)); // Get the middle node based on the length.
    }
    return aCluster.getNodes().at(0);
}
