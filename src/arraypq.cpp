#include "arraypq.h"

ArrayPQ::ArrayPQ(int capacity) 
{   
    m_array.resize(capacity);
};

void ArrayPQ::enqueuePacket(Packet p)
{
    if (p.getPriorityLevel() == NUMBER_OF_PRIORITIZED_DOMAINS)
    {
        m_array[((NUMBER_OF_PRIORITIZED_DOMAINS + 1) / DOMAINS_PER_APQ_LEVEL)].push(p);
    }
    else
    {
        m_array[p.getPriorityLevel() / DOMAINS_PER_APQ_LEVEL].push(p);
    }
}

Packet ArrayPQ::dequeuePacket()//REMEMBER TO CHECK EMPTY
{
    Packet out{};
    for (int i{}; i < m_array.size(); i++)
    {
        if (!m_array[i].empty())
        {
            out = m_array[i].front();
            m_array[i].pop();
            return out;
        }
    }
    return out;
}

bool ArrayPQ::isEmpty()
{   
    for (int i{}; i < m_array.size(); i++)
    {
        if (!m_array[i].empty())
            return false;
    }
    return true;
}