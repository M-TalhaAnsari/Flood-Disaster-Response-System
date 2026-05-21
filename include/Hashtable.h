#pragma once
#include <string>
using namespace std;

#define HASH_TABLE_SIZE 101   // prime number for better distribution

struct HashEntry {
    string key;          // "V101", "VIC5001", etc.
    void* data;          // pointer to a Vehicle or Victim (we cast on retrieval)
    HashEntry* next;
};

class HashTable {
public:
    HashEntry* Buckets[HASH_TABLE_SIZE];
    int Count;

    HashTable();
    void Insert(string key, void* data);
    void* Search(string key);     // returns NULL if not found
    void Remove(string key);
    void Display();
    int GetCount();

private:
    int Hash(string key);    // simple polynomial rolling hash on the string
};
