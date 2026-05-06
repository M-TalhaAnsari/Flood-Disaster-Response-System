#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
using namespace std;
class link_list_Node {
public:
        string data;
        link_list_Node* next;
};
class LinkedList {
private:
    link_list_Node* head;

public:
    LinkedList();
    void insert(string data);
    // Adds a log entry string to the END of the list (tail append)
    // Called after EVERY operation: dispatch, evacuation, delivery, preposition

    void display();
    // Traverses from head to NULL, prints each log entry
    // Called by: FDRRC::displayOperationLog()
};

#endif