#ifndef WET2_DATA_STRUCTURES_TEAM_TREE_HASH_TABLE_H
#define WET2_DATA_STRUCTURES_TEAM_TREE_HASH_TABLE_H

#include <exception>
#include "wet2util.h"
#include "AVL_Tree.h"
#include "Team.h"
#include "DynamicTeamTreeArray.h"
#include "iostream"

class TeamTreeHashTable{

private:
    DynamicTeamTreeArray* m_array;

    void reHashExpand(); //O(1) amotized
    void reHashShrink(); //O(1) amotized
    void deepTransfer(AVL_Node<Team>* node);
    int hashFunction(int teamId) const;

public:
    /**The Holy Trinity**/
    TeamTreeHashTable();
    ~TeamTreeHashTable();
    TeamTreeHashTable(const TeamTreeHashTable& hashTable) = delete;
    TeamTreeHashTable& operator=(const TeamTreeHashTable& hashTable) = delete;

    /**Utility Functions**/
    StatusType insert(Team* teamToInsert);
    StatusType remove(Team* teamToRemove);
    Team* find(int teamId);
    bool member(int teamId);
    bool isEmpty() const;

//    /**Print**/
//    friend std::ostream& operator<<(std::ostream& os, const TeamTreeHashTable& hashTable);

};



#endif
