#include "main.h"

using namespace std;

inline constexpr int MAX_SIM_TIME = 10000;

bool optionIsValid(std::string_view opt)
{
    return (opt.size() == 1 && (opt == "y" || opt == "Y" || opt == "n" || opt == "N"));
}

bool getOption(std::string_view opt)
{
    return ((opt == "y" || opt == "Y") ? true : false);
}

//Queue-based Test Case VERSION
int main()
{
    cout << "Queue-based Test Case VERSION!\n";

    //Seeding the RNG
    unsigned int seed = 1234; 
    srand(seed);

    //Helper variables
    string print_testCase_reply;

    //Create an queue to store the test case
    queue<Packet> testCase{};
    Packet::generateTestCase(testCase);

    //Ask to print test case
    do {
        std::cout << "Do you want to print the test case? [y/N]: ";
    } while (getline(std::cin, print_testCase_reply) && !optionIsValid(print_testCase_reply));
    if (getOption(print_testCase_reply))
        Packet::printTestCase(testCase);
    

    //QUEUE-BASED TESTCASE VERSION
    //Do Heap-based PQ Unit Test
    {        
        //Timer variables for Heap-based Priority Queue
        int hpqClockTime{},
            hpqSimTime{},
            hpqFinalTime{},
            hpqClockTimeStart{clock()};

        //Initialize the HPQ
        HeapPQ h(1000);

        //Helper variables for testing
        int packetEnqueuedIndex{};
        int nextRouterFreeTimestamp{};
        int dequeuedPacketsCounter{};

//---------------------------------------------------------------Run test---------------------------------------------------------------//
        while (true)
        {
            //If router:
            //  - Exceeds MAX_SIM_TIME
            //  OR 
            //  - Has enqueued all packets in testCase
            //  - Has dequeued NUMBER_OF_PACKETS packets
            //  - Is free 
            //-> Break 
            if ((hpqSimTime > MAX_SIM_TIME) 
             || ((packetEnqueuedIndex >= NUMBER_OF_PACKETS) 
             && (dequeuedPacketsCounter >= NUMBER_OF_PACKETS)
             && (hpqSimTime >= nextRouterFreeTimestamp))
            )
            {
                cout << "\nDone Testing!\n";
                break;
            }

            cout << "\n---------------------------------------------------------------------------------\n";
            //Print Current Time
            cout << "Current SimTime: " << hpqSimTime << "\n"; 

            //cout << "packetEnqueuedIndex = " << packetEnqueuedIndex;
            //Enqueue (receive) packet
            if (!testCase.empty() 
             && packetEnqueuedIndex < NUMBER_OF_PACKETS 
             && testCase.front().getReceivingTimestamp() == hpqSimTime)
            {
                //cout << "packetEnqueuedIndex = " << packetEnqueuedIndex;

                //Print Packets as they are received
                if (g_dpl_map[testCase.front().getDomain()] < 100)
                    cout << "Router received a prioritized packet to " << testCase.front().getDomain() << ", which will take " << testCase.front().getProcessingTime() <<"ms to process.\n";
                else
                    cout << "Router received an unprioritized packet, which will take " << testCase.front().getProcessingTime() <<"ms to process.\n";

                //Log received packet to HPQ
                h.enHPQ(testCase.front());

                //Remove received packet from HPQ
                testCase.pop();

                packetEnqueuedIndex++;
            }


            
            //Dequeue (process) packet if router is free
            if (hpqSimTime >= nextRouterFreeTimestamp 
             && packetEnqueuedIndex <= NUMBER_OF_PACKETS
             && !h.isEmpty())
            {
                //Packet frontPacket = h.frontHPQ();
                //nextRouterFreeTimestamp += frontPacket.getProcessingTime();
                //h.deHPQ();

                Packet p = h.deHPQ();
                string domain = p.getDomain();
                int processingTime = p.getProcessingTime();
                if (g_dpl_map[p.getDomain()] < 100)
                    cout << "Router started processing a prioritized packet to " << domain << ", which will take " << processingTime <<"ms to process.\n";
                else
                    cout << "Router started processing an unprioritized packet, which will take " << processingTime <<"ms to process.\n";
                
                nextRouterFreeTimestamp = hpqSimTime + processingTime;

                cout << "Router will be free at: " << nextRouterFreeTimestamp << "\n";

                dequeuedPacketsCounter++;
            }

            //Check if router is free
            if (hpqSimTime >= nextRouterFreeTimestamp)
                cout << "Router is free!\n";
            else
                cout << "Router is busy!\n";

            //Advance SimTime
            hpqSimTime++;

            cout << "---------------------------------------------------------------------------------\n";
        }

        hpqClockTime = (clock() - hpqClockTimeStart);

        hpqFinalTime = hpqSimTime + hpqClockTime;
        
        cout << "\nHeap-based queue took " << hpqClockTime << " milliseconds in ClockTime to run!\n";
        cout << "Heap-based queue took " << hpqSimTime << " milliseconds in SimTime to run!\n";
        cout << "Heap-based queue took " << hpqFinalTime << " milliseconds in ClockTime + SimTime to run!\n";
    }



    //cout << "There are " << g_domains_and_priority_level_array.size() << " domains in the g_domains_and_priority_level_array \n";
    cout << "Packet is " << sizeof(Packet) << " bytes! \n";

}

/*
//Array-Based Test Case VERSION
int main()
{
    cout << "Hello, from DSA-Priority-Queue!\n";

    //Seeding the RNG
    unsigned int seed = 12345; 
    srand(seed);

    //Helper variables
    string print_testCase_reply;

    //Create an queue to store the test case
    queue<Packet> testCase{};

    //Create an array to store the test case
    //array<Packet, NUMBER_OF_PACKETS> testCase{};

    Packet::generateTestCase(testCase);

    //Ask to print test case
    do {
        std::cout << "Do you want to print the test case? [y/N]: ";
    } while (getline(std::cin, print_testCase_reply) && !optionIsValid(print_testCase_reply));
    if (getOption(print_testCase_reply))
    {
        cout << "Test case looks like this: \n";
        for (auto i{0uz}; i < testCase.size(); i++)
        {
            if (g_dpl_map[testCase[i].getDomain()] < 100)
                cout << "At " << testCase[i].getReceivingTimestamp()  << "ms, send the router a prioritized packet to " << testCase[i].getDomain() << ", taking " << testCase[i].getProcessingTime() <<"ms to process.\n";
            else
                cout << "At " << testCase[i].getReceivingTimestamp()  << "ms, send the router an unprioritized packet, taking " << testCase[i].getProcessingTime() <<"ms to process.\n";
        }
    }

    //QUEUE-BASED TESTCASE VERSION
    //Do Heap-based PQ Unit Test
    {        
        //Timer variables for Heap-based Priority Queue
        int hpqClockTime{},
            hpqSimTime{},
            hpqFinalTime{},
            hpqClockTimeStart{clock()};

        //Initialize the HPQ
        HeapPQ h(1000);

        //Helper variables for testing
        int packetEnqueuedIndex{};
        int nextRouterFreeTimestamp{};
        int dequeuedPacketsCounter{};

        while (true)
        {
            //If router:
            //  - Has enqueued all packets in testCase
            //  - Is free 
            //  - Has dequeued NUMBER_OF_PACKETS packets
            //-> Break 
            if ((hpqSimTime > MAX_SIM_TIME) || ((packetEnqueuedIndex >= NUMBER_OF_PACKETS) 
             && (hpqSimTime >= nextRouterFreeTimestamp))
             && (dequeuedPacketsCounter >= NUMBER_OF_PACKETS)
            )
            {
                cout << "\nDone Testing!\n";
                break;
            }

            cout << "\nCurrent SimTime: " << hpqSimTime << "\n"; 

            




             //   cout << "packetEnqueuedIndex = " << packetEnqueuedIndex;
            //Enqueue (receive) packet
            if (packetEnqueuedIndex < NUMBER_OF_PACKETS && testCase[packetEnqueuedIndex].getReceivingTimestamp() == hpqSimTime)
            {
            //    cout << "packetEnqueuedIndex = " << packetEnqueuedIndex;


                //Print Packets as they are received
                if (g_dpl_map[testCase[packetEnqueuedIndex].getDomain()] < 100)
                    cout << "Router received a prioritized packet to " << testCase[packetEnqueuedIndex].getDomain() << ", which will take " << testCase[packetEnqueuedIndex].getProcessingTime() <<"ms to process.\n";
                else
                    cout << "Router received an unprioritized packet, which will take " << testCase[packetEnqueuedIndex].getProcessingTime() <<"ms to process.\n";

                h.enHPQ(testCase[packetEnqueuedIndex]);
                packetEnqueuedIndex++;
            }

            
            //Dequeue (process) packet if router is free
            if (hpqSimTime >= nextRouterFreeTimestamp && packetEnqueuedIndex < NUMBER_OF_PACKETS)
            {
                //Packet frontPacket = h.frontHPQ();
                //nextRouterFreeTimestamp += frontPacket.getProcessingTime();
                //h.deHPQ();

                if (!h.isEmpty())
                {
                    Packet p = h.deHPQ();
                    nextRouterFreeTimestamp += p.getProcessingTime();
                    dequeuedPacketsCounter++;
                }
            }

            //Advance SimTime
            hpqSimTime++;
        }

        hpqClockTime = (clock() - hpqClockTimeStart);

        hpqFinalTime = hpqSimTime + hpqClockTime;
        
        cout << "\nHeap-based queue took " << hpqClockTime << " milliseconds in ClockTime to run!\n";
        cout << "Heap-based queue took " << hpqSimTime << " milliseconds in SimTime to run!\n";
        cout << "Heap-based queue took " << hpqFinalTime << " milliseconds in ClockTime + SimTime to run!\n";
    }


    //ARRAY-BASED TESTCASE VERSION

    //Do Heap-based PQ Unit Test
    {        
        //Timer variables for Heap-based Priority Queue
        int hpqClockTime{},
            hpqSimTime{},
            hpqFinalTime{},
            hpqClockTimeStart{clock()};

        //Initialize the HPQ
        HeapPQ h(1000);

        //Helper variables for testing
        int packetEnqueuedIndex{};
        int nextRouterFreeTimestamp{};
        int dequeuedPacketsCounter{};

        while (true)
        {
            //If router:
            //  - Has enqueued all packets in testCase
            //  - Is free 
            //  - Has dequeued NUMBER_OF_PACKETS packets
            //-> Break 
            if ((hpqSimTime > MAX_SIM_TIME) || ((packetEnqueuedIndex >= NUMBER_OF_PACKETS) 
             && (hpqSimTime >= nextRouterFreeTimestamp))
             && (dequeuedPacketsCounter >= NUMBER_OF_PACKETS)
            )
            {
                cout << "\nDone Testing!\n";
                break;
            }

            cout << "\nCurrent SimTime: " << hpqSimTime << "\n"; 

            




             //   cout << "packetEnqueuedIndex = " << packetEnqueuedIndex;
            //Enqueue (receive) packet
            if (packetEnqueuedIndex < NUMBER_OF_PACKETS && testCase[packetEnqueuedIndex].getReceivingTimestamp() == hpqSimTime)
            {
            //    cout << "packetEnqueuedIndex = " << packetEnqueuedIndex;


                //Print Packets as they are received
                if (g_dpl_map[testCase[packetEnqueuedIndex].getDomain()] < 100)
                    cout << "Router received a prioritized packet to " << testCase[packetEnqueuedIndex].getDomain() << ", which will take " << testCase[packetEnqueuedIndex].getProcessingTime() <<"ms to process.\n";
                else
                    cout << "Router received an unprioritized packet, which will take " << testCase[packetEnqueuedIndex].getProcessingTime() <<"ms to process.\n";

                h.enHPQ(testCase[packetEnqueuedIndex]);
                packetEnqueuedIndex++;
            }

            
            //Dequeue (process) packet if router is free
            if (hpqSimTime >= nextRouterFreeTimestamp && packetEnqueuedIndex < NUMBER_OF_PACKETS)
            {
                //Packet frontPacket = h.frontHPQ();
                //nextRouterFreeTimestamp += frontPacket.getProcessingTime();
                //h.deHPQ();

                if (!h.isEmpty())
                {
                    Packet p = h.deHPQ();
                    nextRouterFreeTimestamp += p.getProcessingTime();
                    dequeuedPacketsCounter++;
                }
            }

            //Advance SimTime
            hpqSimTime++;
        }

        hpqClockTime = (clock() - hpqClockTimeStart);

        hpqFinalTime = hpqSimTime + hpqClockTime;
        
        cout << "\nHeap-based queue took " << hpqClockTime << " milliseconds in ClockTime to run!\n";
        cout << "Heap-based queue took " << hpqSimTime << " milliseconds in SimTime to run!\n";
        cout << "Heap-based queue took " << hpqFinalTime << " milliseconds in ClockTime + SimTime to run!\n";
    }
   
   
    //Do Array-based PQ Unit Test
    {
        //Times for Heap-based Priority Queue
        int apqClockTime{},
            apqSimTime{},
            apqFinalTime{},
            apqClockTimeStart{clock()};

        //Initialize the APQ
        ArrayPQ a(1000);

        //Helper variables for testing
        int packetEnqueuedIndex{};
        int nextRouterFreeTimestamp{};
        
        while (true)
        {
            //If router:
            //  - Has enqueued all packets in testCase
            //  - Is free 
            //-> Break 
            if ((packetEnqueuedIndex == NUMBER_OF_PACKETS) && (apqSimTime >= nextRouterFreeTimestamp))
                break;

            //Enqueue (receive) packet
            if (testCase[packetEnqueuedIndex].getReceivingTimestamp() == apqSimTime)
            {
                h.enHPQ(testCase[packetEnqueuedIndex]);
                packetEnqueuedIndex++;
            }

            //Dequeue (process) packet if router is free
            if (apqSimTime >= nextRouterFreeTimestamp && packetEnqueuedIndex < NUMBER_OF_PACKETS)
            {
                Packet frontPacket = h.frontHPQ();
                nextRouterFreeTimestamp += frontPacket.getProcessingTime();
                h.deHPQ();
            }

            //Advance SimTime
            apqSimTime++;
        }
        apqClockTime = (clock() - apqClockTimeStart);

        apqFinalTime = apqSimTime + apqClockTime;

        cout << "\nArray-based queue took " << apqClockTime << " milliseconds in ClockTime to run!\n";
        cout << "Array-based queue took " << apqSimTime << " milliseconds in SimTime to run!\n";
        cout << "Array-based queue took " << apqFinalTime << " milliseconds in ClockTime + SimTime to run!\n";
    }

    //cout << "There are " << g_domains_and_priority_level_array.size() << " domains in the g_domains_and_priority_level_array \n";
    cout << "Packet is " << sizeof(Packet) << " bytes! \n";

}
*/

/*
        for (auto i{0uz}; i < testCase.size(); i++)
        {


            hpqSimTime++;
        }

        for (int i{}; i < NUMBER_OF_PACKETS; i++)
        {
            h.enHPQ(testCase[i]) ;
            if (i == 100)
                cout << setw(10) << "Received unprioritized packet! \n";
            else
                cout << setw(10) << "Received prioritized packet to: " << testCase[i].getDomain() << "! \n";
        }

        for (int i{}; i < NUMBER_OF_PACKETS; i++)
        {
            //if (testCase[i].getProcessingTime() < 0)
                //cout << setw(10) << "Wait time was " << testCase[i].getProcessingTime() <<" \n";
                //cout << setw(10) << "Wait time was less than ZERO???\n";

            hpqSimTime += testCase[i].getProcessingTime();
            
            h.deHPQ();
        }
            */