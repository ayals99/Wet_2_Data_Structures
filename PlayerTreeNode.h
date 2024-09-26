#ifndef WET2_DATA_STRUCTURES_PLAYER_TREE_NODE_H
#define WET2_DATA_STRUCTURES_PLAYER_TREE_NODE_H

#include "static_const.h"

class PlayerTreeNode{
private:
    PlayerTreeNode* m_left;
    PlayerTreeNode* m_right;
    int m_height;
    int m_amountOfPlayersInNode;
    int m_sizeOfSubTree; // amount of Players in SubTree, not the amount of nodes
    Player m_player; // this is the player's strength, an int

public:
    PlayerTreeNode();
    explicit PlayerTreeNode(Player player);
    ~PlayerTreeNode() = default;
    PlayerTreeNode(const PlayerTreeNode& other) = delete;
    PlayerTreeNode& operator=(const PlayerTreeNode& other) = delete;

    PlayerTreeNode* getLeft() const;
    PlayerTreeNode* getRight() const;
    void setLeftChild(PlayerTreeNode* newLeft);
    void setRightChild(PlayerTreeNode* newRight);

    int getAmountOfPlayersInNode() const;
    void setAmountOfPlayersInNode(int newAmount);
    void incrementAmountOfPlayersInNode();
    void decrementAmountOfPlayersInNode();

    void updateSizeOfSubTree();
    int getSizeOfSubTree() const;

    int getLeftHeight() const;
    int getRightHeight() const;
    int getBalanceFactor() const;
    void updateHeight();

    Player getPlayer() const;
    void setData(Player newPlayer);



    bool isLeaf() const;
    bool hasLeftChildOnly() const;
    bool hasRightChildOnly() const;
    bool hasTwoChildren() const;
};

#endif //WET2_DATA_STRUCTURES_PLAYER_TREE_NODE_H
