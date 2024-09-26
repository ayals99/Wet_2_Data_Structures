#include "PlayerTreeNode.h"
#include <cmath>

PlayerTreeNode::PlayerTreeNode() {
    m_left = nullptr;
    m_right = nullptr;
    m_height = ZERO;
    m_amountOfPlayersInNode = ONE;
    m_sizeOfSubTree = ONE;
    m_player = ZERO;
}

PlayerTreeNode::PlayerTreeNode(Player player) :
        m_left(nullptr),
        m_right(nullptr),
        m_height(ZERO),
        m_amountOfPlayersInNode(ONE),
        m_sizeOfSubTree(ONE),
        m_player(player){}

PlayerTreeNode* PlayerTreeNode::getLeft() const{
    return m_left;
}

PlayerTreeNode* PlayerTreeNode::getRight() const{
    return m_right;
}

void PlayerTreeNode::setLeftChild(PlayerTreeNode* newLeft){
    m_left = newLeft;
}

void PlayerTreeNode::setRightChild(PlayerTreeNode* newRight){
    m_right = newRight;
}

int PlayerTreeNode::getAmountOfPlayersInNode() const{
    return m_amountOfPlayersInNode;
}

void PlayerTreeNode::incrementAmountOfPlayersInNode(){
    m_amountOfPlayersInNode++;
}

void PlayerTreeNode::decrementAmountOfPlayersInNode(){
    m_amountOfPlayersInNode--;
}

int PlayerTreeNode::getLeftHeight() const{
    return m_left == nullptr ? NULL_NODE_HEIGHT : m_left->m_height;
}

int PlayerTreeNode::getRightHeight() const{
    return m_right == nullptr ? NULL_NODE_HEIGHT : m_right->m_height;
}

int PlayerTreeNode::getBalanceFactor() const{
    return getLeftHeight() - getRightHeight();
}

void PlayerTreeNode::updateHeight(){
    //Changed the macro max to fmax
    m_height = (int)fmax(getLeftHeight(), getRightHeight()) + NODE_OWN_HEIGHT;
}

int PlayerTreeNode::getSizeOfSubTree() const {
    return m_sizeOfSubTree;
}

void PlayerTreeNode::updateSizeOfSubTree() {
    m_sizeOfSubTree = m_amountOfPlayersInNode;
    if(m_left != nullptr){
        m_sizeOfSubTree += m_left->getSizeOfSubTree();
    }
    if(m_right != nullptr){
        m_sizeOfSubTree += m_right->getSizeOfSubTree();
    }
}

Player PlayerTreeNode::getPlayer() const {
    return m_player;
}

void PlayerTreeNode::setAmountOfPlayersInNode(int newAmount) {
    m_amountOfPlayersInNode = newAmount;
}

void PlayerTreeNode::setData(Player newPlayer) {
    m_player = newPlayer;
}

bool PlayerTreeNode::isLeaf() const{
    return ( (m_left == nullptr) && (m_right == nullptr) );
}

bool PlayerTreeNode::hasLeftChildOnly() const{
    return ( (m_left != nullptr) && (m_right == nullptr) );
}

bool PlayerTreeNode::hasRightChildOnly() const{
    return ( (m_left == nullptr) && (m_right != nullptr) );
}

bool PlayerTreeNode::hasTwoChildren() const{
    return ( (m_left != nullptr) && (m_right != nullptr) );
}




