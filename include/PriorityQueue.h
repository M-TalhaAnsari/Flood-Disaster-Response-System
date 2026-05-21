    #pragma once
    #include <string>
    using namespace std;

    #define MAX_HEAP_SIZE 1000

    struct HeapNode {
        string id;        // victim ID (e.g. "VIC5001") OR location ID as string (for Dijkstra)
        int priority;     // urgency score for victims, OR tentative distance for Dijkstra
    };

    class PriorityQueue {
    public:
        HeapNode Data[MAX_HEAP_SIZE];
        int Size;

        PriorityQueue();
        void Insert(string id, int priority);
        HeapNode ExtractMin();
        HeapNode Peek();
        bool IsEmpty();
        int GetSize();
        void Display();

    private:
        void HeapifyUp(int index);
        void HeapifyDown(int index);
        void Swap(int i, int j);
    };
