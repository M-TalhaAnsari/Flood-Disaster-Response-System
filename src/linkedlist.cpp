#include "../include/linkedlist.h"

LinkedList::LinkedList() {
    head = NULL;
}

void LinkedList::insert(string data) {
    link_list_Node* newNode = new link_list_Node();
    newNode->data = data;
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } else {
        link_list_Node* temp = head;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newNode;
    }
}

void LinkedList::display() {
    link_list_Node* temp = head;

    while (temp != NULL) {
        cout << temp->data << endl;
        temp = temp->next;
    }
}