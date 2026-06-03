#include "packet.h"

Packet::Packet() {}

using namespace std;   

//<random> test
std::mt19937_64 gen(seed);
//Exclusively used for generating processing time
uniform_int_distribution<int> uniform_rand(0, MAX_PROCESSING_TIME);
//Exclusively used for generating receiving timestamp
binomial_distribution<int> binomial_rand(5, 0.5); 
//Exclusively used for generating domains
poisson_distribution<int> poisson_rand(98); 
//binomial_distribution<int> poisson_rand(99, 0.2); 
//niform_int_distribution<int> poisson_rand(0, NUMBER_OF_PRIORITIZED_DOMAINS);

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
        int domainRand = poisson_rand(gen);

        if (domainRand < 99)
            out.m_domain = g_dpl_arr[domainRand];
        else
            out.m_domain = g_dpl_arr[NUMBER_OF_PRIORITIZED_DOMAINS];       
        
        int timestampRand = binomial_rand(gen);
        //Generate timestamp for when the request reaches the Router
        if (i == 0)
        {
            out.m_receivingTimestamp = (timestampRand % 10) + 1;
            prevReceivingTimestamp = out.m_receivingTimestamp;
        }
        else
        {
            out.m_receivingTimestamp = (prevReceivingTimestamp + (timestampRand % 10) + 1);
            prevReceivingTimestamp = out.m_receivingTimestamp;
        }

        //Generate time Router takes to process (dequeue) request
        out.m_processingTime = (uniform_rand(gen) + 1);

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