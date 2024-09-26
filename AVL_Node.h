#ifndef WET_1_DATA_STRUCTURES_AVL_NODE_H
#define WET_1_DATA_STRUCTURES_AVL_NODE_H


#include "AVL_Tree.h"
#include "static_const.h"
#include <cmath>
#include <exception>

template <typename T>
class AVL_Node{
private:
    AVL_Node<T>* m_left;
    AVL_Node<T>* m_right;
    T* m_data;
    int m_height;

public:
    /**Getters**/
    AVL_Node<T>* getLeft() const{
        return m_left;
    }

    AVL_Node<T>* getRight() const{
        return m_right;
    }

    int getLeftHeight() const{
        return m_left == nullptr ? NULL_NODE_HEIGHT : m_left->m_height;
    }

    int getRightHeight() const{
        return m_right == nullptr ? NULL_NODE_HEIGHT : m_right->m_height;
    }

    int getBalanceFactor() const{
        return getLeftHeight() - getRightHeight();
    }

    T* getData() const{
        return m_data;
    }

    int getHeight() const{
        return m_height;
    }

    /**Setters**/
    void setLeftChild(AVL_Node<T>* left){
        m_left = left;
    }

    void setRightChild(AVL_Node<T>* right){
        m_right = right;
    }

    void setData(T* data){
        m_data = data;
    }

    /**Utilities**/
    void updateHeight(){
        m_height = std::fmax(getLeftHeight(), getRightHeight()) + NODE_OWN_HEIGHT;
    }

    bool isLeaf() const{
        return m_left == nullptr && m_right == nullptr;
    }
    bool hasLeftChildOnly() const{
        return m_left != nullptr && m_right == nullptr;
    }
    bool hasRightChildOnly() const{
        return m_left == nullptr && m_right != nullptr;
    }


    AVL_Node(): m_left(nullptr), m_right(nullptr), m_data(nullptr), m_height(ZERO){}
    explicit AVL_Node(T* data): m_left(nullptr), m_right(nullptr), m_data(data),
            m_height(ZERO){}
    ~AVL_Node(){
        delete m_data;
    }
    AVL_Node(const AVL_Node& other) = delete;
    AVL_Node& operator=(const AVL_Node& other) = delete;
};

#endif //WET_1_DATA_STRUCTURES_AVL_Node_H