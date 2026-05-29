#include "packet.h"

Packet::Packet() {}

using namespace std;

int Packet::getPriorityLevel()
{
    //If Packet's domain is stored as a Prioritized Domain, return its priority level
    if (g_dpl_map.contains(m_domain))
        return (g_dpl_map[m_domain]);
    else
    //If Packet doesn't have a prioritized domain, give it priority level of 99 (lowest possible)
        return NUMBER_OF_PRIORITIZED_DOMAINS;
}

void Packet::generateTestCase(queue<Packet>& q)
{
    cout << "Generating test cases";

    int prevReceivingTimestamp{};

    for (int i{}; i < NUMBER_OF_PACKETS; i++)
    {
        Packet out{};

        //Generate random domain which may or maynot be prioritized 
        if (rand() % (100 / LIKELIHOOD_OF_PRIORIIZED_DOMAINS) == 0)
            out.m_domain = g_dpl_arr[rand() % NUMBER_OF_PRIORITIZED_DOMAINS]; 
        else
            out.m_domain = g_dpl_arr[NUMBER_OF_PRIORITIZED_DOMAINS]; //The 100th element in g_dpl_arr is designated unprioritized
        
        //Generate timestamp for when the request reaches the Router
        if (i == 0)
            out.m_receivingTimestamp = ((rand() % (MAX_RECEIVING_TIME - 1)) + 1);
        else
        {
            out.m_receivingTimestamp = (prevReceivingTimestamp + (rand() % (MAX_RECEIVING_TIME - 1)) + 1);
            prevReceivingTimestamp = out.m_receivingTimestamp;
        }
        //Generate time Router takes to process (dequeue) request
        out.m_processingTime = ((rand() % (MAX_PROCESSING_TIME - 1)) + 1);

        q.push(out);

        if ((i > 0) && (i % 300 == 0))
            cout << ".";
    }
    cout << "\nDone!\n";
}

void Packet::printTestCase(queue<Packet> q)
{        
    cout << "Test case looks like this: \n";
    while (!q.empty())
    {
        if (g_dpl_map[q.front().getDomain()] < 100)
            cout << "At " << q.front().getReceivingTimestamp()  << "ms, send the router a prioritized packet to " << q.front().getDomain() << ", taking " << q.front().getProcessingTime() <<"ms to process.\n";
        else
            cout << "At " << q.front().getReceivingTimestamp()  << "ms, send the router an unprioritized packet, taking " << q.front().getProcessingTime() <<"ms to process.\n";
        q.pop();
    }
    cout << "Done printing test case!\n";
}