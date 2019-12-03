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
        Node lNode = Node(i, lActressName, false);
        mNodes.push_back(lNode); // Create a node
        std::cout << mNodes.at(0).getActorName() << std::endl;
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
                                lFirstActor.addNeighbour(&lSecondActor);
                                lSecondActor.addNeighbour(&lFirstActor);
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
    return "Veronica";
}