#include "Cluster.h"

Cluster::Cluster()
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

std::vector<Node> Cluster::getNodes() const
{
    return mNodes;
}

int Cluster::getMaxLength() const
{
    return mMaxLength;
}