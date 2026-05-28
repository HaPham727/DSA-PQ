#include "arraypq.h"

ArrayPQ::ArrayPQ(int capacity) 
{   
    m_array.resize(capacity);
};

void ArrayPQ::enqueuePacket(Packet p)
{
    for (int i{}; i < m_array.size(); i++)
    {
        if (p.getPriorityLevel() < (NUMBER_OF_PRIORITIZED_DOMAINS / NUMBER_OF_APQ_LEVELS) * i + NUMBER_OF_APQ_LEVELS)
            m_array[i].push(p);
    }
}

void ArrayPQ::dequeuePacket()
{
    for (int i{}; i < m_array.size(); i++)
    {
        if (!m_array[i].empty())
        {
            m_array[i].pop();
            break;
        }
    }
}

void ArrayPQ::printArrayPQ()
{
    for (int i{}; i < m_array.size(); i++)
    {
        
    }
}