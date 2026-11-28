#include "simulator.h"
#include <iostream>
#include "level.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"
using namespace std;
void runSimulation(int& playerFloor, int& playerRoom,
                   int& energy, int& time, int& score,
                   char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                   bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                   char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                   char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                   int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]) {
    int startFloor = 0;
    int startRoom = 0;
    bool valid = false;
    while (!valid) {
        cout << "Choose starting floor (1-5): ";
        cin >> startFloor;
        cout << "Choose starting room (1-4): ";
        cin >> startRoom;
        if (!cin) {
            if (cin.eof()) {
                return;
            }
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
    while (isrunning) {
        renderDashboard(playerFloor, playerRoom, energy, time, score, inventory, inventoryCount, emergencyTripsLeft);
        renderRoomDetails(playerFloor - 1, playerRoom - 1, roomEntities, entityCounts[playerFloor - 1][playerRoom - 1]);
        cout << "Commands: M I C U K S L B O Q\n> ";
        char command;
        cin >> command;
        if (!cin) {
            break;
        }

        if (command == 'Q' || command == 'q') {
            isrunning = false;
        }
        else if (command == 'I' || command == 'i') {
            handleInspectCommand(playerFloor, playerRoom, energy, time, roomEntities,
                                 entityCounts[playerFloor - 1][playerRoom - 1], objectives, score);
        }
        else if (command == 'C' || command == 'c') {
            char symbol;
            cout << "Item symbol to collect: ";
            cin >> symbol;
            bool picked = handleCollectCommand(symbol, playerFloor, playerRoom, energy, time, score,
                                               roomEntities, entityCounts[playerFloor - 1][playerRoom - 1],
                                               inventory, inventoryCount, objectives);
            if (picked && symbol == ENT_EMERGENCY_CARD) {
                emergencyTripsLeft = 3;
            }
        }
        else if (command == 'S' || command == 's') {
            bool saved = saveGameStateToFile("out/savegame.txt", playerFloor, playerRoom,
                                             energy, time, score,
                                             inventory, inventoryCount,
                                             objectives, emergencyTripsLeft,
                                             roomEntities, entityCounts);
            if (saved) {
                cout << "Game saved." << endl;
            } else {
                cout << "Could not save the game." << endl;
            }
        }
        else if (command == 'L' || command == 'l') {
            bool load = loadGameStateFromFile("out/savegame.txt", playerFloor, playerRoom,
                                              energy, time, score,
                                              inventory, inventoryCount,
                                              objectives, emergencyTripsLeft,
                                              roomEntities, entityCounts);
            if (load) {
                cout << "Game loaded." << endl;
            } else {
                cout << "No valid save file found." << endl;
            }
        }
        else if (command == 'B' || command == 'b') {
            renderAsciiBuilding(playerFloor, playerRoom);
        }
        else if (command == 'O' || command == 'o') {
            renderObjectivesChecklist(objectives);
        }
        else if (command == 'M' || command == 'm') {
            int tf, tr;
            cout << "Target floor (1-5): ";
            cin >> tf;
            cout << "Target room (1-4): ";
            cin >> tr;
            if (!cin) {
                if (cin.eof()) {
                    break;
                }
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Please enter numbers only." << endl;
            } else {
                bool lift2Operational = objectives[OBJ_FUSE_INSTALLED] && objectives[OBJ_LIFT_PASS];
                int cf = playerFloor - 1;
                int cr = playerRoom - 1;
                bool moved = handleMoveCommand(tf - 1, tr - 1, cf, cr, energy, time,
                                               horizontalConn, inventory, inventoryCount,
                                               emergencyTripsLeft, lift2Operational);
                if (moved) {
                    playerFloor = cf + 1;
                    playerRoom = cr + 1;
                } else {
                    cout << "You cannot go there." << endl;
                }
            }
        }
        else if (command == 'U' || command == 'u') {
            char symbol;
            cout << "Item symbol to use: ";
            cin >> symbol;
            bool used = handleUseCommand(symbol, playerFloor - 1, playerRoom - 1, energy, time, score,
                                         inventory, inventoryCount, objectives);
            if (!used) {
                cout << "You cannot use that here." << endl;
            }
        }
        else if (command == 'K' || command == 'k') {
            char code[10];
            cout << "Enter cabinet code: ";
            cin.width(10);
            cin >> code;
            bool opened = handleUnlockCabinetCommand(code, playerFloor - 1, playerRoom - 1, objectives, score,
                                                     roomEntities,
                                                     entityCounts[playerFloor - 1][playerRoom - 1]);
            if (opened) {
                applyResourceDelta(energy, time, COST_ITEM_USE_ENERGY, COST_ITEM_USE_TIME);
                cout << "The cabinet opens!" << endl;
            } else {
                cout << "The cabinet stays locked." << endl;
            }
        }
        else {
            cout << "Unknown command.\n";
        }

        if (checkEndConditions(energy, time, objectives)) {
            isrunning = false;
        }
    }

    bool win = objectives[OBJ_MISSION_WON];
    if (win) {
        cout << "MISSION COMPLETE! You boarded the train." << endl;
    } else if (energy <= 0 || time <= 0) {
        cout << "MISSION FAILED: you ran out of energy or time." << endl;
    }
    writeGameSummaryReport("out/game_summary.txt", score, energy, time, objectives, win);
}

bool handleMoveCommand(int targetFloor, int targetRoom,
                       int& currentFloor, int& currentRoom,
                       int& energy, int& time,
                       const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                       const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                       int& emergencyTripsLeft, bool lift2Operational) {
    if (targetFloor < 0 || targetFloor >= NUM_FLOORS ||
        targetRoom < 0 || targetRoom >= ROOMS_PER_FLOOR) {
        return false;
    }

    if (targetFloor == currentFloor && targetRoom == currentRoom) {
        return false;
    }

    int deltaEnergy = 0;
    int deltaTime = 0;
    bool emergencyTripUsed = false;
    char connType = CONN_NONE;

    if (targetFloor == currentFloor) {
        if (isDirectlyConnected(currentFloor, currentRoom,
                                targetFloor, targetRoom,
                                horizontalConn, connType)) {
            if (!canTraverse(targetFloor, targetRoom, connType,
                             inventory, inventoryCount,
                             emergencyTripsLeft, lift2Operational)) {
                return false;
            }

            if (connType == CONN_WINDOW) {
                deltaEnergy = -3;
                deltaTime = -2;
            } else {
                deltaEnergy = -2;
                deltaTime = -1;
            }
        } else {
            return false;
        }
    }
    // LIFT 1
    else if (canUseLift1(currentFloor + 1, targetFloor + 1) &&
             currentRoom == targetRoom &&
             (currentRoom == 1 || currentRoom == 2)) {
        deltaEnergy = -4;
        deltaTime = -3;
    }
    // STAIRS
    else if (currentRoom == 0 &&
             targetRoom == 0 &&
             ((currentFloor == 0 && targetFloor == 1) ||
              (currentFloor == 1 && targetFloor == 0) ||
              (currentFloor == 1 && targetFloor == 2) ||
              (currentFloor == 2 && targetFloor == 1) ||
              (currentFloor == 3 && targetFloor == 4) ||
              (currentFloor == 4 && targetFloor == 3))) {
        deltaEnergy = -2;
        deltaTime = -1;
    }
    // LIFT 2
    else if (canUseLift2(currentFloor + 1, targetFloor + 1,
                         lift2Operational, emergencyTripsLeft) &&
             currentRoom == 3 &&
             targetRoom == 3) {
        if (lift2Operational) {
            if (!hasItem(inventory, inventoryCount, ENT_LIFT_PASS)) {
                return false;
            }
        } else {
            if (!hasItem(inventory, inventoryCount, ENT_EMERGENCY_CARD) ||
                emergencyTripsLeft <= 0) {
                return false;
            }
        }
        deltaEnergy = -4;
        deltaTime = -3;

        if (!lift2Operational) {
            emergencyTripUsed = true;
        }
    }
    else {
        return false;
    }

    currentFloor = targetFloor;
    currentRoom = targetRoom;
    applyResourceDelta(energy, time, deltaEnergy, deltaTime);

    if (emergencyTripUsed) {
        emergencyTripsLeft--;
    }

    return true;
}

bool handleInspectCommand(int currentFloor, int currentRoom,
                          int& energy, int& time,
                          const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int entityCount,
                          bool objectives[NUM_OBJECTIVES], int& score) {
    applyResourceDelta(energy, time, COST_INSPECT_ENERGY, COST_INSPECT_TIME);

    int f = currentFloor - 1;
    int r = currentRoom - 1;
    bool newfound = false;
    for (int e = 0; e < entityCount; e++) {
        char s = roomEntities[f][r][e];
        if (s == ENT_CLUE_4 && !objectives[OBJ_CLUE_4]) {
            objectives[OBJ_CLUE_4] = true;
            applyScoreDelta(score, SCORE_CLUE_FOUND);
            cout << "You found clue digit 4!" << endl;
            newfound = true;
        }
        if (s == ENT_CLUE_2 && !objectives[OBJ_CLUE_2]) {
            objectives[OBJ_CLUE_2] = true;
            applyScoreDelta(score, SCORE_CLUE_FOUND);
            cout << "You found clue digit 2!" << endl;
            newfound = true;
        }
        if (s == ENT_CLUE_7 && !objectives[OBJ_CLUE_7]) {
            objectives[OBJ_CLUE_7] = true;
            applyScoreDelta(score, SCORE_CLUE_FOUND);
            cout << "You found clue digit 7!" << endl;
            newfound = true;
        }
    }
    if (!newfound) {
        cout << "You find nothing new." << endl;
    }
    return true;
}


bool handleCollectCommand(char itemSymbol, int currentFloor, int currentRoom,
                          int& energy, int& time, int& score,
                          char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int& entityCount,
                          char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                          bool objectives[NUM_OBJECTIVES]) {
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
        cout << "That item is not here." << endl;
        return false;
    }
    if (itemSymbol == ENT_CLUE_4 || itemSymbol == ENT_CLUE_2 || itemSymbol == ENT_CLUE_7 ||
        itemSymbol == ENT_CAT || itemSymbol == ENT_PASSENGER || itemSymbol == ENT_CONTROL_PANEL) {
        cout << "You cannot pick that up." << endl;
        return false;
    }
    // The Train Key can only be taken after the cabinet is unlocked.
    if (itemSymbol == ENT_CABINET_TRAIN_KEY && !objectives[OBJ_CABINET_UNLOCKED]) {
        cout << "The cabinet is locked. Unlock it first." << endl;
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
        cout << "You used an Emergency Toolkit: +15 energy." << endl;
    }
    if (itemSymbol == ENT_OBSERVATION_TOKEN) {
        applyResourceDelta(energy, time, 0, BONUS_OBSERVATION_TOKEN);
        cout << "You used an Observation Token: +15 time." << endl;
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
        cout << "You picked up the Lift Pass." << endl;
    }
    if (itemSymbol == ENT_FUSE) {
        objectives[OBJ_FUSE_COLLECTED] = true;
        cout << "You picked up the Fuse." << endl;
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
    if (itemSymbol == ENT_CABINET_TRAIN_KEY) {
        objectives[OBJ_TRAIN_KEY] = true;
        applyScoreDelta(score, SCORE_TRAIN_KEY);
        cout << "You took the Train Key." << endl;
    }
    return true;
}


bool handleUseCommand(char itemSymbol, int currentFloor, int currentRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES]) {
    if (!hasItem(inventory, inventoryCount, itemSymbol)) {
        return false;
    }

    if (itemSymbol == ENT_CAT_CARRIER) {
        if (currentFloor != 2 || currentRoom != 2) {
            return false;
        }

        if (objectives[OBJ_CAT_SECURED]) {
            return false;
        }

        objectives[OBJ_CAT_SECURED] = true;

        removeItem(inventory, inventoryCount, ENT_CAT_CARRIER);

        applyScoreDelta(score, 100);
        applyResourceDelta(energy, time, -1, -1);

        return true;
    }

    if (itemSymbol == ENT_FUSE) {
        if (currentFloor != 2 || currentRoom != 3) {
            return false;
        }

        if (!installFuseControlPanel(inventory, inventoryCount, objectives, score)) {
            return false;
        }

        applyResourceDelta(energy, time, -1, -1);

        return true;
    }

    if (itemSymbol == ENT_MEDICAL_KIT) {
        if (currentFloor != 3 || currentRoom != 0) {
            return false;
        }

        if (objectives[OBJ_PASSENGER_RESCUED]) {
            return false;
        }

        objectives[OBJ_PASSENGER_RESCUED] = true;

        removeItem(inventory, inventoryCount, ENT_MEDICAL_KIT);

        applyScoreDelta(score, 100);
        applyResourceDelta(energy, time, -1, -1);

        return true;
    }

    if (itemSymbol == ENT_MASTER_KEY) {
        if (currentFloor != 2 ||
            (currentRoom != 0 && currentRoom != 1)) {
            return false;
        }

        if (!unlockFloor3Door(inventory, inventoryCount)) {
            return false;
        }

        applyResourceDelta(energy, time, -1, -1);

        return true;
    }

    if (itemSymbol == ENT_CABINET_TRAIN_KEY) {
        if (currentFloor != 4 || currentRoom != 1) {
            return false;
        }

        if (!objectives[OBJ_PASSENGER_RESCUED] || !objectives[OBJ_CAT_SECURED]) {
            return false;
        }

        if (objectives[OBJ_TRAIN_PREPARED]) {
            return false;
        }

        objectives[OBJ_TRAIN_PREPARED] = true;
        objectives[OBJ_MISSION_WON] = true;

        applyScoreDelta(score, SCORE_PREPARE_TRAIN);
        applyScoreDelta(score, SCORE_MISSION_VICTORY);
        applyResourceDelta(energy, time, COST_ITEM_USE_ENERGY, COST_ITEM_USE_TIME);

        return true;
    }

    return false;
                      }
bool handleUnlockCabinetCommand(const char* code, int currentFloor, int currentRoom,
                                bool objectives[NUM_OBJECTIVES], int& score,
                                char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                                int& entityCount) {
    
    (void)roomEntities;
    (void)entityCount;

    if (currentFloor != 4 || currentRoom != 0) {
        return false;
    }

    if (objectives[OBJ_CABINET_UNLOCKED]) {
        return false;
    }

    if (!objectives[OBJ_CLUE_4] ||
        !objectives[OBJ_CLUE_2] ||
        !objectives[OBJ_CLUE_7]) {
        return false;
    }

    if (!verifyCabinetCode(code, objectives)) {
        return false;
    }

    objectives[OBJ_CABINET_UNLOCKED] = true;
    applyScoreDelta(score, SCORE_CABINET_UNLOCK);
    return true;
}


bool checkEndConditions(int energy, int time, const bool objectives[NUM_OBJECTIVES]) {
    if (energy <= 0 || time <= 0) {
        return true;
    }
    if (objectives[OBJ_MISSION_WON]) {
        return true;
    }
    return false;
}

                                


