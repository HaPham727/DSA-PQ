#ifndef ARRAYPQ_H
#define ARRAYPQ_H

#include <vector>
#include <queue>

#include "packet.h"

using namespace std;

//int ;

class ArrayPQ
{
private:
    //Store queues of packets in a vector with size = NUMBER_OF_APQ_LEVELS + 1
    vector<queue<Packet>> m_array;

public:
    //Constructor function to 
    ArrayPQ(int size);

    void enqueuePacket(Packet p);

    void dequeuePacket();
    
    void printArrayPQ();
};

#endif