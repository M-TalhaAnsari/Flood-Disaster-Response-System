#include <iostream>
#include "../include/PriorityQueue.h"
using namespace std;

PriorityQueue::PriorityQueue() {
    Size = 0;
}

void PriorityQueue::Swap(int i, int j) {
    HeapNode temp = Data[i];
    Data[i] = Data[j];
    Data[j] = temp;
}

void PriorityQueue::HeapifyUp(int index)
 {
    if (index == 0) {
        return;
    }

    int parent = (index - 1) / 2;
    if (Data[index].priority < Data[parent].priority) {
        Swap(index, parent);
        HeapifyUp(parent);
    }
}

void PriorityQueue::HeapifyDown(int index) {
    int left = (2 * index) + 1;
    int right = (2 * index) + 2;
    int smallest = index;

    if (left < Size && Data[left].priority < Data[smallest].priority) {
        smallest = left;
    }
    if (right < Size && Data[right].priority < Data[smallest].priority) {
        smallest = right;
    }

    if (smallest != index) {
        Swap(index, smallest);
        HeapifyDown(smallest);
    }
}

void PriorityQueue::Insert(string id, int priority) {
    if (Size >= MAX_HEAP_SIZE) {
        cout << "Priority queue is full" << endl;
        cin.get();
        cin.get();
        return;
    }

    Data[Size].id = id;
    Data[Size].priority = priority;
    Size++;
    HeapifyUp(Size - 1);
}

HeapNode PriorityQueue::ExtractMin() {
    HeapNode empty;
    empty.id = "";
    empty.priority = -1;

    if (Size == 0) {
        cout << "Priority queue is empty" << endl;
        cin.get();
        cin.get();
        return empty;
    }

    HeapNode top = Data[0];

    if (Size == 1) {
        Size--;
        return top;
    }

    Data[0] = Data[Size - 1];
    Size--;
    HeapifyDown(0);
    return top;
}

HeapNode PriorityQueue::Peek() {
    HeapNode empty;
    empty.id = "";
    empty.priority = -1;

    if (Size == 0) {
        cout << "Priority queue is empty" << endl;
        cin.get();
        cin.get();
        return empty;
    }

    return Data[0];
}

bool PriorityQueue::IsEmpty() {
    if (Size == 0) {
        return true;
    }
    return false;
}

int PriorityQueue::GetSize() {
    return Size;
}

void PriorityQueue::Display() {
    if (Size == 0) {
        cout << "Priority queue is empty" << endl;
        cin.get();
        cin.get();
        return;
    }

    cout << "Priority Queue contents (heap order):" << endl;
    for (int i = 0; i < Size; i++) {
        cout << "  [" << i << "] id=" << Data[i].id
             << "  priority=" << Data[i].priority << endl;
    }
    cin.get();
    cin.get();
}
