#ifndef WET2_DATA_STRUCTURES_STRENGTH_TEAM_TREE_H
#define WET2_DATA_STRUCTURES_STRENGTH_TEAM_TREE_H

#include "StrengthTeamNode.h"

class StrengthTeamTree {
private:
    int m_size;
    StrengthTeamNode* m_root;

    void deleteTree(StrengthTeamNode* node); // O(n)

    StrengthTeamNode* rightRotation(StrengthTeamNode* originalRoot); // O(1)

    StrengthTeamNode* leftRotation(StrengthTeamNode* originalRoot); // O(1)

    StrengthTeamNode* LR_Rotation(StrengthTeamNode* originalRoot); // O(1)

    StrengthTeamNode* RL_Rotation(StrengthTeamNode* originalRoot); // O(1)

    bool exists_In_Tree(Team* team); // O(log n)
    StrengthTeamNode* AUX_remove(StrengthTeamNode* currentNode, Team* teamToRemove); // O(log n)
    StrengthTeamNode* AUX_insert(StrengthTeamNode* parent, StrengthTeamNode* currentNode, StrengthTeamNode* newNode); // O(log n)

    int AUX_findTeamIndex(StrengthTeamNode* currentNode, int sumOfIndexesUntilNow, Team* team); // O(log n)
    Team* AUX_findTeamByIndex(StrengthTeamNode* currentNode, int sumOfIndexesUntilNow, int indexToSearch); // O(log n)

    StrengthTeamNode* balance_Node(StrengthTeamNode* root); // O(1)
    void swapData(StrengthTeamNode *firstNode, StrengthTeamNode *secondNode); // O(1)



public:
    StrengthTeamTree(); // O(1)
    ~StrengthTeamTree(); // O(n)
    StrengthTeamTree(const StrengthTeamTree& other) = delete;
    StrengthTeamTree& operator=(const StrengthTeamTree& other) = delete;

    int getMaxRankInTree() const; // O(1)
    int getNumWins(Team* team); // O(log n)
    void insert(Team* team); // O(log n)
    void remove(Team* team); // O(log n) - will also update "wins" field of team that was removed
    Team* findTeamByIndex(int indexToSearch); // O(log n)
    int findTeamIndex(Team* team);
    Team* findCorrespondingLowPowerTeam(int lowPower); // O(log n)
    Team* findCorrespondingHighPowerTeam(int highPower); // O(log n)
    void accumulateWinnings (Team* teamLowBracket, Team* teamHighBracket); // O(log n)
    void accumulate(Team* teamHighBracket); // O(log n)
    void deccumulate(Team* teamLowBracket); // O(log n)
};

#endif
