#ifndef WET2_DATA_STRUCTURES_ARRIVAL_QUEUE_H
#define WET2_DATA_STRUCTURES_ARRIVAL_QUEUE_H

#include "static_const.h"
#include <iostream>


class arrivalQueue{
private:

    class arrivalQueueNode{

    public:
        arrivalQueueNode* m_next;
        Player m_data;

        /* big three and operator=() */
        explicit arrivalQueueNode(Player player) : m_next(nullptr), m_data(player){};
        ~arrivalQueueNode() = default;
        arrivalQueueNode(const arrivalQueueNode& other) = delete;
        arrivalQueueNode& operator=(const arrivalQueueNode& other) = delete;

    };

    arrivalQueueNode* m_head;
    int m_size;

public:

    /* big three and operator=() */
    arrivalQueue();
    ~arrivalQueue();
    arrivalQueue(const arrivalQueue& other) = delete;
    arrivalQueue& operator=(const arrivalQueue& other) = delete;

    /* arrivalQueue public methods*/
    Player top() const; // O(1) amortized
    void pop(); // O(1) amortized
    void push(Player player); // O(1) amortized
    void appendToStart(arrivalQueue* otherToAdd); // O(k_otherToAdd)

    int getSize() const;


};

#endif //WET2_DATA_STRUCTURES_ARRIVAL_QUEUE_H
