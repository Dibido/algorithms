#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

class Node
{
    public:
        Node();
        Node(int aNodeId, std::string aActorName, bool aIsMale);
        virtual ~Node();

        void setNodeId(const int aNodeid);
        void setActorName(std::string aActorName);
        
        void printNode() const;
        void addNeighbour(Node* aNode);
        const std::vector <Node*>& getNeighbours() const;
        void removeNeighbour(Node* aNode);
        void setOppositeNeighbour();

        int getId() const;
        std::string getActorName() const;
        bool isMale() const;
        bool isFemale() const;
        bool hasOppositeGenderNeighbour() const;

        bool operator==(const Node& aNode) const;

        Node(const Node& aNode);

        Node& operator=(const Node& aNode);

    private:

        int mId;
        std::string mActorName;
        bool mIsMale;                                    
        bool mHasOppositeGenderNeighbour;
        std::vector<Node*> mNeighbours;
};