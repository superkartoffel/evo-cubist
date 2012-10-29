#ifndef __BINARYHEAP_H_
#define __BINARYHEAP_H_

#include <QList>

template <class T>
class BinaryHeap {
public:
    void enqueue(T item) {
        mList.append(item);
        int i = mList.count() - 1;
        int parent = (i-1)/2;
        while (parent >= 0 && mList[i] < mList[parent]) {
            T temp = mList[parent];
            mList[parent] = mList[i];
            mList[i] = temp;
            i = parent;
            parent = (i-1)/2;
        }
    }


    T dequeue(void) {
        if (mList.isEmpty())
            return T();
        T item = mList[0];
        int i = 0;
        mList[0] = mList[count()-1];
        mList.removeLast();
        while (i < count()) {
            int left = 2*i+1;
            int right = left + 1;
            if (right > count()-1)
                break;
            int min = left;
            if (mList[right] < mList[left])
                min = right;
            if (mList[i] > mList[min]) {
                T data = mList[min];
                mList[min] = mList[i];
                mList[i] = data;
                i = min;
            }
            else
                break;
        }
        return item;
    }

    inline int count(void) const { return mList.count(); }
    inline bool isEmpty(void) const { return mList.isEmpty(); }
    T top(void) const { return !mList.isEmpty()? mList.first() : T(); }

private:
    QList<T> mList;
};


#endif // __BINARYHEAP_H_
