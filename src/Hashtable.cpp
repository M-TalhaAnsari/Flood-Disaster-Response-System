#include <iostream>
#include "../include/HashTable.h"
using namespace std;

HashTable::HashTable() {
    Count = 0;
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Buckets[i] = NULL;
    }
}

int HashTable::Hash(string key) {
    // polynomial rolling hash, base 31, mod HASH_TABLE_SIZE
    int hashValue = 0;
    for (int i = 0; i < (int)key.length(); i++) {
        hashValue = ((hashValue * 31) + (int)key[i]) % HASH_TABLE_SIZE;
    }
    if (hashValue < 0) {
        hashValue = hashValue + HASH_TABLE_SIZE;
    }
    return hashValue;
}

void HashTable::Insert(string key, void* data) {
    int index = Hash(key);

    // If key already exists, overwrite the data pointer
    HashEntry* temp = Buckets[index];
    while (temp != NULL) {
        if (temp->key == key) {
            temp->data = data;
            cout << "Key " << key << " already existed; data updated" << endl;
            return;
        }
        temp = temp->next;
    }

    // Otherwise insert at the head of the bucket's chain
    HashEntry* newEntry = new HashEntry;
    newEntry->key = key;
    newEntry->data = data;
    newEntry->next = Buckets[index];
    Buckets[index] = newEntry;
    Count++;
}

void* HashTable::Search(string key) {
    int index = Hash(key);

    HashEntry* temp = Buckets[index];
    while (temp != NULL) {
        if (temp->key == key) {
            return temp->data;
        }
        temp = temp->next;
    }
    return NULL;
}

void HashTable::Remove(string key) {
    int index = Hash(key);

    if (Buckets[index] == NULL) {
        cout << "Key " << key << " not found" << endl;
        return;
    }

    // Head case
    if (Buckets[index]->key == key) {
        HashEntry* toDelete = Buckets[index];
        Buckets[index] = Buckets[index]->next;
        delete toDelete;
        Count--;
        return;
    }

    // Middle / tail case
    HashEntry* prev = Buckets[index];
    HashEntry* curr = Buckets[index]->next;
    while (curr != NULL) {
        if (curr->key == key) {
            prev->next = curr->next;
            delete curr;
            Count--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    cout << "Key " << key << " not found" << endl;
}

void HashTable::Display() {
    if (Count == 0) {
        cout << "Hash table is empty" << endl;
        return;
    }

    cout << "Hash table contents (" << Count << " entries):" << endl;
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        if (Buckets[i] != NULL) {
            cout << "  Bucket " << i << ":";
            HashEntry* temp = Buckets[i];
            while (temp != NULL) {
                cout << " -> [" << temp->key << "]";
                temp = temp->next;
            }
            cout << endl;
        }
    }
}

int HashTable::GetCount() {
    return Count;
}
