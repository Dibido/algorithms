#include "HopcroftKarp.h"
#include <queue>
#include <iostream>

HopcroftKarp::HopcroftKarp(int aNumberOfLeftNodes, int aNumberOfRightNodes)
{
  mLeftMatchings.resize(aNumberOfLeftNodes + 1);
  mRightMatchings.resize(aNumberOfRightNodes + 1);

  mLeftDistances.resize(aNumberOfLeftNodes + 1);
  mLeftNeighbours.resize(aNumberOfLeftNodes + 1);
}

void HopcroftKarp::addEdge(int aLeftNode, int aRightNode)
{
  mLeftNeighbours.at(aLeftNode).push_back(aRightNode);
}

int HopcroftKarp::computeMaximumMatching()
{
  for(unsigned int i = 0; i < mLeftMatchings.size(); i++)
  {
    mLeftMatchings.at(i) = EMPTY;
  }

  for(unsigned int i = 0; i < mRightMatchings.size(); i++)
  {
    mRightMatchings.at(i) = EMPTY;
  }

  int lMaxMatching = 0;

  // As long as BFS finds an augmenting path
  while(BFS())
  {
    //std::cout << "BFS returned true " << std::endl;
    // For every left vertex:
    for(unsigned int i = 1; i < mLeftMatchings.size(); i++)
    {
      // If its free, and theres an augmenting path starting from this vertex
      if(mLeftMatchings.at(i) == EMPTY && DFS(i))
      {
        lMaxMatching++;
      }
    }
  }

  return lMaxMatching;
}

bool HopcroftKarp::BFS() 
{ 
    //std::cout << " start BFS " << std::endl;
    // Will contain vertices from left side only.
    std::queue<int> lQueue; 
  
    // Initialize distance to dummy node 0 as infinite 
    mLeftDistances.at(0) = INF; 

    for (unsigned int i = 1; i < mLeftMatchings.size(); i++) 
    { 
        // Free vertex, so add to queue.
        if (mLeftMatchings.at(i) == EMPTY) 
        { 
            lQueue.push(i);

            // Also set corresponding distance to 0 
            mLeftDistances.at(i) = 0;              
        } 
        else
        {
          mLeftDistances.at(i) = INF;
        }
    } 
   
    while (!lQueue.empty()) 
    { 
      //std::cout << "Dequeing" << std::endl;
        // Dequeue a left vertex 
        int lLeftNode = lQueue.front(); 
        lQueue.pop(); 
  
        // If path can be shortened
        if (mLeftDistances.at(lLeftNode) < mLeftDistances.at(0)) 
        { 
            // For all neighbours 
            for (auto lIterator = mLeftNeighbours.at(lLeftNode).begin(); lIterator != mLeftNeighbours.at(lLeftNode).end(); ++lIterator) 
            { 
                int lRightNode = *lIterator; 
  
                if (mLeftDistances.at(mRightMatchings.at(lRightNode)) == INF) 
                { 
                    // Consider pair in matching and add to queue 
                    mLeftDistances.at(mRightMatchings.at(lRightNode)) = mLeftDistances.at(lLeftNode) + 1; 
                    lQueue.push(mRightMatchings.at(lRightNode)); 
                }   
            } 
        } 
    } 
  
    // If distance to dummy node (0) is smaller then INF, it means there is an augmenting path.
    return (mLeftDistances.at(0) < INF); 
}

bool HopcroftKarp::DFS(int aLeftNode) 
{ 
    if (aLeftNode != EMPTY) 
    { 
        // For all neighbours 
        for (auto lIterator = mLeftNeighbours.at(aLeftNode).begin(); lIterator != mLeftNeighbours.at(aLeftNode).end(); ++lIterator) 
        { 
            // Neighbour of given left node 
            int lRightNode = *lIterator; 
  
            // Just one difference between the two
            if (mLeftDistances.at(mRightMatchings.at(lRightNode)) == mLeftDistances.at(aLeftNode) + 1) 
            { 
                // If dfs for leftnode where the rightnode is currently matched with also true  
                if (DFS(mRightMatchings.at(lRightNode))) 
                { 
                    mRightMatchings.at(lRightNode) = aLeftNode; 
                    mLeftMatchings.at(aLeftNode) = lRightNode; 
                    return true; 
                } 
            } 
        } 
  
        // No augmenting path from given aLeftNode
        mLeftDistances.at(aLeftNode) = INF; 
        return false; 
    }

    return true; 
} 