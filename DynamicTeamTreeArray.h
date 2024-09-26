#ifndef WET2_DATA_STRUCTURES_DYNAMIC_TEAM_TREE_ARRAY_H
#define WET2_DATA_STRUCTURES_DYNAMIC_TEAM_TREE_ARRAY_H

#include <exception>
#include "wet2util.h"
#include "AVL_Tree.h"
#include "Team.h"
#include "static_const.h"

class DynamicTeamTreeArray {

private:
    AVL_Tree<Team>** m_array;
    int m_arraySize;
    int m_numberOfElements;


public:

    /**The Holy Trinity**/
    explicit DynamicTeamTreeArray(int size = INITIAL_SIZE);
    ~DynamicTeamTreeArray();
    DynamicTeamTreeArray(const DynamicTeamTreeArray& array) = delete;
    DynamicTeamTreeArray& operator=(const DynamicTeamTreeArray& array) = delete;

    /**Getters**/
    int getArraySize() const;
    int getNumberOfElements() const;

    /**Utility Functions**/
    void updateNumberOfElements();
    void incrementNumberOfElements();
    void decrementNumberOfElements();

    //operator [] to access elements in the array
    AVL_Tree<Team>* operator[](int index);
};



#endif //WET2_DATA_STRUCTURES_DYNAMIC_TEAM_TREE_ARRAY_H
