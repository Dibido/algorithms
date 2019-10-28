#include "Parser.h"

Parser::Parser()
{

}

Parser::~Parser()
{

}

std::pair <int, std::vector<std::pair<int, int>>> Parser::Parse()
{
    // Read input
    // Read the first line
    std::string lLine;
    std::getline(std::cin, lLine);
    // Parse the values
    static std::regex lRegex("\\d+");
    std::regex_iterator<std::string::iterator> lRit(lLine.begin(),
            lLine.end(), lRegex);
    std::regex_iterator<std::string::iterator> lrEnd;
    //Check whether the right amount of arguments were given
    if(std::distance(lRit,lrEnd) != 2){
        std::cout << "Wrong input for number of values on line 1" << std::endl;
        std::cout << "Output :" << lLine << std::endl;
        exit(-1);
    }
    std::string lNodes = lRit->str();
    int lNumberOfNodes = std::stoi(lNodes);
    ++lRit;
    std::string lEdges = lRit->str();
    int lNumberOfEdges = std::stoi(lEdges);
    // Create the nodes
    std::pair <int, std::vector<std::pair<int, int>>> lNodeVector;
    std::vector<std::pair<int,int>> lConnections;
    // Read the rest
    for (std::string lLine; std::getline(std::cin, lLine);) {
        std::regex_iterator<std::string::iterator> lRit(lLine.begin(), lLine.end(), lRegex);
        std::regex_iterator<std::string::iterator> lrEnd;
        // Check whether the right amount of arguments were given
        if(std::distance(lRit,lrEnd) != 2){
            std::cout << "Wrong input for number of values on the line." << std::endl;
            exit(-1);
        }
        int lLeftNode = std::stoi(lRit->str());
        ++lRit;
        int lRightNode = std::stoi(lRit->str());

        lConnections.push_back(std::pair<int,int>(lLeftNode, lRightNode));
    }
    return std::pair <int, std::vector<std::pair<int, int>>> (lNumberOfNodes, lConnections);
}