// --- Main Program ---

FUNCTION main():
    SEED the random number generator

    // Set up test cases
    GENERATE a queue of packets called 'testCase'

    // ---------------------------------------------------------
    // TEST 1: Heap-Based Priority Queue (HPQ)
    // ---------------------------------------------------------
        
    IF getOption(user_reply) is True:
        RECORD clock start time
        INITIALIZE hpqSimTime = 0
        INITIALIZE Heap Priority Queue 'h'
        INITIALIZE enqueuedCount = 0, dequeuedCount = 0, freeCounter = 0
        INITIALIZE nextRouterFreeTime = 0
        
        WHILE True:
            // Check termination conditions
            IF MAX_SIM_TIME exceeded
            OR enqueued&dequeued ALL PACKETS AND Router IS FREE
                BREAK loop
                
            PRINT "Current SimTime: " + hpqSimTime
            
            // Phase 1: Receive (Enqueue) Packet
            IF 'hpqTestCase' has packets AND enqueuedCount < MAX_PACKETS AND front packet arrives at hpqSimTime:
                PRINT packet received status (check domain for priority level)
                ENQUEUE front packet into 'h'
                REMOVE front packet from 'hpqTestCase'
                INCREMENT enqueuedCount
                
            // Phase 2: Process (Dequeue) Packet
            IF router is free (hpqSimTime >= nextRouterFreeTime) AND enqueuedCount <= MAX_PACKETS AND 'h' is not empty:
                DEQUEUE packet from 'h'
                PRINT packet processing status
                SET nextRouterFreeTime = hpqSimTime + packet's processing time
                PRINT "Router will be free at: " + nextRouterFreeTime
                INCREMENT dequeuedCount
                
            // Phase 3: Update Router Status
            IF router is free (hpqSimTime >= nextRouterFreeTime):
                INCREMENT freeCounter
                PRINT "Router is free!"
            ELSE:
                PRINT "Router is busy!"
                
            // Advance Simulation Time
            INCREMENT hpqSimTime
            
        CALCULATE final execution times
        PRINT HPQ performance metrics (clock time, sim time, idle time)


    // ---------------------------------------------------------
    // TEST 2: Array-Based Priority Queue (APQ)
    // ---------------------------------------------------------
    
    LOOP:
        PROMPT user: "Do you want to test the Array-Based Priority Queue? [y/N]: "
    UNTIL user provides a valid option
    
    IF getOption(user_reply) is True:
        RECORD clock start time
        INITIALIZE apqSimTime = 0
        INITIALIZE Array Priority Queue 'a'
        INITIALIZE enqueuedCount = 0, dequeuedCount = 0, freeCounter = 0
        INITIALIZE nextRouterFreeTime = 0
        
        WHILE True:
            // Check termination conditions
            IF apqSimTime > MAX_SIM_TIME OR 
               (enqueuedCount >= MAX_PACKETS AND dequeuedCount >= MAX_PACKETS AND router is free):
                PRINT "Done Testing!"
                BREAK loop
                
            PRINT "Current SimTime: " + apqSimTime
            
            // Phase 1: Receive (Enqueue) Packet
            IF 'apqTestCase' has packets AND enqueuedCount < MAX_PACKETS AND front packet arrives at apqSimTime:
                PRINT packet received status (check domain for priority level)
                ENQUEUE front packet into 'a' (using original testCase logic per your code)
                REMOVE front packet from 'apqTestCase'
                INCREMENT enqueuedCount
                
            // Phase 2: Process (Dequeue) Packet
            IF router is free (apqSimTime >= nextRouterFreeTime) AND enqueuedCount <= MAX_PACKETS AND 'a' is not empty:
                DEQUEUE packet from 'a'
                PRINT packet processing status
                SET nextRouterFreeTime = apqSimTime + packet's processing time
                PRINT "Router will be free at: " + nextRouterFreeTime
                INCREMENT dequeuedCount
                
            // Phase 3: Update Router Status
            IF router is free (apqSimTime >= nextRouterFreeTime):
                INCREMENT freeCounter
                PRINT "Router is free!"
            ELSE:
                PRINT "Router is busy!"
                
            // Advance Simulation Time
            INCREMENT apqSimTime
            
        CALCULATE final execution times
        PRINT APQ performance metrics (clock time, sim time, idle time)

    PRINT "That's everything!"
END FUNCTION