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

    // Find the center of the cluster
    lLongestCluster.setMiddleNode(findMiddleNode(lLongestCluster));

    // For the remaining clusters:
    // Find the center of the cluster
    // Connect it to the center of the biggest cluster
    // Find the longest path in the remaining cluster
    return 0;
}

Cluster Solver::findLongestCluster(std::vector<Cluster> aClusters)
{
    Cluster lLongestCluster = aClusters.at(0);
    for(auto lCluster : aClusters)
    {
        if (getClusterLength(lCluster) > lLongestCluster.getMaxLength())
        {
            lLongestCluster = lCluster;
        }
    }
    std::cout << "LongestLength : " << lLongestCluster.getMaxLength() << std::endl;
    return lLongestCluster;
}

int Solver::getClusterLength(Cluster aCluster)
{
    int lClusterLength = 0;
    // Find the leaf nodes
    std::queue<Node> lLeafQueue;
    for(auto lNode : aCluster.getNodes())
    {
        if(lNode.getNeighbours().size() == 1)
        {
            lLeafQueue.push(lNode);       
        }
    }
    while(!lLeafQueue.empty())
    {
        // Run the dijkstra algorithm
        int lLongestPath = findLongestPath(lLeafQueue.front());
        if (lLongestPath > lClusterLength)
        {
            lClusterLength = lLongestPath;
        }
        lLeafQueue.pop();
    }
    aCluster.setMaxLength(lClusterLength);
    return lClusterLength;
}

int Solver::findLongestPath(Node aNode)
{
    int lLongestPath = 0;
    for (auto lNeighbour : aNode.getNeighbours())
    {
        lNeighbour->removeNeighbour(&aNode);
        lLongestPath = std::max(lLongestPath, findLongestPath(*lNeighbour));
    }
    return lLongestPath + 1;
}


/**
// Utility function that will return the depth 
// of the tree 
int depthOfTree(struct Node *ptr) 
{ 
    // Base case 
    if (!ptr) 
        return 0; 
  
    int maxdepth = 0; 
  
    // Check for all children and find 
    // the maximum depth 
    for (vector<Node*>::iterator it = ptr->child.begin(); 
                           it != ptr->child.end(); it++) 
  
        maxdepth = max(maxdepth , depthOfTree(*it)); 
  
    return maxdepth + 1; 
}

*/

Node Solver::findMiddleNode(Cluster aCluster)
{
    return aCluster.getNodes().at(0);
}