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
    // Find clusters
    mClusters = findClusters(mNodes);
    std::cout << mClusters.size() << std::endl;
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
    // Create new connections

    // Find the longest path in the remaining cluster
    return 0;
}