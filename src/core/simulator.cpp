#include "simulator.h"
#include <iostream>
#include <level.h>
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"
#include "viewer_iface.h"
using namespace std;
  
void runSimulation(int& playerFloor, int& playerRoom,
                   int& energy, int& time, int& score,
                   char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                   bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                   char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                   char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                   int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){
 int startFloor = 0;
 int startRoom = 0;
 bool valid=false;
  while (!valid) {
 cout << "Choose starting floor (1-5): ";
cin >> startFloor;
cout << "Choose starting room (1-4): ";
cin >> startRoom;
if (!cin) {
    cin.clear();
    cin.ignore(1000, '\n');
    cout << "Please enter numbers only.\n";
    continue;
}
valid = validateStartingRoom(startFloor, startRoom, horizontalConn);
if (!valid) {
    cout << "That room is not allowed. Try again.\n";
}
  }
  playerFloor = startFloor;
playerRoom = startRoom;
 bool isrunning = true;
 bool victory = false;
 while (isrunning) {
renderDashboard(playerFloor, playerRoom, energy, time, score, inventory, inventoryCount, emergencyTripsLeft);
renderRoomDetails(playerFloor, playerRoom, roomEntities, entityCounts[playerFloor - 1][playerRoom - 1]); 
cout << "Commands: M I C U K S L Q\n> ";
char command;
cin >> command;
if (!cin) {
    break;
}
if (command == 'Q' || command == 'q') {
    isrunning = false;
} else {
    cout << "Unknown command.\n";
}
}




}      
                

