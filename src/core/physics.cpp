#include "physics.h"
#include "util.h"

bool isDirectlyConnected(int fromF,int fromR,int toF,int toR,const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR-1],char& outConnType)
{
    if (fromF < 0 || fromF >= NUM_FLOORS ||
    toF < 0 || toF >= NUM_FLOORS ||
    fromR < 0 || fromR >= ROOMS_PER_FLOOR ||
    toR < 0 || toR >= ROOMS_PER_FLOOR)
{
    return false;
}
    
if (fromF != toF)
{
    return false;
}
if (toR != fromR + 1 && toR != fromR - 1)
{
    return false;
}
int connectionIndex = (fromR < toR) ? fromR : toR;
outConnType = horizontalConn[fromF][connectionIndex];
return true;
}

bool canTraverse(int targetFloor, int targetRoom, char connType,
                 const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                 int emergencyTripsLeft, bool lift2Operational){
    if (targetFloor<0|| targetFloor>=NUM_FLOORS||targetRoom<0|| targetRoom>=ROOMS_PER_FLOOR
 )
{
    return false;
}
if (connType == CONN_DOOR || connType == CONN_WINDOW)
{
    return true;
}

if (connType == CONN_SOLID_WALL || connType == CONN_NONE)
{
    return false;
}
if (connType == CONN_LOCKED_DOOR)
{
    if (hasItem(inventory, inventoryCount, ENT_MASTER_KEY))
    {
        return true;
    }
    else
    {
        return false;
    }
}
return false;

}
void applyResourceDelta(int& energy, int& time,
                         int deltaEnergy, int deltaTime){
   energy = energy + deltaEnergy;   
   time = time + deltaTime;
   energy = clampInt(energy, 0, MAX_ENERGY);
time = clampInt(time, 0, MAX_TIME);                      
                         }
                         void applyScoreDelta(int& score, int deltaScore){
                            score =score+deltaScore;
                         }
                         bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY],
             int inventoryCount, char itemSymbol){
    for (int i = 0; i < inventoryCount; i++) {
        if (inventory[i] == itemSymbol)
{
    return true;
}
    }
return false;
             }
bool addItem(char inventory[MAX_INVENTORY_CAPACITY],
             int& inventoryCount, char itemSymbol){
    if (inventoryCount >= MAX_INVENTORY_CAPACITY)
{
    return false;
}

    inventory[inventoryCount] = itemSymbol;
    inventoryCount++;

    return true;
             }
    bool removeItem(char inventory[MAX_INVENTORY_CAPACITY],
                int& inventoryCount, char itemSymbol){
 for (int i = 0; i < inventoryCount; i++)
{
    if (inventory[i] == itemSymbol)
{
    for (int j = i; j < inventoryCount - 1; j++)
{
    inventory[j] = inventory[j + 1];
}
inventoryCount--;
return true;
}

}
return false;
                }

             
