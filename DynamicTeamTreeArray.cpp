#include "DynamicTeamTreeArray.h"

//constructor
DynamicTeamTreeArray::DynamicTeamTreeArray(int size) : m_arraySize(size), m_numberOfElements(0) {
    try {
        //allocate memory for the array
        m_array = new AVL_Tree<Team> *[m_arraySize];
    }catch (std::bad_alloc& e) {
        throw e;
    }
    for (int i = 0; i < m_arraySize; i++) {
        try {
            m_array[i] = new AVL_Tree<Team>; //allocate memory for the tree
        } catch (std::bad_alloc& e) {
            for (int j = 0; j < i; j++) {
                delete m_array[j];
            }
            delete[] m_array;
            throw e;
        }
    }
}

//destructor
DynamicTeamTreeArray::~DynamicTeamTreeArray() {
    for (int i = 0; i < m_arraySize; i++) {
        delete m_array[i];
    }
    delete[] m_array;
}

int DynamicTeamTreeArray::getArraySize() const {
    return m_arraySize;
}

int DynamicTeamTreeArray::getNumberOfElements() const {
    return m_numberOfElements;
}

void DynamicTeamTreeArray::updateNumberOfElements() {
    for (int i = 0; i < m_arraySize; ++i) {
        if (m_array[i]->getSize() > 0) {
            m_numberOfElements += m_array[i]->getSize();
        }
    }
}

AVL_Tree<Team>* DynamicTeamTreeArray::operator[](int index) {
    if (index < 0 || index >= m_arraySize) {
        throw std::out_of_range("Index is out of bounds");
    }
    return m_array[index];
}

void DynamicTeamTreeArray::incrementNumberOfElements() {
    m_numberOfElements++;
}

void DynamicTeamTreeArray::decrementNumberOfElements() {
    m_numberOfElements--;
}

