#ifndef WET2_DATA_STRUCTURES_STRENGTH_TEAM_NODE_H
#define WET2_DATA_STRUCTURES_STRENGTH_TEAM_NODE_H

#include "Team.h"

class StrengthTeamNode {
private:
    StrengthTeamNode* m_left;
    StrengthTeamNode* m_right;
    Team* m_team;
    int m_height;
    int m_maxRankInSubTree;
    int m_sizeOfSubtree;
    int m_accumulate;

public:
    /* big three and operator=() */
    explicit StrengthTeamNode(Team* team);
    ~StrengthTeamNode() = default;
    StrengthTeamNode(const StrengthTeamNode& other) = delete;
    StrengthTeamNode& operator=(const StrengthTeamNode& other) = delete;

    int getSizeOfSubtree() const;

    int getAccumulate() const;

    StrengthTeamNode* getLeft() const;

    StrengthTeamNode* getRight() const;

    int getLeftHeight() const;

    int getRightHeight() const;

    int getBalanceFactor() const;

    Team* getTeam() const;

    int getHeight() const;

    int getMaxRankInSubTree() const;

    /**Setters**/
    void setLeftChild(StrengthTeamNode* left);

    void setRightChild(StrengthTeamNode* right);

    void setTeam(Team* data);

    /**Utilities**/
    void updateHeight();
    void updateSizeOfSubTree();
    void updateMaxRankInSubTree();

    void incrementAccumulate();
    void decrementAccumulate();

    bool isLeaf() const;

    bool hasLeftChildOnly() const;

    bool hasRightChildOnly() const;

    void pushAccumulateToChildren();
};
#endif //WET2_DATA_STRUCTURES_STRENGTH_TEAM_NODE_H
