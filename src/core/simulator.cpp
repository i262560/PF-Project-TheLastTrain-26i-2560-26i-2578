#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"
#include <iostream>
using namespace std;
//2nd function 
bool handleMoveCommand(int targetFloor, int targetRoom,
                       int& currentFloor, int& currentRoom,
                       int& energy, int& time,
                       const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                       const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                       int& emergencyTripsLeft, bool lift2Operational){
             if (targetFloor < 0 || targetFloor >= NUM_FLOORS ||
    targetRoom < 0 || targetRoom >= ROOMS_PER_FLOOR)
{
    return false;
}

if (targetFloor == currentFloor && targetRoom == currentRoom)
{
    return false;
}
int deltaEnergy = 0;
int deltaTime = 0;
bool emergencyTripUsed = false;
char connType = CONN_NONE;
if (targetFloor == currentFloor)
{
    if (isDirectlyConnected(currentFloor, currentRoom,
                            targetFloor, targetRoom,
                            horizontalConn, connType))
    {
        if (!canTraverse(targetFloor, targetRoom, connType,
                         inventory, inventoryCount,
                         emergencyTripsLeft, lift2Operational))
        {
            return false;
        }

        if (connType == CONN_WINDOW)
        {
            deltaEnergy = -3;
            deltaTime = -2;
        }
        else
        {
            deltaEnergy = -2;
            deltaTime = -1;
        }
    }
    else
    {
        return false;
    }
}
// LIFT 1
else if (
    canUseLift1(currentFloor + 1, targetFloor + 1) &&
    currentRoom == targetRoom &&
    (currentRoom == 1 || currentRoom == 2)
)
{
    deltaEnergy = -4;
    deltaTime = -3;
}
// STAIRS
else if (
    currentRoom == 0 &&
    targetRoom == 0 &&
    ((currentFloor == 0 && targetFloor == 1) ||
     (currentFloor == 1 && targetFloor == 0) ||
     (currentFloor == 1 && targetFloor == 2) ||
     (currentFloor == 2 && targetFloor == 1) ||
     (currentFloor == 3 && targetFloor == 4) ||
     (currentFloor == 4 && targetFloor == 3))
)
{
    deltaEnergy = -2;
    deltaTime = -1;
}
// LIFT 2
else if (
    canUseLift2(currentFloor + 1, targetFloor + 1,
                lift2Operational, emergencyTripsLeft) &&
    currentRoom == 3 &&
    targetRoom == 3
)
{
    if (lift2Operational)
{
    if (!hasItem(inventory, inventoryCount, ENT_LIFT_PASS))
    {
        return false;
    }
}
else
{
    if (!hasItem(inventory, inventoryCount, ENT_EMERGENCY_CARD) ||
        emergencyTripsLeft <= 0)
    {
        return false;
    }
}
deltaEnergy = -4;
deltaTime = -3;

if (!lift2Operational)
{
    emergencyTripUsed = true;
}
}
else
{
    return false;
}
currentFloor = targetFloor;
currentRoom = targetRoom; 
applyResourceDelta(energy, time, deltaEnergy, deltaTime);

if (emergencyTripUsed)
{
    emergencyTripsLeft--;
}

return true;          
                       }
//5th function
bool handleUseCommand(char itemSymbol, int currentFloor, int currentRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES]){
     if (!hasItem(inventory, inventoryCount, itemSymbol))
{
    return false;
}
if (itemSymbol == ENT_CAT_CARRIER)
{
    if (currentFloor != 2 || currentRoom != 2)
    {
        return false;
    }

    if (objectives[OBJ_CAT_SECURED])
    {
        return false;
    }

    objectives[OBJ_CAT_SECURED] = true;

    removeItem(inventory, inventoryCount, ENT_CAT_CARRIER);

    applyScoreDelta(score, 100);
    applyResourceDelta(energy, time, -1, -1);

    return true;
}
if (itemSymbol == ENT_FUSE)
{
    if (currentFloor != 2 || currentRoom != 3)
    {
        return false;
    }

    if (!installFuseControlPanel(inventory, inventoryCount, objectives, score))
    {
        return false;
    }

    applyResourceDelta(energy, time, -1, -1);

    return true;
}
if (itemSymbol == ENT_MEDICAL_KIT)
{
    if (currentFloor != 3 || currentRoom != 0)
    {
        return false;
    }

    if (objectives[OBJ_PASSENGER_RESCUED])
    {
        return false;
    }

    objectives[OBJ_PASSENGER_RESCUED] = true;

    removeItem(inventory, inventoryCount, ENT_MEDICAL_KIT);

    applyScoreDelta(score, 100);
    applyResourceDelta(energy, time, -1, -1);

    return true;
}
if (itemSymbol == ENT_MASTER_KEY)
{
    if (currentFloor != 2 ||
        (currentRoom != 0 && currentRoom != 1))
    {
        return false;
    }

    if (!unlockFloor3Door(inventory, inventoryCount))
    {
        return false;
    }

    applyResourceDelta(energy, time, -1, -1);

    return true;
}
 return false;



                    
                      }
//6th function
bool handleUnlockCabinetCommand(const char* code, int currentFloor, int currentRoom,
                                bool objectives[NUM_OBJECTIVES], int& score,
                                char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                                int& entityCount){

    if (currentFloor != 4 || currentRoom != 0)
    {
        return false;
    }

    if (!objectives[OBJ_CLUE_4] ||
        !objectives[OBJ_CLUE_2] ||
        !objectives[OBJ_CLUE_7])
    {
        return false;
    }
    if (!verifyCabinetCode(code, objectives))
{
    return false;
}
objectives[OBJ_CABINET_UNLOCKED] = true;
applyScoreDelta(score, 30);
 return true;
}
                                


