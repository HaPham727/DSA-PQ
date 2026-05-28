#ifndef HEAPPQ_H
#define HEAPPQ_H

#include <iostream>
#include <vector>
#include <array>
#include <queue>
#include <map>

#include "packet.h"

using namespace std;

class HeapPQ
{
private:
    //Current size of the heap
    int m_size;
    //Max size of heap
    int m_capacity;
    //Store elements as a vector
    vector<Packet> m_heap;
    //Returns the parent index
    int parentIndex(int i){return (i - 1) / 2;}
    //Returns the left child
    int leftIndex(int i){return 2 * i + 1;}
    //Returns the right child
    int rightIndex(int i){return 2 * i + 2;}
public:
    //Constructor
    HeapPQ(int capacity); 
    //Recursively heapify a sub-tree
    void heapify(int i);
    //Check if heap is empty
    bool isEmpty();
    //enHPQ a key into the min heap
    void enHPQ(Packet k);
    //Extracts the minumum element
    Packet deHPQ();
    //Print the heap
    void printHeap();
};



#endif