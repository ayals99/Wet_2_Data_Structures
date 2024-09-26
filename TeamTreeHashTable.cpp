#include "TeamTreeHashTable.h"

//Constructor
TeamTreeHashTable::TeamTreeHashTable() {
    try {
        m_array = new DynamicTeamTreeArray();
    } catch (std::bad_alloc& e) {
        throw e;
    }
}

//Destructor
TeamTreeHashTable::~TeamTreeHashTable() {
    delete m_array;
}

//hashFunction distributes the teams to the array
//by the team's id
int TeamTreeHashTable::hashFunction(int teamId) const {
    return teamId % m_array->getArraySize();
}

//deepTransfer is a recursive function that transfers
//all the nodes from the old array to the new array
//one tree at a time
void TeamTreeHashTable::deepTransfer(AVL_Node<Team>* node) {
    if (node == nullptr) {
        return;
    }
    deepTransfer(node->getLeft());
    deepTransfer(node->getRight());
    try {
        insert(node->getData());
        node->setData(nullptr);
    } catch (std::bad_alloc& e) {
        throw e;
    }
}

//reHash is a function that checks if the load factor
//is too high or too low and if it is, it creates a new
//array with the new size and transfers all the nodes
//from the old array to the new array
//then it deletes the old array
void TeamTreeHashTable::reHashExpand() {
    int arraySize = m_array->getArraySize();
    int numberOfElements = m_array->getNumberOfElements();
    int newArraySize = 0;
    double loadFactor = double (numberOfElements) / arraySize;
    DynamicTeamTreeArray* new_array;
    DynamicTeamTreeArray* old_array = m_array;
    if (loadFactor >= MAX_LOAD_FACTOR) {
        newArraySize = arraySize * EXPANSION_MULT;
    }
    else {
        return;
    }
    try {
        new_array = new DynamicTeamTreeArray(newArraySize);
    } catch (std::bad_alloc& e) {
        throw e;
    }
    m_array = new_array;
    //transfer all the nodes from the old array to the new array
    for (int i = 0; i < arraySize; i++) {
        if ((*old_array)[i]->getSize())
        {
            deepTransfer((*old_array)[i]->getRoot());
        }
    }
    delete old_array;
    //update the number of elements in the new array
}

void TeamTreeHashTable::reHashShrink() {
    int arraySize = m_array->getArraySize();
    int numberOfElements = m_array->getNumberOfElements();
    int newArraySize = 0;
    double loadFactor = double (numberOfElements) / arraySize;
    DynamicTeamTreeArray* new_array;
    DynamicTeamTreeArray* old_array = m_array;
    if (loadFactor <= MIN_LOAD_FACTOR && arraySize > MIN_ARRAY_SIZE) {
        newArraySize = arraySize / EXPANSION_MULT;
    }
    else {
        return;
    }
    try {
        new_array = new DynamicTeamTreeArray(newArraySize);
    } catch (std::bad_alloc& e) {
        throw e;
    }
    m_array = new_array;
    //transfer all the nodes from the old array to the new array
    for (int i = 0; i < arraySize; i++) {
        if ((*old_array)[i]->getSize())
        {
            deepTransfer((*old_array)[i]->getRoot());
        }
    }
    delete old_array;
    //update the number of elements in the new array
}

//insert is a function that inserts a new team to the hash table
//if the team is already in the hash table, it returns failure
//if the team is not in the hash table, it inserts the team
//and then checks if the load factor is too high
//if it is, it rehashes the table
StatusType TeamTreeHashTable::insert(Team* teamToInsert) {
    if (teamToInsert == nullptr) {
        return StatusType::INVALID_INPUT;
    }
    if (member(teamToInsert->getID())) {
        return StatusType::FAILURE;
    }
    StatusType status;
    int index = hashFunction(teamToInsert->getID());
    try {
        status = (*m_array)[index]->insert(teamToInsert);
    } catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    m_array->incrementNumberOfElements();
    try {
        reHashExpand();
    } catch (std::bad_alloc& e) {
        throw e;
    }
    return status;
}

//remove is a function that removes a team from the hash table
//if the team is not in the hash table, it returns failure
//if the team is in the hash table, it removes the team
//and then checks if the load factor is too low
//if it is, it rehashes the table
StatusType TeamTreeHashTable::remove(Team* teamToRemove) {
    if (teamToRemove == nullptr) {
        return StatusType::INVALID_INPUT;
    }

    StatusType status;

    if (!member(teamToRemove->getID())) {
        return StatusType::FAILURE;
    }

    int index = hashFunction(teamToRemove->getID());

    try {
        status = (*m_array)[index]->remove(teamToRemove);
    }
    catch (std::bad_alloc& e) {
        throw e;
    }

    m_array->decrementNumberOfElements();

    try {
        reHashShrink();
    } catch (std::bad_alloc& e) {
        throw e;
    }

    return status;
}

//find is a function that finds a team in the hash table
//if the team is not in the hash table, it returns nullptr
//if the team is in the hash table, it returns the team
Team* TeamTreeHashTable::find(int teamId) {
    int index = hashFunction(teamId);
    return (*m_array)[index]->find(teamId);
}

//member is a function that checks if a team is in the hash table
//if the team is in the hash table, it returns true
//if the team is not in the hash table, it returns false
bool TeamTreeHashTable::member(int teamId) {
    return find(teamId) != nullptr;
}

//std::ostream &operator<<(std::ostream &os, const TeamTreeHashTable &hashTable) {
//    os << "Hash Table:" << std::endl;
//    for (int i = 0; i < hashTable.m_array->getArraySize(); i++) {
//        os << "Tree " << i << ":" << std::endl;
//        (*hashTable.m_array)[i]->printTreeInOrder();
//        os << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
//    }
//    return os;
//}

bool TeamTreeHashTable::isEmpty() const {
    if (m_array->getNumberOfElements() == ZERO) {
        return true;
    }
    return false;
}


