#include <iostream>
#include <string>
#include <regex>
#include <vector>

class Parser
{
    public:
        Parser();
        ~Parser();
        /**
         * Reads the input from stdin, and returns a pair with the number of nodes and a vector with all the connections between the nodes.
         */
        std::pair <int, std::vector<std::pair<int, int>>> Parse();
};