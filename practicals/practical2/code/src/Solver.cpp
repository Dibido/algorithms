#include "Solver.h"

#include "HopcroftKarp.h"

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
//#include "Stopwatch.hpp"

Solver::Solver()
{

    //namespace sw = stopwatch;

    //  sw::Stopwatch lWatch;
    //sw::Stopwatch lWatch2;

    auto& input = std::cin;
    input >> mNumberOfNodes >> mNumberOfMovies;

    for(int i = 0; i < mNumberOfNodes; i++) // Read the actresses
    {
        std::string lActressName;
        input >> lActressName; // Read the name
        mNodes.insert(make_pair(lActressName, Node(i, lActressName, false))); // Create a node
    }

    for(int i = mNumberOfNodes; i < (2 * mNumberOfNodes); i++) // Read the actors
    {
        std::string lActorName;
        input >> lActorName; // Read the name
        mNodes.insert(make_pair(lActorName, Node(i, lActorName, true)));  // Create a node
    }

    //std::cout << "Creating all nodes took: " << lWatch.lap() << std::endl;

        //    unsigned long lMillis = 0;
          //  unsigned long lMillis2 = 0;
         //   unsigned long lMillis3 = 0;
         //   unsigned long lMillis4 = 0;

    // Handle the movies
    for(int i = 0; i < mNumberOfMovies; i++)
    {

        //std::cout << "After " << i << " movies time elapsed: " << lWatch() << std::endl;
  
        std::string lMovieName;
        input >> lMovieName; // Read moviename
        unsigned int lNumberOfActors;
        input >> lNumberOfActors; // Read number of actors

        // Read the actors
        std::vector <std::string> lActorNames;
        for(size_t i = 0; i < lNumberOfActors; i++) 
        {
            std::string lActorName;
            input >> lActorName;
            lActorNames.push_back(lActorName);
        }
        
       // lWatch.lap<sw::mus>();

        // For each actor
        for(size_t i = 0; i < lActorNames.size(); i++)
        {
          //lWatch2.lap<sw::mus>();
          auto lIt = mNodes.find(lActorNames.at(i));
          //lMillis3 += lWatch2.lap<sw::mus>();
          
            // Add connections to the actors listed after it
            for(size_t j = i; j < lActorNames.size(); j++)
            {
                if(j != i)
                {
                  //  lWatch2.lap<sw::mus>();
                    auto lNeighbourIt = mNodes.find(lActorNames.at(j));
                    //lMillis3 += lWatch2.lap<sw::mus>();
                    
                    // Only add neighbour if it has the opposite gender
                    if(lIt->second.isMale() != lNeighbourIt->second.isMale())
                    {
                      //  lWatch2.lap<sw::mus>();
                      lIt->second.addNeighbour(&lNeighbourIt->second);
                      // lNeighbourIt->second.addNeighbour(&lIt->second); IS ALSO DONE NOW IN ADDNEIGHBOUR!
                      //  lMillis2 += lWatch2.lap<sw::mus>();
                    }
                }
                
            }
            
        }
     // lMillis+= lWatch.lap<sw::mus>();
    }

      // std::cout << "Last forloop in total took: " << lMillis << std::endl;
        //   std::cout << "lMillis2: " << lMillis2 << std::endl;
         //  std::cout << "lMillis3: " << lMillis3 << std::endl;

   // std::cout << "Handling all movies took: " << lWatch.lap() << std::endl;

    for(auto lIt = mNodes.begin(); lIt != mNodes.end(); lIt++)
    {
      mNodePointers.push_back(&lIt->second);
    }

    //std::cout << "Pushbacking mNodes into mNodePointers took: " << lWatch.lap() << std::endl;
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
            // std::cout << "NODE CAUSED NO OPPOSITE GENDER : " << std::endl;
            // lNode->printNode();
            return "Veronique";
        }
    }
    
    // Find all the clusters in the input
    mClusters = findClusters();
    // Check if maybe any of the clusters is unbalanced
    for(auto& lCluster : mClusters)
    {
      if(!lCluster.isBalanced())
      {
        return "Veronique";
      }
    }
    // Check whether all the clusters have a perfect matching, if no perfect matching veronique wins
    for(Cluster& lCluster : mClusters)
    {
        if (!lCluster.hasPerfectMatching())
        {
            // std::cout << "CLUSTER CAUSED NO PERFECT MATCHING." << std::endl;
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

            // Will keep track if number of added males/females is equal in the end. Adding male is +1, female -1.
            int lBalance = 0;

            while(!lNodeQueue.empty())
            {
                Node* lCurrentNode = lNodeQueue.front();
                lNodeQueue.pop();

                // Mark the node as handled
                lNodeList.at(lCurrentNode->getId()).second = true;
                lCluster.addNode(lCurrentNode);

                lBalance += lCurrentNode->isMale() ? 1 : -1;

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
            
            if (lBalance != 0)
            {
                lCluster.setBalanced(false);
            }

            lClusters.push_back(lCluster);
        }
    }

    return lClusters;
}