#include "main.h"

using namespace std;

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
    srand(seed);

    //Helper variables
    string print_testCase_reply, print_apq_reply, print_hpq_reply;

    //Create an queue to store the test case
    queue<Packet> testCase{};
    Packet::generateTestCase(testCase);
    queue<Packet> hpqTestCase = testCase;
    queue<Packet> apqTestCase = testCase;

    //Ask to print test case
    do {
        std::cout << "Do you want to print the test case? [y/N]: ";
    } while (getline(std::cin, print_testCase_reply) && !optionIsValid(print_testCase_reply));
    if (getOption(print_testCase_reply))
        Packet::printTestCase(testCase);

    //cout << "\nHPQ testCase: \n";
    //Packet::printTestCase(hpqTestCase);
    //cout << "\nAPQ testCase: \n";
    //Packet::printTestCase(apqTestCase);

        
    //Ask whether to print Heap-Based Priority Queue 
    do {
        std::cout << "Do you want to test the Heap-Based Priority Queue? [y/N]: ";
    } while (getline(std::cin, print_hpq_reply) && !optionIsValid(print_hpq_reply));
    if (getOption(print_hpq_reply))
    //Do Heap-based PQ Unit Test
    {
        //Timer variables for Heap-based Priority Queue
        int hpqClockTime{},
            hpqSimTime{},
            hpqFinalTime{},
            hpqClockTimeStart{clock()};

        //Initialize the HPQ
        HeapPQ h(NUMBER_OF_PACKETS);

        //Helper variables for testing
        int enqueuedPacketsCounter{};
        int nextRouterFreeTimestamp{};
        int dequeuedPacketsCounter{};

        //Debug
        int hpqFreeCounter{};


//---------------------------------------------------------------Run test---------------------------------------------------------------//
        while (true)//H
        {
            //If router:
            //  - Exceeds MAX_SIM_TIME
            //  OR 
            //  - Has enqueued all packets in testCase
            //  - Has dequeued NUMBER_OF_PACKETS packets
            //  - Is free 
            //-> Break 
            if ((hpqSimTime > MAX_SIM_TIME) 
             || ((enqueuedPacketsCounter >= NUMBER_OF_PACKETS) 
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

            //Enqueue (receive) packet
            if (!hpqTestCase.empty() 
             && enqueuedPacketsCounter < NUMBER_OF_PACKETS 
             && hpqTestCase.front().getReceivingTimestamp() == hpqSimTime)
            {
                //Print Packets as they are received
                if (g_dpl_map[hpqTestCase.front().getDomain()] < 99)
                    cout << "Router received a prioritized packet to " << hpqTestCase.front().getDomain() << ", which will take " << hpqTestCase.front().getProcessingTime() <<"ms to process.\n";
                else
                    cout << "Router received an unprioritized packet, which will take " << hpqTestCase.front().getProcessingTime() <<"ms to process.\n";

                //Log received packet to HPQ
                h.enHPQ(hpqTestCase.front());

                //Remove received packet from hpqTestCase
                hpqTestCase.pop();

                //Count enqueued Packet
                enqueuedPacketsCounter++;
            }

            //Dequeue (process) packet if router is free
            if (hpqSimTime >= nextRouterFreeTimestamp 
             && enqueuedPacketsCounter <= NUMBER_OF_PACKETS
             && !h.isEmpty())
            {
                Packet p = h.deHPQ();
                string domain = p.getDomain();
                int processingTime = p.getProcessingTime();
                
                if (g_dpl_map[domain] < 99)
                    cout << "Router started processing a prioritized packet to " << domain << ", which will take " << processingTime <<"ms to process.\n";
                else
                    cout << "Router started processing an unprioritized packet, which will take " << processingTime <<"ms to process.\n";
                
                nextRouterFreeTimestamp = hpqSimTime + processingTime;

                cout << "Router will be free at: " << nextRouterFreeTimestamp << "\n";

                dequeuedPacketsCounter++;
            }

            //Check if router is free
            if (hpqSimTime >= nextRouterFreeTimestamp)
            {
                hpqFreeCounter++;
                cout << "Router is free!\n";
            }
            else
                cout << "Router is busy!\n";

            //Advance SimTime
            hpqSimTime++;

            cout << "---------------------------------------------------------------------------------\n";
        }
//---------------------------------------------------------------End test---------------------------------------------------------------//

        hpqClockTime = (clock() - hpqClockTimeStart);

        hpqFinalTime = hpqSimTime + hpqClockTime;
        
        cout << "\nHeap-based queue took " << hpqClockTime << " milliseconds in ClockTime to run!\n";
        cout << "Heap-based queue took " << hpqSimTime << " milliseconds in SimTime to run!\n";
        cout << "Heap-based queue took " << hpqFinalTime << " milliseconds in ClockTime + SimTime to run!\n";
        cout << "Heap-based queue was free for " << hpqFreeCounter << " milliseconds!\n";
    }

    //Ask whether to print Array-Based Priority Queue 
    do {
        std::cout << "Do you want to test the Array-Based Priority Queue? [y/N]: ";
    } while (getline(std::cin, print_apq_reply) && !optionIsValid(print_apq_reply));
    if (getOption(print_apq_reply))
    //Do Array-based PQ Unit Test
    {
        //Times for Array-based Priority Queue
        int apqClockTime{},
            apqSimTime{},
            apqFinalTime{},
            apqClockTimeStart{clock()};

        //Initialize the APQ
        ArrayPQ a(NUMBER_OF_APQ_LEVELS);

        //Helper variables for testing
        int enqueuedPacketsCounter{};
        int nextRouterFreeTimestamp{};
        int dequeuedPacketsCounter{};
        int quota = 5;

        //Debug
        int apqFreeCounter{};
        
//---------------------------------------------------------------Run test---------------------------------------------------------------//
        while (true)//A
        {
            //If router:
            //  - Exceeds MAX_SIM_TIME
            //  OR 
            //  - Has enqueued all packets in testCase
            //  - Has dequeued NUMBER_OF_PACKETS packets
            //  - Is free 
            //-> Break 
            if ((apqSimTime > MAX_SIM_TIME) 
             || ((enqueuedPacketsCounter >= NUMBER_OF_PACKETS) 
             && (dequeuedPacketsCounter >= NUMBER_OF_PACKETS)
             && (apqSimTime >= nextRouterFreeTimestamp))
            )
            {
                cout << "\nDone Testing!\n";
                break;
            }
            cout << "\n---------------------------------------------------------------------------------\n";
            //Print Current Time
            cout << "Current SimTime: " << apqSimTime << "\n"; 

            //Enqueue (receive) packet
            if (!apqTestCase.empty() 
             && enqueuedPacketsCounter < NUMBER_OF_PACKETS 
             && apqTestCase.front().getReceivingTimestamp() == apqSimTime)
            {
                //Print Packets as they are received
                if (g_dpl_map[apqTestCase.front().getDomain()] < 99)
                    cout << "Router received a prioritized packet to " << apqTestCase.front().getDomain() << ", which will take " << apqTestCase.front().getProcessingTime() <<"ms to process.\n";
                else
                    cout << "Router received an unprioritized packet, which will take " << apqTestCase.front().getProcessingTime() <<"ms to process.\n";

                //Log received packet to APQ
                a.enqueuePacket(testCase.front());

                //Remove received packet from apqTestCase
                apqTestCase.pop();

                //Count enqueued Packet
                enqueuedPacketsCounter++;
            }

            //if (quota == 0)
            //    quota = 5;

            //Dequeue (process) packet if router is free //NOT DEQUEUEING
            if (apqSimTime >= nextRouterFreeTimestamp 
             && enqueuedPacketsCounter <= NUMBER_OF_PACKETS
             && !a.isEmpty())
            {
                Packet p = a.dequeuePacket();
                string domain = p.getDomain();
                int processingTime = p.getProcessingTime();

                if (g_dpl_map[domain] < 99)
                    cout << "Router started processing a prioritized packet to " << domain << ", which will take " << processingTime <<"ms to process.\n";
                else
                    cout << "Router started processing an unprioritized packet, which will take " << processingTime <<"ms to process.\n";
                
                nextRouterFreeTimestamp = apqSimTime + processingTime;

                cout << "Router will be free at: " << nextRouterFreeTimestamp << "\n";

                dequeuedPacketsCounter++;
            }

            //Check if router is free
            if (apqSimTime >= nextRouterFreeTimestamp)
            {
                apqFreeCounter++;
                cout << "Router is free!\n";
            }
            else
                cout << "Router is busy!\n";

            //Advance SimTime
            apqSimTime++;

            cout << "---------------------------------------------------------------------------------\n";
        }
//---------------------------------------------------------------End test---------------------------------------------------------------//

        apqClockTime = (clock() - apqClockTimeStart);

        apqFinalTime = apqSimTime + apqClockTime;

        cout << "\nArray-based queue took " << apqClockTime << " milliseconds in ClockTime to run!\n";
        cout << "Array-based queue took " << apqSimTime << " milliseconds in SimTime to run!\n";
        cout << "Array-based queue took " << apqFinalTime << " milliseconds in ClockTime + SimTime to run!\n";
        cout << "Heap-based queue was free for " << apqFreeCounter << " milliseconds!\n";

    }

    //cout << "There are " << g_domains_and_priority_level_array.size() << " domains in the g_domains_and_priority_level_array \n";
    cout << "That's everything!\n "; //<< sizeof(Packet) << " bytes! \n";

}
