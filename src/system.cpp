#include <iostream>
#include "../include/system.h"

using namespace std;

void clearScreen()
{
    system("cls");
}

void header(){
    cout<<"==============================================================\n";
    cout<<"                  Flood Disaster Response                     \n";
    cout<<"                           &                                  \n";
    cout<<"             Intelligent Rescue Cordinator System             \n";
    cout<<"==============================================================\n";

}
void main_menu(){
    clearScreen();
    header();
    cout << "\n====== MAIN MENU ======\n";
    cout << "1. Flood Reporting\n";
    cout << "2. Location & Road Management\n";
    cout << "3. Evacuation & Dispatch\n";
    cout << "4. Search & Reports\n";
    cout << "0. Exit\n";
}

FDRRC::FDRRC()
    : graph(),
      locationCount(0),
      engine(graph, locations, locationCount)
{}

void FDRRC::Create_Rescue_Request() {
    cout << "Create Rescue Request\n";
}


void FDRRC::reportflood() {
    cout << "Report Flood\n";
}

void FDRRC::addLocation() {}
void FDRRC::addRoad() {}
void FDRRC::UpdateRoadStatus() {}
void FDRRC::UpdateFloodSeverity() {}
void FDRRC::findEvacuationRoute() {}
void FDRRC::DispatchNearestVehicle() {}
void FDRRC::searchLocation() {}
void FDRRC::addVehicle(){};
void FDRRC::SearchVehicle() {}
void FDRRC::displayOperationLog() {}
void FDRRC::findHighestRiskLocation() {
    // calls engine.runFullAnalysis()
}

void FDRRC::prePositionVehicle() {
    // calls engine.getHighestRiskLocation()
    // then dijkstra to move vehicle there
}


void FDRRC::menu() {

    int mainChoice, subChoice;
    
    bool Exit = true;
    while(Exit){
 
        main_menu();
        cin>>mainChoice;
        if(mainChoice == 0){
            Exit = false;
        }
        // FLOOD REPORTING
        else if(mainChoice==1){
            clearScreen();
            cout << "\n--- Flood Reporting ---\n";
            cout << "1. Create Rescue Request\n";
            cout << "2. Report Flood Status\n";
            cout << "0. Back\n";
            cin >> subChoice;

            if(subChoice == 1) Create_Rescue_Request();
            else if(subChoice == 2) reportflood();
            else continue;
        }
        
        // LOCATION & ROAD
        else if(mainChoice==2){
            clearScreen();    
            cout << "\n--- Location & Road Management ---\n";
            cout << "1. Add Location\n";
            cout << "2. Add Road\n";
            cout << "3. Add Vehicle\n";
            cout << "4. Update Road Status\n";
            cout << "5. Update Flood Severity\n";
            cout << "0. Back\n";

            cin >> subChoice;

            if(subChoice == 1) addLocation();
            else if(subChoice == 2) addRoad();
            else if(subChoice == 3) addVehicle();
            else if(subChoice == 4) UpdateRoadStatus();
            else if(subChoice == 5) UpdateFloodSeverity();
            else continue;

          }


        // EVACUATION & DISPATCH
        else if (mainChoice==3){
            clearScreen();
            cout << "\n--- Evacuation & Dispatch ---\n";
            cout << "1. Find Evacuation Route\n";
            cout << "2. Dispatch Nearest Vehicle\n";
            cout << "3. Find Highest Risk Location\n";
            cout << "4. Pre-position Vehicle\n";
            cout << "0. Back\n";

            cin >> subChoice;

            if(subChoice == 1) findEvacuationRoute();
            else if(subChoice == 2) DispatchNearestVehicle();
            else if(subChoice == 3) findHighestRiskLocation();
            else if(subChoice == 4) prePositionVehicle();
            else continue;

        }
          
        // SEARCH & REPORTS
        else if(mainChoice ==4){ 
            clearScreen();
            cout << "\n--- Search & Reports ---\n";
            cout << "1. Search Location\n";
            cout << "3. Search Vehicle\n";
            cout << "4. Display Operation Log\n";
            cout << "0. Back\n";

            cin >> subChoice;

            if(subChoice == 1) searchLocation();
            else if(subChoice == 2) SearchVehicle();
            else if(subChoice == 3) displayOperationLog();
            else continue;

        }
        else{
            cout<<"Invalid Input\n";
            cin.ignore();
            cin.get();
            continue;
        }
    }
}
    
