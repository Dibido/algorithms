#include "Solver.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

Solver::Solver()
{
    auto& input = std::cin;

    int lNumberOfActors, lNumberOfMovies;
    input >> lNumberOfActors >> lNumberOfMovies;

    mNumberOfNodes = lNumberOfActors;
    mNumberOfMovies = lNumberOfMovies;
    
    for(int i = 0; i < lNumberOfActors; i++) // Read the actresses
    {
        std::string lActressName;
        input >> lActressName; // Read the name
        mNodes.insert(make_pair(lActressName, Node(i, lActressName, false))); // Create a node
    }

    for(int i = lNumberOfActors; i < (2 * lNumberOfActors); i++) // Read the actors
    {
        std::string lActorName;
        input >> lActorName; // Read the name
        mNodes.insert(make_pair(lActorName, Node(i, lActorName, true)));  // Create a node
    }

    // Handle the movies
    for(int i = 0; i < lNumberOfMovies; i++)
    {
        std::string lMovieName;
        input >> lMovieName; // Read moviename
        unsigned int lNumberOfActors;
        input >> lNumberOfActors; // Read number of actors
        std::vector <std::string> lActorNames;
        for(size_t i = 0; i < lNumberOfActors; i++) // Read the actors
        {
            std::string lActorName;
            input >> lActorName;
            lActorNames.push_back(lActorName);
        }
        
        for(size_t i = 0; i < lActorNames.size(); i++)
        {
          auto lIt = mNodes.find(lActorNames.at(i));
          
            for(size_t j = i; j < lActorNames.size(); j++)
            {
                if(j != i)
                {
                    auto lNeighbourIt = mNodes.find(lActorNames.at(j));
                    // Only add neighbour if it has the opposite gender
                    if(lIt->second.isMale() != lNeighbourIt->second.isMale())
                    {
                        lIt->second.addNeighbour(&lNeighbourIt->second);
                        lNeighbourIt->second.addNeighbour(&lIt->second);
                    }
                }
            }
        }
    }

    for(auto lIt = mNodes.begin(); lIt != mNodes.end(); lIt++)
    {
      mNodePointers.push_back(&lIt->second);
    }

    // for(auto& lIt : mNodePointers)
    // {
     // lIt->printNode();
    // }
}

Solver::~Solver()
{
}

std::string Solver::compute()
{
    for(auto& lNode : mNodePointers)
    {
        // Check whether every node has at least one member (of opposite gender)
        if((lNode->getNeighbours().size() == 0))
        {
            std::cout << "NODE CAUSED NO OPPOSITE GENDER : " << std::endl;
            lNode->printNode();
            return "Veronique";
        }
    }

    // Find all the clusters in the input
    mClusters = findClusters();

    // Check whether all the clusters have a perfect matching
    for(Cluster& lCluster : mClusters)
    {
        if (!lCluster.isBalanced())
        {
            std::cout << "CLUSTER CAUSED INBALANCE." << std::endl;
            return "Veronique";
        }
    }

    for(Cluster& lCluster : mClusters)
    {
        if (!lCluster.hasPerfectMatching())
        {
            std::cout << "CLUSTER CAUSED NO PERFECT MATCHING." << std::endl;
            return "Veronique";
        }
    }

    // All the clusters have a perfect matching, thus Mark wins.
    return "Mark";
}

std::vector<Cluster> Solver::findClusters()
{
    std::vector<Cluster> lClusters;
 
    // First storing all our nodes in this nodeMap, so they are ordered on ID (0, 1... n) and we can store them in lNodeList.
    std::map<int, Node*> lNodeMap;

    for(unsigned int i = 0; i < mNodePointers.size(); i++)
    {
      lNodeMap.insert(std::make_pair(mNodePointers.at(i)->getId(),mNodePointers.at(i)));
    }

    std::vector<std::pair<int,bool>> lNodeList; //used for finding the clusters, int for the id and bool for if it has been added to a cluster

    // Put all the nodes in the nodelist, also make it so that mNodePointers are ordered (that is, .at(0) has pointer for node with id 0 etc.)
    int i = 0;
    for(auto lIt = lNodeMap.begin(); lIt != lNodeMap.end(); lIt++)
    {
        lNodeList.push_back(std::pair<int, bool>(lIt->second->getId(), false)); // By default not in a cluster
        mNodePointers.at(i) = lIt->second;
        i++;
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
            lNodeQueue.push(mNodePointers.at(i));

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
                      lNodeList.at(lNeighbour->getId()).second = true;
                    }
                }
            }
            
            //lCluster.renumber();
            int lBalance = 0;
            for(auto& lNode : lCluster.getNodes())
            {
                if (lNode->isMale())
                {
                    lBalance++;
                }
                else
                {
                    lBalance--;
                }
            }
            std::cout << "Balance : " << lBalance << std::endl;
            if (lBalance != 0)
            {
                lCluster.setBalanced(false);
            }
            lClusters.push_back(lCluster);
        }
    }

    return lClusters;
}

