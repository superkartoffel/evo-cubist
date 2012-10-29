#ifndef __PRIORITYQUEUE_H_
#define __PRIORITYQUEUE_H_

#include "binaryheap.h"




template <class T>
class PriorityQueue
{
public:
    void enqueue(T item) {
        mHeap.enqueue(item);
    }

    T dequeue(void) {
        return mHeap.dequeue();
    }

    int count(void) const {
        return mHeap.count();
    }

private:
    BinaryHeap<T> mHeap;
};
#endif // __PRIORITYQUEUE_H_
