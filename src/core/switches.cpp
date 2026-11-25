#include "switches.h"
#include "util.h"
#include "physics.h"
#include <cstring>
bool canUseLift1(int fromFloor, int toFloor)
{
    if (fromFloor == 1 && toFloor == 2)
    {
        return true;
    }
    else if (fromFloor == 2 && toFloor == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool canUseLift2(int fromFloor, int toFloor, bool lift2Operational, int emergencyTripsLeft)
  
{
    bool validFloors =
        (fromFloor == 2 || fromFloor == 4 || fromFloor == 5) &&
        (toFloor == 2 || toFloor == 4 || toFloor == 5) &&
        (fromFloor != toFloor);

    if (validFloors && (lift2Operational || emergencyTripsLeft > 0))
    {
        return true;
    }
    return false;

}
bool unlockFloor3Door(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount){

    return hasItem(inventory, inventoryCount, ENT_MASTER_KEY);

}
bool installFuseControlPanel(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                             bool objectives[NUM_OBJECTIVES], int& score){
                                 if (!hasItem(inventory, inventoryCount, ENT_FUSE))
        return false;

    if (objectives[OBJ_FUSE_INSTALLED])
        return false;

    removeItem(inventory, inventoryCount, ENT_FUSE);

    objectives[OBJ_FUSE_INSTALLED] = true;
    applyScoreDelta(score, 20);

    return true;
                             }
bool verifyCabinetCode(const char* enteredCode, const bool objectives[NUM_OBJECTIVES]){
 if (!objectives[OBJ_CLUE_4] ||
        !objectives[OBJ_CLUE_2] ||
        !objectives[OBJ_CLUE_7])
    {
        return false;
    }

    if (strcmp(enteredCode, "427") != 0)
        return false;

    return true;
}
