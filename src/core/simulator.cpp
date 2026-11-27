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
renderRoomDetails(playerFloor-1, playerRoom-1, roomEntities, entityCounts[playerFloor - 1][playerRoom - 1]); 
cout << "Commands: M I C U K S L Q\n> ";
char command;
cin >> command;
if (!cin) {
    break;
}
if (command == 'Q' || command == 'q') {
    isrunning = false;
}  else if (command == 'I' || command == 'i') {
    handleInspectCommand(playerFloor, playerRoom, energy, time, roomEntities,
                         entityCounts[playerFloor - 1][playerRoom - 1], objectives, score);
    }

else {
    cout << "Unknown command.\n";
}
}
}  
 bool handleInspectCommand(int currentFloor, int currentRoom,
                          int& energy, int& time,
                          const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int entityCount,
                          bool objectives[NUM_OBJECTIVES], int& score){
 applyResourceDelta(energy, time, COST_INSPECT_ENERGY, COST_INSPECT_TIME);

int f = currentFloor - 1;
int r = currentRoom - 1;
bool newfound = false;
for (int e = 0; e < entityCount; e++) {
    char s = roomEntities[f][r][e];
    if (s == ENT_CLUE_4 && !objectives[OBJ_CLUE_4]) {
    objectives[OBJ_CLUE_4] = true;
    applyScoreDelta(score, SCORE_CLUE_FOUND);
    cout << "You found clue digit 4!"<<endl;
    newfound = true;
    }
     if (s == ENT_CLUE_2 && !objectives[OBJ_CLUE_2]) {
    objectives[OBJ_CLUE_2] = true;
    applyScoreDelta(score, SCORE_CLUE_FOUND);
    cout << "You found clue digit 42"<<endl;
    newfound = true;
     }
     if (s == ENT_CLUE_7 && !objectives[OBJ_CLUE_7]) {
    objectives[OBJ_CLUE_7] = true;
    applyScoreDelta(score, SCORE_CLUE_FOUND);
    cout << "You found clue digit 7!"<<endl;
    newfound = true;
}
}
if (!newfound) {
    cout << "You find nothing new."<<endl;
}
return true;
}






                

