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
  else if (command == 'C' || command == 'c') {
    char symbol;
    cout << "Item symbol to collect: ";
    cin >> symbol;
     bool picked = handleCollectCommand(symbol, playerFloor, playerRoom, energy, time, score,
                                       roomEntities,entityCounts[playerFloor - 1][playerRoom - 1],
                                       inventory, inventoryCount, objectives);
    if (picked && symbol == ENT_EMERGENCY_CARD) {
        emergencyTripsLeft = 3;
    }
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
bool handleCollectCommand(char itemSymbol, int currentFloor, int currentRoom,
                          int& energy, int& time, int& score,
                          char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int& entityCount,
                          char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                          bool objectives[NUM_OBJECTIVES]){
                         
int f = currentFloor - 1;
int r = currentRoom - 1;
int pos = -1;
for (int e = 0; e < entityCount; e++) {
    if (roomEntities[f][r][e] == itemSymbol) {
        pos = e;
        break;
    }
}
if (pos == -1) {
    cout << "That item is not here."<<endl;
    return false;
}
for (int e = pos; e < entityCount - 1; e++) {
    roomEntities[f][r][e] = roomEntities[f][r][e + 1];
}
roomEntities[f][r][entityCount - 1] = ENT_NONE;
entityCount--;
applyResourceDelta(energy, time, COST_ITEM_PICKUP_ENERGY, COST_ITEM_PICKUP_TIME);
if (itemSymbol == ENT_ENERGY_PACK) {
    applyResourceDelta(energy, time, BONUS_ENERGY_PACK, 0);
    cout << "You used an Energy Pack: +25 energy." << endl;
}
if (itemSymbol == ENT_EMERGENCY_TOOLKIT) {
    applyResourceDelta(energy, time, BONUS_EMERGENCY_TOOLKIT, 0);
    cout << "You used an Emergency Toolkit: +15 energy."<<endl;
}
if (itemSymbol == ENT_OBSERVATION_TOKEN) {
    applyResourceDelta(energy, time,0, BONUS_OBSERVATION_TOKEN);
    cout << "You used an Observation Token: +15 energy."<<endl;
}
if (itemSymbol != ENT_ENERGY_PACK && itemSymbol != ENT_EMERGENCY_TOOLKIT && itemSymbol != ENT_OBSERVATION_TOKEN) {
    addItem(inventory, inventoryCount, itemSymbol);
}
if (itemSymbol == ENT_MASTER_KEY) {
    objectives[OBJ_MASTER_KEY] = true;
    applyScoreDelta(score, SCORE_KEY_COLLECT);
    cout << "You picked up the Master Key." << endl;
}
if (itemSymbol == ENT_LIFT_PASS) {
    objectives[OBJ_LIFT_PASS] = true;
    applyScoreDelta(score, SCORE_LIFT_PASS);
    cout << "Lift Pass" << endl;
}
if (itemSymbol == ENT_FUSE) {
    objectives[OBJ_FUSE_COLLECTED] = true;
    cout << "You picked up the fuse." << endl;
}
if (itemSymbol == ENT_CAT_CARRIER) {
    objectives[OBJ_CAT_CARRIER] = true;
    cout << "You picked up the Cat Carrier." << endl;
}
if (itemSymbol == ENT_MEDICAL_KIT) {
    cout << "You picked up the Medical Kit." << endl;
}
if (itemSymbol == ENT_EMERGENCY_CARD) {
    cout << "You picked up the Emergency Access Card." << endl;
}
return true;

 }







                

