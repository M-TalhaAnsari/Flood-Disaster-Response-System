#include <iostream>
#include "../include/LinkedList.h"
using namespace std;

LinkedList::LinkedList() {
    Head = NULL;
    Tail = NULL;
    Size = 0;
}

void LinkedList::Append(int actionType, string description) {
    LogEntry* newEntry = new LogEntry;
    newEntry->actionType = actionType;
    newEntry->description = description;
    newEntry->sequenceNumber = Size + 1;
    newEntry->next = NULL;

    if (Head == NULL) {
        // Empty list case
        Head = newEntry;
        Tail = newEntry;
    } else {
        // Append at tail
        Tail->next = newEntry;
        Tail = newEntry;
    }
    Size++;
}

void LinkedList::Display() {
    if (Head == NULL) {
        cout << "Operations log is empty" << endl;
        cin.get();
        return;
    }

    cout << "------ Operations Log ------" << endl;
    LogEntry* temp = Head;
    while (temp != NULL) {
        cout << "[" << temp->sequenceNumber << "] " << temp->description << endl;
        temp = temp->next;
    }
    cout << "----------------------------" << endl;
    cin.get();
    cin.get();
}

int LinkedList::GetSize() {
    return Size;
}
