#include "logger.h"
#include "util.h"
#include <fstream>
using namespace std;
bool saveGameStateToFile(const char* filename,
                         int playerFloor, int playerRoom,
                         int energy, int time, int score,
                         const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                         const bool objectives[NUM_OBJECTIVES], int emergencyTripsLeft,
                         const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                         const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){
    
                         
    ofstream file(filename);
     if(!file) {
        return false;
     }
      file << playerFloor << "\n";
     file << playerRoom << "\n";
      file << energy << "\n";
     file << time << "\n";
    file << score << "\n";
    file << inventoryCount << "\n";
    file << emergencyTripsLeft << "\n";
    file.close();
   return true;
    }



