#include <vector>
#include <limits.h>

#define EMPTY 0
#define INF INT_MAX 

class HopcroftKarp
{
  public:
    HopcroftKarp(int aNumberOfLeftNodes, int aNumberOfRightNodes);

    void addEdge(int aLeftNode, int aRightNode);

    // Returns the maximum matching, that is the corresponding number not the actual matching as we're not intersted in that.
    int computeMaximumMatching();

  private:
    // Contains the corresponding node of a left node if they form a pair in the matching. Otherwise EMPTY.
    std::vector<int> mLeftMatchings;
    
    // Contains the corresponding node of a right node if they form a pair in the matching. Otherwise EMPTY.
    std::vector<int> mRightMatchings;

    // Distances for all nodes on left side
    std::vector<int> mLeftDistances;

    // Contains for every node on left side a list of neighbours (from right side)
    std::vector<std::vector<int>> mLeftNeighbours;

    // Runs BFS and checks if an augmenting path is available. If so, returns true otherwise false. Used for computing maximum matching.
    bool BFS();

    // Runs DFS from given starting node (left side). Returns true if augmenting path available, otherwise false. Used for computing maximum matching.
    bool DFS(int aNode);
};