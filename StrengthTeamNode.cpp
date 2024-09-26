#include "StrengthTeamNode.h"

int StrengthTeamNode::getSizeOfSubtree() const{
    return m_sizeOfSubtree;
}

int StrengthTeamNode::getAccumulate() const{
    return m_accumulate;
}

StrengthTeamNode* StrengthTeamNode::getLeft() const{
    return m_left;
}

StrengthTeamNode* StrengthTeamNode::getRight() const{
    return m_right;
}

int StrengthTeamNode::getLeftHeight() const{
    return m_left == nullptr ? NULL_NODE_HEIGHT : m_left->m_height;
}

int StrengthTeamNode::getRightHeight() const{
    return m_right == nullptr ? NULL_NODE_HEIGHT : m_right->m_height;
}

int StrengthTeamNode::getBalanceFactor() const{
    return getLeftHeight() - getRightHeight();
}

Team* StrengthTeamNode::getTeam() const{
    return m_team;
}

int StrengthTeamNode::getMaxRankInSubTree() const {
    return m_maxRankInSubTree;
}

int StrengthTeamNode::getHeight() const{
    return m_height;
}

/**Setters**/
void StrengthTeamNode::setLeftChild(StrengthTeamNode* left){
    m_left = left;
}

void StrengthTeamNode::setRightChild(StrengthTeamNode* right){
    m_right = right;
}

void StrengthTeamNode::setTeam(Team* data){
    m_team = data;
}

/**Utilities**/
void StrengthTeamNode::updateHeight(){
    m_height = (int)fmax(getLeftHeight(), getRightHeight()) + NODE_OWN_HEIGHT;
}

bool StrengthTeamNode::isLeaf() const{
    return m_left == nullptr && m_right == nullptr;
}

bool StrengthTeamNode::hasLeftChildOnly() const{
    return m_left != nullptr && m_right == nullptr;
}

bool StrengthTeamNode::hasRightChildOnly() const{
    return m_left == nullptr && m_right != nullptr;
}

StrengthTeamNode::StrengthTeamNode(Team* team) :
                                                m_left(nullptr),
                                                m_right(nullptr),
                                                m_team(team),
                                                m_height(ZERO),
                                                m_maxRankInSubTree(team->getStrength() + team->getNumOfWins()),
                                                m_sizeOfSubtree(ONE),
                                                m_accumulate(ZERO)
                                                {}

void StrengthTeamNode::pushAccumulateToChildren() { // O(1)
    if (m_left != nullptr) {
        m_left->m_accumulate += m_accumulate;
    }
    if (m_right != nullptr) {
        m_right->m_accumulate += m_accumulate;
    }
    if (m_team!= nullptr) {
        m_team->changeNumOfWins(m_accumulate); // O(1)
    }
    m_accumulate = ZERO;
    this->updateMaxRankInSubTree();
}

void StrengthTeamNode::updateSizeOfSubTree() { // O(1)
    m_sizeOfSubtree = ONE;
    if (m_left != nullptr) {
        m_sizeOfSubtree += m_left->m_sizeOfSubtree;
    }
    if (m_right != nullptr) {
        m_sizeOfSubtree += m_right->m_sizeOfSubtree;
    }
}

void StrengthTeamNode::updateMaxRankInSubTree() { // O(1)
    // getNumOfWins should be updated because if we reached a node then we used pushAccumulateToChildren()
    // and the number of wins should be updated
    m_maxRankInSubTree = m_team->getStrength() + m_team->getNumOfWins() + m_accumulate; // O(1)
    if (m_left != nullptr) {
        m_maxRankInSubTree = (int)fmax(m_maxRankInSubTree, m_left->m_maxRankInSubTree + m_left->m_accumulate); // O(1)
    }
    if (m_right != nullptr) {
        m_maxRankInSubTree = (int)fmax(m_maxRankInSubTree, m_right->m_maxRankInSubTree + m_right->m_accumulate); // O(1)
    }
}

void StrengthTeamNode::incrementAccumulate() {
    m_accumulate++;
}
void StrengthTeamNode::decrementAccumulate() {
    m_accumulate--;
}