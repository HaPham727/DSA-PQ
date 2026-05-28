#include "packet.h"

Packet::Packet() {}

using namespace std;

int Packet::getPriorityLevel()
{
    if (g_dpl_map.contains(m_domain))
        return (g_dpl_map[m_domain]);
    else
        return NUMBER_OF_PRIORITIZED_DOMAINS;
}

//ARRAY-BASED TESTCASE VERSION
void Packet::generateTestCase(array<Packet, NUMBER_OF_PACKETS>& arr)
{
    cout << "Generating test cases";
    for (int i{}; i < NUMBER_OF_PACKETS; i++)
    {
        //Generate random domain which may be prioritized 
        if (rand() % (100 / LIKELIHOOD_OF_PRIORIIZED_DOMAINS) == 0)
            arr[i].m_domain = g_dpl_arr[rand() % 100]; 
        else
            arr[i].m_domain = g_dpl_arr[100]; //The 100th element in g_dpl_arr is designated unprioritized

        //Generate time Router takes to process (dequeue) request
        arr[i].m_processingTime = (rand() % 10);

        //Generate timestamp for when the request reaches the Router
        if (i == 0)
            arr[i].m_receivingTimestamp = (rand() % 10) + 1;
        else
            arr[i].m_receivingTimestamp = (arr[i - 1].m_receivingTimestamp + (rand() % 10) + 1);

        if ((i > 0) && (i % 300 == 0))
            cout << ".";
    }
    cout << "\nDone!\n";
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
            out.m_domain = g_dpl_arr[rand() % 100]; 
        else
            out.m_domain = g_dpl_arr[100]; //The 100th element in g_dpl_arr is designated unprioritized
        
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