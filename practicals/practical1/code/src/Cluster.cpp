#include "Cluster.h"

Cluster::Cluster() : mMaxLength(-1)
{
}

Cluster::~Cluster()
{
}

void Cluster::addNode(Node aNode)
{
    mNodes.push_back(aNode);
}

void Cluster::setMaxLength(int aMaxLength)
{
    mMaxLength = aMaxLength;    
}

void Cluster::setMiddleNode(Node aNode)
{
    mMiddleNode = aNode;
}

Node& Cluster::getMiddleNode()
{
    return mMiddleNode;
}

std::vector<Node> Cluster::getNodes() const
{
    return mNodes;
}

int Cluster::getMaxLength() const
{
    return mMaxLength;
}

bool Cluster::operator==(Cluster& aCluster)
{
    return mMiddleNode == aCluster.getMiddleNode();
}