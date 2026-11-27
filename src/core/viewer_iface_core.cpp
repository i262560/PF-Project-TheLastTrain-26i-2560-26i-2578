#include "viewer_iface.h"
#include "util.h"
#include <iostream>
using namespace std;
void renderDashboard(int playerFloor, int playerRoom, int energy, int time, int score,
                     const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                     int emergencyTripsLeft){
                      
    cout << "\nTHE LAST TRAIN \n";

    cout << "Floor: " << playerFloor << endl;
    cout << "Room: " << playerRoom << endl;
    cout << "Energy: " << energy << endl;
    cout << "Time: " << time << endl;
    cout << "Score: " << score << endl;

    cout << "Inventory: ";

    if (inventoryCount == 0)
    {
        cout << "Empty";
    }
    else
    {
        for (int i = 0; i < inventoryCount; i++)
        {
            cout << inventory[i] << " ";
        }
    }

    cout << endl;
    cout << "Emergency trips left: " << emergencyTripsLeft << endl;

}
                     

void renderRoomDetails(int floor, int room,
                       const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                       int entityCount)
{
    cout << "\nRoom Details \n";
    cout << "Floor: " << floor + 1 << endl;
    cout << "Room: " << room + 1 << endl;

    if (entityCount == 0)
    {
        cout << "This room is empty.\n";
    }
    else
    {
        cout << "Entities in this room: ";

        for (int i = 0; i < entityCount; i++)
        {
            cout << roomEntities[floor][room][i] << " ";
        }

        cout << endl;
    }
}
                       
void renderAsciiBuilding(int playerFloor, int playerRoom){

    cout << "\nBUILDING MAP \n";

    for (int floor = 1; floor <= NUM_FLOORS; floor++)
    {
        cout << "Floor " << floor << ": ";

        for (int room = 1; room <= ROOMS_PER_FLOOR; room++)
        {
            if (floor == playerFloor && room == playerRoom)
            {
                cout << "[ P ] ";
            }
            else
            {
                cout << "[ " << room << " ] ";
            }
        }

        cout << endl;
    }

    cout << "\n";
}
void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES]){
 const char* objectiveNames[NUM_OBJECTIVES] =
    {
        "Collect Master Key",
        "Discover Clue 4",
        "Discover Clue 2",
        "Discover Clue 7",
        "Unlock Security Cabinet",
        "Collect Train Key",
        "Collect Fuse",
        "Install Fuse",
        "Collect Lift Pass",
        "Activate Lift 2",
        "Rescue Passenger",
        "Collect Cat Carrier",
        "Secure Cat",
        "Prepare Train",
        "Win Mission",
        "Mission Failed"
    };

    cout << "\n OBJECTIVES \n";

    for (int i = 0; i < NUM_OBJECTIVES; i++)
    {
        if (objectives[i])
        {
            cout << "[X] ";
        }
        else
        {
            cout << "[ ] ";
        }

        cout << objectiveNames[i] << endl;
    }

    cout << "\n";
}



