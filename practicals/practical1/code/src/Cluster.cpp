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