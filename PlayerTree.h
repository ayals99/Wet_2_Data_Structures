#ifndef WET2_DATA_STRUCTURES_PLAYER_TREE_H
#define WET2_DATA_STRUCTURES_PLAYER_TREE_H


#include "static_const.h"
#include "PlayerTreeNode.h"

class PlayerTree {
private:
    PlayerTreeNode* m_root;
    int m_amountOfPlayers;

    /** Member Functions **/
    PlayerTreeNode* AUX_find(PlayerTreeNode* currentNode, int Key) const;

    void deleteTree(PlayerTreeNode* node);

    PlayerTreeNode* rightRotation(PlayerTreeNode* originalRoot);

    PlayerTreeNode* leftRotation(PlayerTreeNode* originalRoot);

    PlayerTreeNode* LR_Rotation(PlayerTreeNode* originalRoot);

    PlayerTreeNode* RL_Rotation(PlayerTreeNode* originalRoot);

    PlayerTreeNode* balance_Node(PlayerTreeNode* root);

    PlayerTreeNode* AUX_insert(PlayerTreeNode* parent, PlayerTreeNode* currentNode, PlayerTreeNode* newNode);
    void AUX_insert(PlayerTreeNode* parent, PlayerTreeNode* currentNode, Player playerToInsert);
    PlayerTreeNode* AUX_find_Minimum_In_Subtree(PlayerTreeNode* currentNode);
    PlayerTreeNode* AUX_remove(PlayerTreeNode* currentNode, PlayerTreeNode* nodeToRemove);
    void AUX_remove(PlayerTreeNode* currentNode, Player playerToRemove);

    Player findPlayerByRank(PlayerTreeNode* root, int sumOfRanksUntilNow, int rankToSearch);




public:
    PlayerTree();
    ~PlayerTree();

    void insert(Player player); // O(log k) where k is the amount of players
    void remove(Player player); // O(log k)
    PlayerTreeNode* getRoot() const; // O(1)
    void setRoot(PlayerTreeNode* newRoot); // O(1)
    void setSize(int newSize); // O(1)

    void InOrderToArray(Player* array); // O(k)

    // NOTICE: Cannot be called on an empty tree
    Player findMedian(); // O(log k)

    int getAmountOfPlayers() const; // O(1)
    bool PlayerIsInTree(Player player);


};



#endif //WET2_DATA_STRUCTURES_PLAYER_TREE_H
