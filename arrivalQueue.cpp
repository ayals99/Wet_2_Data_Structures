#include "arrivalQueue.h"

arrivalQueue::arrivalQueue() : m_head(nullptr), m_size(0){}

Player arrivalQueue::top() const {
    if (m_head == nullptr){
        return NOT_FOUND;
    }
    return m_head->m_data;
}

void arrivalQueue::pop() {
    if(m_head == nullptr){
        return;
    }
    arrivalQueueNode* toPop = m_head;
    m_head = m_head->m_next;
    // check if cos lik
    delete toPop;
    m_size--;
}

void arrivalQueue::push(Player player) {
    arrivalQueueNode* newNode;
    try{
        newNode = new arrivalQueueNode(player);
    }
    catch (std::bad_alloc& e){
        throw e;
    }
    newNode->m_next = m_head;
    m_head = newNode;
    m_size++;
}

arrivalQueue::~arrivalQueue() {
    while (m_head != nullptr){
        pop();
    }
}

void arrivalQueue::appendToStart(arrivalQueue *otherToAdd) {
    if(otherToAdd->m_head == nullptr){
        return;
    }
    if(this->m_head == nullptr){
        this->m_head = otherToAdd->m_head;
        otherToAdd->m_head = nullptr;
        this->m_size = otherToAdd->m_size;
        otherToAdd->m_size = ZERO;
        return;
    }

    arrivalQueueNode* current = otherToAdd->m_head;
    while(current->m_next){
        current = current->m_next;
    }
    current->m_next = this->m_head;
    this->m_head = otherToAdd->m_head;
    otherToAdd->m_head = nullptr;
    this->m_size = otherToAdd->m_size + this->m_size;
    otherToAdd->m_size = ZERO;

}

int arrivalQueue::getSize() const {
    return m_size;
}