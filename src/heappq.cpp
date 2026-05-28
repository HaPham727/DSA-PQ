#include "heappq.h"

using namespace std;

//Constructor that sets the heap size and capacity
HeapPQ::HeapPQ(int capacity)
{
    m_size = 0;
    this->m_capacity = capacity;
    m_heap.resize(capacity);
}

//Recursive function to maintain structure
void HeapPQ::heapify(int i)
{
    // Set initial conditions
    int l = leftIndex(i);
    int r = rightIndex(i);
    int smallest = i;

    // Find the smallest element of the three: node at i, its left or its right child
    if ((l < m_size) && (m_heap[l].getPriorityLevel() < m_heap[smallest].getPriorityLevel()))
    {
        smallest = l;
    }
    if ((r < m_size) && (m_heap[r].getPriorityLevel() < m_heap[smallest].getPriorityLevel()))
    {
        smallest = r;
    }

    // If the smallest of l or r, continue heapify
    if (smallest != i)
    {
        swap(m_heap[i], m_heap[smallest]);
        heapify(smallest);
    }
}

//Check if HeapPQ is empty
bool HeapPQ::isEmpty()
{
    if (m_size == 0)
    {
        //cout << "Dealing with EMPTY HEAP!\n";
        return true;
    }
    else 
        return false;
}

//Enqueue Packet into the HeapPQ
void HeapPQ::enHPQ(Packet k)
{
    // Make sure there is still space in the heap
    if (m_size == m_capacity)
    {
        cout << "MIN HEAP FULL!" << endl;
        return;
    }

    // Increse the amount of elements in the heap
    m_size++;

    // enHPQ the new key at the end
    int i = m_size - 1;
    m_heap[i] = k;

    // Fix the min heap property
    // Moves the element up until i >= parent or root
    while(i != 0 && m_heap[parentIndex(i)].getPriorityLevel() > m_heap[i].getPriorityLevel())
    {
        swap(m_heap[i], m_heap[parentIndex(i)]);
        i = parentIndex(i);
    }
}

//Removes the smallest element and fixes the order
Packet HeapPQ::deHPQ()
{
    //Check if the heap is empty [Dont need since empty checks are done when calling these functions]
    //if (m_size == 0)
    //{
    //    cout << "EMPTY HEAP" << endl;
    //    return -1;
    //}
    //Check if there is only 1 element
    if (m_size == 1)
    {
        m_size--;
        return m_heap[0];
    }
    //Normal extraction
    else
    {
        //Store the root
        Packet root = m_heap[0];

        //Maintain heap shape and then order
        m_heap[0] = m_heap[m_size - 1];
        m_size--;
        heapify(0);

        //Return min element
        return root;
    }
}

//Print the heap in a pretty format
void HeapPQ::printHeap()
{
    int power = 0;
    int value = 1;
    for (int i = 0; i < m_size; i++)
    {
        if (i == value)
        {
            cout << endl;
            power += 1;
            value += (1 << power);
        }
        cout << m_heap[i].getPriorityLevel() << "  ";
    }
    cout << endl;
}

