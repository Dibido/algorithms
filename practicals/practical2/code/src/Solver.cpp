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
        mNodes.push_back(Node(i, lActressName, false)); // Create a node
    }

    for(int i = lNumberOfActors; i < (2 * lNumberOfActors); i++) // Read the actors
    {
        std::string lActorName;
        input >> lActorName; // Read the name
        mNodes.push_back(Node(i, lActorName, true));  // Create a node
    }

    // Handle the movies
    for(int i = 0; i < lNumberOfMovies; i++)
    {
        std::string lMovieName;
        input >> lMovieName; // Read moviename
        int lNumberOfActors;
        input >> lNumberOfActors; // Read number of actors
        std::vector <std::string> lActorNames;
        for(int i = 0; i < lNumberOfActors; i++) // Read the actors
        {
            std::string lActorName;
            input >> lActorName;
            lActorNames.push_back(lActorName);
        }
        for(size_t i = 0; i < lActorNames.size(); i++)
        {
            for(Node& lFirstActor : mNodes)
            {
                if (lFirstActor.getActorName() == lActorNames.at(i)) // Find the actor
                {
                    for(size_t j = i; j < lActorNames.size(); j++)
                    {
                        for(Node& lSecondActor : mNodes)
                        {
                            if (lSecondActor.getActorName() == lActorNames.at(j)) // Find the actor
                            {
                                if(!(lFirstActor == lSecondActor)) // No self loops
                                {
                                    lFirstActor.addNeighbour(&lSecondActor);
                                    lSecondActor.addNeighbour(&lFirstActor);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    for (auto lNode : mNodes)
    {
        lNode.printNode();
    }
}

Solver::~Solver()
{
}

std::string Solver::compute()
{
    for(auto lNode : mNodes)
    {
        if(lNode.hasOppositeGenderNeighbour() == false) // Check whether every node has at least one member of opposite gender
        {
            std::cout << "NODE CAUSED NO OPPOSITE GENDER : " << std::endl;
            lNode.printNode();
            return "Veronique";
        }
    }

    // Find all the clusters in the input
    mClusters = findClusters(mNodes);

    // Check whether all the clusters have a perfect matching
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