#ifndef STRUCTURES_H
#define STRUCTURES_H

#include<iostream>

struct Vehicle{
    int id;
    std::string type;
    std::string current_location;
    int status; // 0 =Available, 1=Dispatched 2=prepositioned
    int capacity;
};


struct Request{
    std::string Location;   
    int num_people;
    int floodseverity; //1-4 reported by citizen
    bool ishandled; //true after vehicle dispatched
};

struct Location {
    int id;
    std::string name;
    int type;        // 0=Village 1=Camp 2=Hospital 3=Depot 4=Station
    int population;
    int floodSeverity; // 0-4
    int riskScore;     // population * floodSeverity
};

#endif