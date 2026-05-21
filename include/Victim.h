#pragma once
#include <string>
using namespace std;

struct Victim {
    string id;             // "VIC5001"
    int locationId;        // where they are
    int numPeople;
    int severity;          // 1-5 scale
    int urgency;           // heap priority: 1000 - ((severity * 10) + numPeople)
                           // smaller value = more urgent (suits min-heap)
    bool rescued;
};
