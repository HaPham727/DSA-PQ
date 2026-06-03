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

//ClockTime only VERSION
int main()
{
    //Seeding the RNG
    srand(seed);

        //Timer variables for Normal Queue
    int //apqClockTime{},
        nqSimTime{},
        nqFinalTime{};

    //Timer variables for Array-based Priority Queue
    int //apqClockTime{},
        apqSimTime{},
        apqFinalTime{};

    //Timer variables for Heap-based Priority Queue
    int //hpqClockTime{},
        hpqSimTime{},
        hpqFinalTime{};

    //Helper variables
    string print_testCase_reply, print_nq_reply, print_apq_reply, print_hpq_reply, print_comparison_reply;

    //Create an queue to store the test case
    queue<Packet> testCase{};
    Packet::generateTestCase(testCase);

    queue<Packet> nqTestCase = testCase;
    //Format: Domain, WaitTime for that instance
    array<pair<string, int>, NUMBER_OF_PACKETS> nqWaitTime{}; 
    //Format: Number of Times showed up, Total WaitTime
    array<pair<double, double>, NUMBER_OF_PRIORITIZED_DOMAINS + 1> nqAvgWaitTime{}; 

    queue<Packet> apqTestCase = testCase;
    //Format: Domain, WaitTime for that instance
    array<pair<string, int>, NUMBER_OF_PACKETS> apqWaitTime{}; 
    //Format: Number of Times showed up, Total WaitTime
    array<pair<double, double>, NUMBER_OF_PRIORITIZED_DOMAINS + 1> apqAvgWaitTime{}; 

    queue<Packet> hpqTestCase = testCase;
    //Format: Domain, WaitTime for that instance
    array<pair<string, int>, NUMBER_OF_PACKETS> hpqWaitTime{}; 
    //Format: Number of Times showed up, Total WaitTime
    array<pair<double, double>, NUMBER_OF_PRIORITIZED_DOMAINS + 1> hpqAvgWaitTime{}; 

    
    //Ask to print test case
    do {
        cout << "Do you want to print out the test case? [y/N]: ";
    } while (getline(cin, print_testCase_reply) && !optionIsValid(print_testCase_reply));
    if (getOption(print_testCase_reply))
        Packet::printTestCase(testCase);
        
    //Ask to test & print Normal Queue 
    do {
        cout << "\nDo you want to run the Normal Queue? [y/N]: ";
    } while (getline(std::cin, print_nq_reply) && !optionIsValid(print_nq_reply));
    if (getOption(print_nq_reply))
    {
        //Start clock for nq test
        int nqClockTimeStart{clock()};

        //Initialize the NQ
        queue<Packet> n{};

        //Helper variables for testing
        int enqueuedPacketsCounter{};
        int nextRouterFreeTimestamp{};
        int dequeuedPacketsCounter{};

        //Debug
        int nqFreeCounter{};
        
        //---------------------------------------------------------------Run test---------------------------------------------------------------//
        while (true)//N
        {
            nqSimTime = clock() - nqClockTimeStart;

            //If router:
            //  - Exceeds MAX_SIM_TIME
            //  OR 
            //  - Has enqueued all packets in testCase
            //  - Has dequeued NUMBER_OF_PACKETS packets
            //  - Is free 
            //-> Break 
            if ((nqSimTime > MAX_SIM_TIME) 
             || ((enqueuedPacketsCounter >= NUMBER_OF_PACKETS) 
             && (dequeuedPacketsCounter >= NUMBER_OF_PACKETS)
             && (nqSimTime >= nextRouterFreeTimestamp))
            )
            {
                cout << "\n---------------------------------------------------------------------------------\n";
                cout << "Current SimTime: " << nqSimTime << "\n"; 
                cout << "Router is free!\n";
                cout << "---------------------------------------------------------------------------------\n";
                cout << "\nArray-based Priority Queue is done testing!\n";
                break;
            }

            cout << "\n---------------------------------------------------------------------------------\n";
            //Print Current Time
            cout << "Current SimTime: " << nqSimTime << "\n"; 

            //Enqueue (receive) packet
            if (!nqTestCase.empty() 
             && enqueuedPacketsCounter < NUMBER_OF_PACKETS 
             && nqTestCase.front().getReceivingTimestamp() <= /*==*/ nqSimTime)
            {
                //Log Domain and WaitTime start for each packet
                nqWaitTime[enqueuedPacketsCounter].first = nqTestCase.front().getDomain();
                nqWaitTime[enqueuedPacketsCounter].second = 0 - clock() ;//+ apqTestCase.front().getProcessingTime();

                //Print Packets as they are received
                if (g_dpl_map[nqTestCase.front().getDomain()] < 99)
                    cout << "Router received a prioritized packet to " << nqTestCase.front().getDomain() << ", which will take " << nqTestCase.front().getProcessingTime() <<"ms to process.\n";
                else
                    cout << "Router received an unprioritized packet, which will take " << nqTestCase.front().getProcessingTime() <<"ms to process.\n";

                //Log received packet to NQ
                n.push(nqTestCase.front());

                //Remove received packet from nqTestCase
                nqTestCase.pop();

                //Count enqueued packets
                enqueuedPacketsCounter++;
            }

            //Dequeue (process) packet if router is free 
            if (nqSimTime >= nextRouterFreeTimestamp 
             //&& enqueuedPacketsCounter <= NUMBER_OF_PACKETS
             && !n.empty())
            {
                Packet p = n.front();
                n.pop();
                string domain = p.getDomain();
                int processingTime = p.getProcessingTime();

                if (g_dpl_map[domain] < 99)
                    cout << "Router started processing a prioritized packet to " << domain << ", which will take " << processingTime <<"ms to process.\n";
                else
                    cout << "Router started processing an unprioritized packet, which will take " << processingTime <<"ms to process.\n";
                
                nextRouterFreeTimestamp = nqSimTime + processingTime;

                cout << "Router will be free at: " << nextRouterFreeTimestamp << "\n";

                //Calculate WaitTime
                nqWaitTime[dequeuedPacketsCounter].second += clock();

                //Count dequeued packets
                dequeuedPacketsCounter++;
            }

            //Check if router is free
            if (nqSimTime >= nextRouterFreeTimestamp)
            {
                nqFreeCounter++;
                cout << "Router is free!\n";
            }
            else
                cout << "Router is busy!\n";

            //Advance SimTime
            //apqSimTime++;

            cout << "---------------------------------------------------------------------------------\n";
        }
        //---------------------------------------------------------------End test---------------------------------------------------------------//

        cout << "\nNormal queue took " << nqSimTime << " milliseconds to run!\n";
        cout << "Normal queue was free for " << nqFreeCounter << " milliseconds!\n";
    }


    //Ask to test & print Array-based Priority Queue 
    do {
        cout << "\nDo you want to run the Array-based Priority Queue? [y/N]: ";
    } while (getline(std::cin, print_apq_reply) && !optionIsValid(print_apq_reply));
    if (getOption(print_apq_reply))
    {
        //Start clock for apq test
        int apqClockTimeStart{clock()};

        //Initialize the APQ
        int capacity = ((NUMBER_OF_PRIORITIZED_DOMAINS + 1) / DOMAINS_PER_APQ_LEVEL) + 1;
        ArrayPQ a(capacity);

        //Helper variables for testing
        int enqueuedPacketsCounter{};
        int nextRouterFreeTimestamp{};
        int dequeuedPacketsCounter{};
        //int quota = 5;

        //Debug
        int apqFreeCounter{};
        
        //---------------------------------------------------------------Run test---------------------------------------------------------------//
        while (true)//A
        {
            apqSimTime = clock() - apqClockTimeStart;

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
                cout << "\n---------------------------------------------------------------------------------\n";
                cout << "Current SimTime: " << apqSimTime << "\n"; 
                cout << "Router is free!\n";
                cout << "---------------------------------------------------------------------------------\n";
                cout << "\nArray-based Priority Queue is done testing!\n";
                break;
            }

            cout << "\n---------------------------------------------------------------------------------\n";
            //Print Current Time
            cout << "Current SimTime: " << apqSimTime << "\n"; 

            //Enqueue (receive) packet
            if (!apqTestCase.empty() 
             && enqueuedPacketsCounter < NUMBER_OF_PACKETS 
             && apqTestCase.front().getReceivingTimestamp() <= /*==*/ apqSimTime)
            {
                //Log Domain and WaitTime start for each packet
                apqWaitTime[enqueuedPacketsCounter].first = apqTestCase.front().getDomain();
                apqWaitTime[enqueuedPacketsCounter].second = 0 - clock() ;//+ apqTestCase.front().getProcessingTime();

                //Print Packets as they are received
                if (g_dpl_map[apqTestCase.front().getDomain()] < 99)
                    cout << "Router received a prioritized packet to " << apqTestCase.front().getDomain() << ", which will take " << apqTestCase.front().getProcessingTime() <<"ms to process.\n";
                else
                    cout << "Router received an unprioritized packet, which will take " << apqTestCase.front().getProcessingTime() <<"ms to process.\n";

                //Log received packet to APQ
                a.enqueuePacket(apqTestCase.front());

                //Remove received packet from apqTestCase
                apqTestCase.pop();

                //Count enqueued packets
                enqueuedPacketsCounter++;
            }

            //Dequeue (process) packet if router is free 
            if (apqSimTime >= nextRouterFreeTimestamp 
             //&& enqueuedPacketsCounter <= NUMBER_OF_PACKETS
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

                //Calculate WaitTime
                apqWaitTime[dequeuedPacketsCounter].second += clock();

                //Count dequeued packets
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
            //apqSimTime++;

            cout << "---------------------------------------------------------------------------------\n";
        }
        //---------------------------------------------------------------End test---------------------------------------------------------------//

        cout << "\nArray-based queue took " << apqSimTime << " milliseconds to run!\n";
        cout << "Array-based queue was free for " << apqFreeCounter << " milliseconds!\n";
    }

    //Ask to test & print Heap-based Priority Queue 
    do {
        cout << "\nDo you want to run the Heap-based Priority Queue? [y/N]: ";
    } while (getline(cin, print_hpq_reply) && !optionIsValid(print_hpq_reply));
    if (getOption(print_hpq_reply))
    {
        //Start clock for hpq test
        int hpqClockTimeStart{clock()};

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
            hpqSimTime = clock() - hpqClockTimeStart;
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
                cout << "\n---------------------------------------------------------------------------------\n";
                cout << "Current SimTime: " << hpqSimTime << "\n"; 
                cout << "Router is free!\n";
                cout << "---------------------------------------------------------------------------------\n";
                cout << "\nHeap-based Priority Queue is done testing!\n";
                break;
            }

            cout << "\n---------------------------------------------------------------------------------\n";
            //Print Current Time
            cout << "Current SimTime: " << hpqSimTime << "\n"; 

            //Enqueue (receive) packet
            if (!hpqTestCase.empty() 
             && enqueuedPacketsCounter < NUMBER_OF_PACKETS 
             && hpqTestCase.front().getReceivingTimestamp() <= hpqSimTime)
            {
                //Log Domain and WaitTime start for each packet
                hpqWaitTime[enqueuedPacketsCounter].first = hpqTestCase.front().getDomain();
                hpqWaitTime[enqueuedPacketsCounter].second = 0 - clock() ;//+ hpqTestCase.front().getProcessingTime();

                //Print Packets as they are received
                if (g_dpl_map[hpqTestCase.front().getDomain()] < 99)
                    cout << "Router received a prioritized packet to " << hpqTestCase.front().getDomain() << ", which will take " << hpqTestCase.front().getProcessingTime() <<"ms to process.\n";
                else
                    cout << "Router received an unprioritized packet, which will take " << hpqTestCase.front().getProcessingTime() <<"ms to process.\n";

                //Log received packet to HPQ
                h.enHPQ(hpqTestCase.front());

                //Remove received packet from hpqTestCase
                hpqTestCase.pop();

                //Count enqueued packets
                enqueuedPacketsCounter++;
            }

            //Dequeue (process) packet if router is free
            if (hpqSimTime >= nextRouterFreeTimestamp 
             //&& enqueuedPacketsCounter <= NUMBER_OF_PACKETS
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

                //Calculate WaitTime
                hpqWaitTime[dequeuedPacketsCounter].second += clock();

                //Count dequeued packets
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
            //hpqSimTime++;

            cout << "---------------------------------------------------------------------------------\n";
        }
        //---------------------------------------------------------------End test---------------------------------------------------------------//

        cout << "\nHeap-based queue took " << hpqSimTime << " milliseconds to run!\n";
        cout << "Heap-based queue was free for " << hpqFreeCounter << " milliseconds!\n";
    }








    //---------------------------------------------------------------Run Comparison---------------------------------------------------------------//
    if (getOption(print_apq_reply) && getOption(print_hpq_reply))
    {    
        string print_nqWaitTime_reply, print_apqWaitTime_reply, print_hpqWaitTime_reply;
        
        for (int i{}; i < NUMBER_OF_PACKETS; i++)
        {
            //cout << setw(30) << apqWaitTime[i].first << ": " << setw(5) << apqWaitTime[i].second << "\n";
            nqAvgWaitTime[g_dpl_map[nqWaitTime[i].first]].first += 1;
            nqAvgWaitTime[g_dpl_map[nqWaitTime[i].first]].second += static_cast<double>(nqWaitTime[i].second);
        }

        for (int i{}; i < NUMBER_OF_PACKETS; i++)
        {
            //cout << setw(30) << apqWaitTime[i].first << ": " << setw(5) << apqWaitTime[i].second << "\n";
            apqAvgWaitTime[g_dpl_map[apqWaitTime[i].first]].first += 1;
            apqAvgWaitTime[g_dpl_map[apqWaitTime[i].first]].second += static_cast<double>(apqWaitTime[i].second);
        }

        for (int i{}; i < NUMBER_OF_PACKETS; i++)
        {
            //cout << setw(30) << hpqWaitTime[i].first << ": " << setw(5) << hpqWaitTime[i].second << "\n";
            hpqAvgWaitTime[g_dpl_map[hpqWaitTime[i].first]].first += 1;
            hpqAvgWaitTime[g_dpl_map[hpqWaitTime[i].first]].second += static_cast<double>(hpqWaitTime[i].second);
        }
        
        do {
            cout << "\nDo you want to print out the average WaitTimes of individual domains for Normal Queue? [y/N]: ";
        } while (getline(cin, print_apqWaitTime_reply) && !optionIsValid(print_apqWaitTime_reply));
        if (getOption(print_apqWaitTime_reply))
        {
            cout << "\nNormal Queue Performance:\n";
            cout << "|" << setw(17) << "Domain" << setw(13) << "|" << "Priority Rank (1-100)" << "|" << "Average Processing Time" << "|" << "Instances" << "|\n";
            for (int i{}; i < nqAvgWaitTime.size(); i++)
            {
                //If domain showed up at all
                if (nqAvgWaitTime[i].first != 0)
                    //Print Domain name and Average WaitTime
                    //cout << setw(30) << g_dpl_arr[i] << " gets processed (in ClockTime, on average) in" << setw(10) << ((apqAvgWaitTime[i].second) / (apqAvgWaitTime[i].first)) << "ms throughout " << (apqAvgWaitTime[i].first) << " packets! \n";
                    //cout << "|" << setw(29) << g_dpl_arr[i] << "|"  <<  setw(10) << ((g_dpl_map[g_dpl_arr[i]] != 99) ? ((g_dpl_map[g_dpl_arr[i]] / 20) + 1) : 6 ) << setw(11) << "|" << setw(23) << std::fixed << std::setprecision(2) << ((nqAvgWaitTime[i].second) / (nqAvgWaitTime[i].first)) << "|" << setw(9) << std::fixed << std::setprecision(0) << (nqAvgWaitTime[i].first) << "| \n";
                    cout << "|" << setw(29) << g_dpl_arr[i] << "|"  <<  setw(11) << g_dpl_map[g_dpl_arr[i]]  << setw(11) << "|" << setw(23) << std::fixed << std::setprecision(2) << ((nqAvgWaitTime[i].second) / (nqAvgWaitTime[i].first)) << "|" << setw(9) << std::fixed << std::setprecision(0) << (nqAvgWaitTime[i].first) << "| \n";
    
            }
        }

        
        do {
            cout << "\nDo you want to print out the average WaitTimes of individual domains for Array-based Priority Queue? [y/N]: ";
        } while (getline(cin, print_apqWaitTime_reply) && !optionIsValid(print_apqWaitTime_reply));
        if (getOption(print_apqWaitTime_reply))
        {
            cout << "\nArray-based Priority Queue Performance:\n";

            cout << "|" << setw(17) << "Domain" << setw(13) << "|" << "Priority Level (1-6)" << "|" << "Average Processing Time" << "|" << "Instances" << "|\n";
            for (int i{}; i < apqAvgWaitTime.size(); i++)
            {
                
                //If domain showed up at all
                if (apqAvgWaitTime[i].first != 0)
                    //Print Domain name and Average WaitTime
                    //cout << setw(30) << g_dpl_arr[i] << " gets processed (in ClockTime, on average) in" << setw(10) << ((apqAvgWaitTime[i].second) / (apqAvgWaitTime[i].first)) << "ms throughout " << (apqAvgWaitTime[i].first) << " packets! \n";
                    cout << "|" << setw(29) << g_dpl_arr[i] << "|"  <<  setw(10) << ((g_dpl_map[g_dpl_arr[i]] != 99) ? ((g_dpl_map[g_dpl_arr[i]] / 20) + 1) : 6 ) << setw(11) << "|" << setw(23) << std::fixed << std::setprecision(2) << ((apqAvgWaitTime[i].second) / (apqAvgWaitTime[i].first)) << "|" << setw(9) << std::fixed << std::setprecision(0) << (apqAvgWaitTime[i].first) << "| \n";
            }
        }

        do {
            cout << "\nDo you want to print out the average WaitTimes of individual domains for Heap-based Priority Queue? [y/N]: ";
        } while (getline(cin, print_hpqWaitTime_reply) && !optionIsValid(print_hpqWaitTime_reply));
        if (getOption(print_hpqWaitTime_reply))
        {
            cout << "\nHeap-based Priority Queue Performance:\n";

            cout << "|" << setw(17) << "Domain" << setw(13) << "|" << "Priority Rank (1-100)" << "|" << "Average Processing Time" << "|" << "Instances" << "|\n";
            for (int i{}; i < hpqAvgWaitTime.size(); i++)
            {
                //If domain showed up at all
                if (hpqAvgWaitTime[i].first != 0)
                    //Print Domain name and Average WaitTime
                    //cout << setw(30) << g_dpl_arr[i] << " gets processed (in ClockTime, on average) in" << setw(10) << ((hpqAvgWaitTime[i].second) / (hpqAvgWaitTime[i].first)) << "ms throughout " << (hpqAvgWaitTime[i].first) << " packets! \n";
                    cout << "|" << setw(29) << g_dpl_arr[i] << "|"  <<  setw(11) << g_dpl_map[g_dpl_arr[i]]  << setw(11) << "|" << setw(23) << std::fixed << std::setprecision(2) << ((hpqAvgWaitTime[i].second) / (hpqAvgWaitTime[i].first)) << "|" << setw(9) << std::fixed << std::setprecision(0) << (hpqAvgWaitTime[i].first) << "| \n";
            }
        }
        
        
        do {
            cout << "\nDo you want to print a comparison between the queues? [y/N]: ";
        } while (getline(cin, print_comparison_reply) && !optionIsValid(print_comparison_reply));
        if (getOption(print_comparison_reply))
        {
            //cout << "|" << setw(17) << "Domain" << setw(13) << "|" << " Array-based Priority Queue " << "|" << " Heap-based Priority Queue " << "|" << " Difference " << "|\n";
            cout << "|" << setw(17) << "Domain" << setw(13) << "|" << " Normal Queue " << "|" << " Array-based Priority Queue " << "|" << " Heap-based Priority Queue " << "|\n";

            //cout << "-----------------------------------------------------------------------------------------------------\n";
            cout << "-------------------------------------------------------------------------------------------------------\n";
            for (int i{}; i < NUMBER_OF_PRIORITIZED_DOMAINS + 1; i++)
            {
                //If domain showed up at all
                if (nqAvgWaitTime[i].first != 0 && apqAvgWaitTime[i].first != 0 && hpqAvgWaitTime[i].first != 0)
                {
                    double nqAvgProcessingTime  {(nqAvgWaitTime[i].second) / (nqAvgWaitTime[i].first)};
                    double apqAvgProcessingTime {(apqAvgWaitTime[i].second) / (apqAvgWaitTime[i].first)};
                    double hpqAvgProcessingTime {(hpqAvgWaitTime[i].second) / (hpqAvgWaitTime[i].first)};

                    cout << "|" << setw(29) << g_dpl_arr[i] << "|";

                    if (nqAvgProcessingTime <= apqAvgProcessingTime && nqAvgProcessingTime <= hpqAvgProcessingTime)
                        cout << setw(10) << "Fastest" << setw(5) << "|";
                    else
                        cout << setw(10) << "       " << setw(5) << "|";

                    if (apqAvgProcessingTime <= hpqAvgProcessingTime && apqAvgProcessingTime <= nqAvgProcessingTime)
                        cout << setw(17) << "Fastest" << setw(12) << "|";
                    else
                        cout << setw(17) << "       " << setw(12) << "|";

                    if (hpqAvgProcessingTime <= nqAvgProcessingTime && hpqAvgProcessingTime <= apqAvgProcessingTime)
                        cout << setw(17) << "Fastest" << setw(11) << "|";
                    else
                        cout << setw(17) << "       " << setw(11) << "|";
                    
                    cout << "\n";

                    //Print Comparison
                    //if (apqAvgProcessingTime < hpqAvgProcessingTime)
                    //    cout << "|" << setw(29) << g_dpl_arr[i] << "|"  <<  setw(18) << "Faster" << setw(11) << "|" <<  setw(17) << "      " << setw(11) << "|" << setw(12) << std::fixed << std::setprecision(2) << (hpqAvgProcessingTime - apqAvgProcessingTime) << "| \n";
                    //else if (apqAvgProcessingTime > hpqAvgProcessingTime)
                    //    cout << "|" << setw(29) << g_dpl_arr[i] << "|"  <<  setw(18) << "      " << setw(11) << "|" <<  setw(17) << "Faster" << setw(11) << "|" << setw(12) << std::fixed << std::setprecision(2) << (apqAvgProcessingTime - hpqAvgProcessingTime) << "| \n";
                    //else
                    //    cout << "|" << setw(29) << g_dpl_arr[i] << "|"  <<  setw(17) << "Equal" << setw(12) << "|" <<  setw(15) << "Equal" << setw(13) << "|" << setw(12) << std::fixed << std::setprecision(2) << 0 << "| \n";
                

                    //Print Comparison
                    //if (apqAvgProcessingTime < hpqAvgProcessingTime)
                    //    cout << "|" << setw(29) << g_dpl_arr[i] << "|"  <<  setw(18) << "Faster" << setw(11) << "|" <<  setw(17) << "      " << setw(11) << "|" << setw(12) << std::fixed << std::setprecision(2) << (hpqAvgProcessingTime - apqAvgProcessingTime) << "| \n";
                    //else if (apqAvgProcessingTime > hpqAvgProcessingTime)
                    //    cout << "|" << setw(29) << g_dpl_arr[i] << "|"  <<  setw(18) << "      " << setw(11) << "|" <<  setw(17) << "Faster" << setw(11) << "|" << setw(12) << std::fixed << std::setprecision(2) << (apqAvgProcessingTime - hpqAvgProcessingTime) << "| \n";
                    //else
                    //    cout << "|" << setw(29) << g_dpl_arr[i] << "|"  <<  setw(17) << "Equal" << setw(12) << "|" <<  setw(15) << "Equal" << setw(13) << "|" << setw(12) << std::fixed << std::setprecision(2) << 0 << "| \n";
                }
            }
            //cout << "-----------------------------------------------------------------------------------------------------\n";
            cout << "-------------------------------------------------------------------------------------------------------\n";

            cout << "|" << setw(16) << "Total"  << setw(14) << "|";

            if (nqSimTime <= apqSimTime && nqSimTime <= hpqSimTime)
                cout << setw(10) << "Fastest" << setw(5) << "|";
            else
                cout << setw(10) << "       " << setw(5) << "|";

            if (apqSimTime <= nqSimTime && apqSimTime <= hpqSimTime)
                cout << setw(17) << "Fastest" << setw(12) << "|";
            else
                cout << setw(17) << "       " << setw(12) << "|";

            if (hpqSimTime <= nqSimTime && hpqSimTime <= apqSimTime)
                cout << setw(17) << "Fastest" << setw(11) << "|";
            else
                cout << setw(17) << "       " << setw(11) << "|";
            
            cout << "\n";

   
            //if (apqSimTime < hpqSimTime)
            //    cout << "|" << setw(16) << "Total" << setw(14) << "|"  <<  setw(18) << "Faster" << setw(11) << "|" <<  setw(17) << "      " << setw(11) << "|" << setw(12) << std::fixed << std::setprecision(2) << (hpqSimTime - apqSimTime) << "| \n";
            //else if (apqSimTime > hpqSimTime)
            //    cout << "|" << setw(16) << "Total" << setw(14) << "|"  <<  setw(18) << "      " << setw(11) << "|" <<  setw(17) << "Faster" << setw(11) << "|" << setw(12) << std::fixed << std::setprecision(2) << (apqSimTime - hpqSimTime) << "| \n";
            //else
            //    cout << "|" << setw(16) << "Total" << setw(14) << "|"  <<  setw(17) << "Equal" << setw(12) << "|" <<  setw(15) << "Equal" << setw(13) << "|" << setw(12) << std::fixed << std::setprecision(2) << 0 << "| \n";
        }
        
    }
    //---------------------------------------------------------------End Comparison---------------------------------------------------------------//
   
    cout << "\nThat's everything!\n "; //<< sizeof(Packet) << " bytes! \n";

}

/*
//Sim Time Test Case VERSION
int main()
{
    //Seeding the RNG
    srand(seed);



    //Timer variables for Array-based Priority Queue
    int apqClockTime{},
        apqSimTime{},
        apqFinalTime{};

    //Timer variables for Heap-based Priority Queue
    int hpqClockTime{},
        hpqSimTime{},
        hpqFinalTime{};

    //Helper variables
    string print_testCase_reply, print_apq_reply, print_hpq_reply, print_comparison_reply;

    //Create an queue to store the test case
    queue<Packet> testCase{};
    Packet::generateTestCase(testCase);

    queue<Packet> apqTestCase = testCase;
    //Format: Domain, WaitTime for that instance
    array<pair<string, int>, NUMBER_OF_PACKETS> apqWaitTime{}; 
    //Format: Number of Times showed up, Total WaitTime
    array<pair<double, double>, NUMBER_OF_PRIORITIZED_DOMAINS + 1> apqAvgWaitTime{}; 

    queue<Packet> hpqTestCase = testCase;
    //Format: Domain, WaitTime for that instance
    array<pair<string, int>, NUMBER_OF_PACKETS> hpqWaitTime{}; 
    //Format: Number of Times showed up, Total WaitTime
    array<pair<double, double>, NUMBER_OF_PRIORITIZED_DOMAINS + 1> hpqAvgWaitTime{}; 

    
    //Ask to print test case
    do {
        cout << "Do you want to print the test case? [y/N]: ";
    } while (getline(cin, print_testCase_reply) && !optionIsValid(print_testCase_reply));
    if (getOption(print_testCase_reply))
        Packet::printTestCase(testCase);
        
    //Ask to test & print Array-based Priority Queue 
    do {
        cout << "\nDo you want to run the Array-based Priority Queue? [y/N]: ";
    } while (getline(std::cin, print_apq_reply) && !optionIsValid(print_apq_reply));
    if (getOption(print_apq_reply))
    {
        //Start clock for apq test
        int apqClockTimeStart{clock()};

        //Initialize the APQ
        ArrayPQ a(NUMBER_OF_APQ_LEVELS);

        //Helper variables for testing
        int enqueuedPacketsCounter{};
        int nextRouterFreeTimestamp{};
        int dequeuedPacketsCounter{};
        //int quota = 5;

        //Debug
        int apqFreeCounter{};
        
        //---------------------------------------------------------------Run test---------------------------------------------------------------//
        while (true)//A
        {
            apqSimTime = clock() - apqClockTimeStart;
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
                cout << "\n---------------------------------------------------------------------------------\n";
                cout << "Current SimTime: " << apqSimTime << "\n"; 
                cout << "Router is free!\n";
                cout << "---------------------------------------------------------------------------------\n";
                cout << "\nArray-based Priority Queue is done testing!\n";
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
                //Log Domain and WaitTime start for each packet
                apqWaitTime[enqueuedPacketsCounter].first = apqTestCase.front().getDomain();
                apqWaitTime[enqueuedPacketsCounter].second = 0 - clock() ;//+ apqTestCase.front().getProcessingTime();

                //Print Packets as they are received
                if (g_dpl_map[apqTestCase.front().getDomain()] < 99)
                    cout << "Router received a prioritized packet to " << apqTestCase.front().getDomain() << ", which will take " << apqTestCase.front().getProcessingTime() <<"ms to process.\n";
                else
                    cout << "Router received an unprioritized packet, which will take " << apqTestCase.front().getProcessingTime() <<"ms to process.\n";

                //Log received packet to APQ
                a.enqueuePacket(testCase.front());

                //Remove received packet from apqTestCase
                apqTestCase.pop();

                //Count enqueued packets
                enqueuedPacketsCounter++;
            }

            //if (quota == 0)
            //    quota = 5;

            //Dequeue (process) packet if router is free 
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

                //Calculate WaitTime
                apqWaitTime[dequeuedPacketsCounter].second += clock();

                //Count dequeued packets
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
            //apqSimTime++;

            cout << "---------------------------------------------------------------------------------\n";
        }
        //---------------------------------------------------------------End test---------------------------------------------------------------//

        apqClockTime = (clock() - apqClockTimeStart);

        apqFinalTime = apqSimTime + apqClockTime;

        cout << "\nArray-based queue took " << apqClockTime << " milliseconds in ClockTime to run!\n";
        cout << "Array-based queue took " << apqSimTime << " milliseconds in SimTime to run!\n";
        cout << "Array-based queue took " << apqFinalTime << " milliseconds in ClockTime + SimTime to run!\n";
        cout << "Array-based queue was free for " << apqFreeCounter << " milliseconds!\n";
    }

    //Ask to test & print Heap-based Priority Queue 
    do {
        cout << "\nDo you want to run the Heap-based Priority Queue? [y/N]: ";
    } while (getline(cin, print_hpq_reply) && !optionIsValid(print_hpq_reply));
    if (getOption(print_hpq_reply))
    {
        //Start clock for hpq test
        int hpqClockTimeStart{clock()};

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
                cout << "\n---------------------------------------------------------------------------------\n";
                cout << "Current SimTime: " << apqSimTime << "\n"; 
                cout << "Router is free!\n";
                cout << "---------------------------------------------------------------------------------\n";
                cout << "\nHeap-based Priority Queue is done testing!\n";
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
                //Log Domain and WaitTime start for each packet
                hpqWaitTime[enqueuedPacketsCounter].first = hpqTestCase.front().getDomain();
                hpqWaitTime[enqueuedPacketsCounter].second = 0 - clock() ;//+ hpqTestCase.front().getProcessingTime();

                //Print Packets as they are received
                if (g_dpl_map[hpqTestCase.front().getDomain()] < 99)
                    cout << "Router received a prioritized packet to " << hpqTestCase.front().getDomain() << ", which will take " << hpqTestCase.front().getProcessingTime() <<"ms to process.\n";
                else
                    cout << "Router received an unprioritized packet, which will take " << hpqTestCase.front().getProcessingTime() <<"ms to process.\n";

                //Log received packet to HPQ
                h.enHPQ(hpqTestCase.front());

                //Remove received packet from hpqTestCase
                hpqTestCase.pop();

                //Count enqueued packets
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

                //Calculate WaitTime
                hpqWaitTime[dequeuedPacketsCounter].second += clock();

                //Count dequeued packets
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








    //---------------------------------------------------------------Run Comparison---------------------------------------------------------------//
    if (getOption(print_apq_reply) && getOption(print_hpq_reply))
    {    
        string print_apqWaitTime_reply, print_hpqWaitTime_reply;
        
        
        do {
            cout << "\nDo you want to print out the average WaitTimes of individual domains for Array-based Priority Queue? [y/N]: ";
        } while (getline(cin, print_apqWaitTime_reply) && !optionIsValid(print_apqWaitTime_reply));
        if (getOption(print_apqWaitTime_reply))
        {
            cout << "\nArray-based Priority Queue Performance:\n";

            for (int i{}; i < NUMBER_OF_PACKETS; i++)
            {
                //cout << setw(30) << apqWaitTime[i].first << ": " << setw(5) << apqWaitTime[i].second << "\n";
                apqAvgWaitTime[g_dpl_map[apqWaitTime[i].first]].first += 1;
                apqAvgWaitTime[g_dpl_map[apqWaitTime[i].first]].second += static_cast<double>(apqWaitTime[i].second);
            }

            for (int i{}; i < apqAvgWaitTime.size(); i++)
            {
                //If domain showed up at all
                if (apqAvgWaitTime[i].first != 0)
                    //Print Domain name and Average WaitTime
                    cout << setw(30) << g_dpl_arr[i] << " gets processed (in ClockTime, on average) in" << setw(10) << ((apqAvgWaitTime[i].second) / (apqAvgWaitTime[i].first)) << "ms throughout " << (apqAvgWaitTime[i].first) << " packets! \n";
            }
        }

        do {
            cout << "\nDo you want to print out the average WaitTimes of individual domains for Heap-based Priority Queue? [y/N]: ";
        } while (getline(cin, print_hpqWaitTime_reply) && !optionIsValid(print_hpqWaitTime_reply));
        if (getOption(print_hpqWaitTime_reply))
        {
            cout << "\nHeap-based Priority Queue Performance:\n";

            for (int i{}; i < NUMBER_OF_PACKETS; i++)
            {
                //cout << setw(30) << hpqWaitTime[i].first << ": " << setw(5) << hpqWaitTime[i].second << "\n";
                hpqAvgWaitTime[g_dpl_map[hpqWaitTime[i].first]].first += 1;
                hpqAvgWaitTime[g_dpl_map[hpqWaitTime[i].first]].second += static_cast<double>(hpqWaitTime[i].second);
            }

            for (int i{}; i < hpqAvgWaitTime.size(); i++)
            {
                //If domain showed up at all
                if (hpqAvgWaitTime[i].first != 0)
                    //Print Domain name and Average WaitTime
                    cout << setw(30) << g_dpl_arr[i] << " gets processed (in ClockTime, on average) in" << setw(10) << ((hpqAvgWaitTime[i].second) / (hpqAvgWaitTime[i].first)) << "ms throughout " << (hpqAvgWaitTime[i].first) << " packets! \n";
            }
        }

        do {
            cout << "\nDo you want to print a comparison between the 2 queues? [y/N]: ";
        } while (getline(cin, print_comparison_reply) && !optionIsValid(print_comparison_reply));
        if (getOption(print_comparison_reply))
        {


            cout << "\nArray-based queue took " << apqClockTime << " milliseconds in ClockTime to run!\n";
            cout << "Array-based queue took " << apqSimTime << " milliseconds in SimTime to run!\n";
            cout << "Array-based queue took " << apqFinalTime << " milliseconds in ClockTime + SimTime to run!\n";

            cout << "\nHeap-based queue took " << hpqClockTime << " milliseconds in ClockTime to run!\n";
            cout << "Heap-based queue took " << hpqSimTime << " milliseconds in SimTime to run!\n";
            cout << "Heap-based queue took " << hpqFinalTime << " milliseconds in ClockTime + SimTime to run!\n";
        }
    }
    //---------------------------------------------------------------End Comparison---------------------------------------------------------------//
   
    cout << "\nThat's everything!\n "; //<< sizeof(Packet) << " bytes! \n";

}
*/