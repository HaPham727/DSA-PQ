#ifndef ARRAYPQ_H
#define ARRAYPQ_H

#include <vector>
#include <queue>

#include "packet.h"

using namespace std;

class ArrayPQ
{
private:
    //Store queues of packets in a vector with size = NUMBER_OF_APQ_LEVELS 
    vector<queue<Packet>> m_array;

public:
    //Constructor function to 
    ArrayPQ(int size);

    void enqueuePacket(Packet p);

    Packet dequeuePacket();

    bool isEmpty();
};

#endif