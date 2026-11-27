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
     for (int j = 0; j < MAX_INVENTORY_CAPACITY; j++) {
    file << inventory[j] << "\n";
   }
   for (int j = 0; j < NUM_OBJECTIVES; j++) {
    file << objectives[j] << "\n";
   }
    for (int f = 0; f < NUM_FLOORS; f++) {
    for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
        file << entityCounts[f][r] << "\n";
    }
   }
   for (int f = 0; f < NUM_FLOORS; f++) {
    for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
        for (int k = 0; k < MAX_ENTITIES_PER_ROOM; k++) {
            file << roomEntities[f][r][k] << "\n";
        }

    }
  }
    file.close();
   return true;
    }
    bool loadGameStateFromFile(const char* filename,
                           int& playerFloor, int& playerRoom,
                           int& energy, int& time, int& score,
                           char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                           bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                           char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                           int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){
   ifstream file(filename);
     if (!file) {
    return false;
  }
    file >> playerFloor;
    file >> playerRoom;
    file >> energy;
    file >> time;
    file >> score;
    file >> inventoryCount;
    file >> emergencyTripsLeft;
      if (!file) {
    return false;
   }
    char oneline[MAX_LINE_LEN];
    file.getline(oneline, MAX_LINE_LEN);
      for (int j=0; j<MAX_INVENTORY_CAPACITY;j++) {
        file.getline(oneline, MAX_LINE_LEN);
        inventory[j]=oneline[0];
      }
      for (int k = 0; k < NUM_OBJECTIVES; k++) {
    file >> objectives[k];
  }
for (int f = 0; f < NUM_FLOORS; f++) {
    for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
        file >> entityCounts[f][r];
    }
  }
    file.getline(oneline, MAX_LINE_LEN);
  for (int f = 0; f < NUM_FLOORS; f++) {
    for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
        for (int k = 0; k < MAX_ENTITIES_PER_ROOM; k++) {
            file.getline(oneline, MAX_LINE_LEN);
            roomEntities[f][r][k] = oneline[0];
        }
    }
  }
   if (!file) {
    return false;
 }
   if (playerFloor < 1 || playerFloor > NUM_FLOORS) {
    return false;
}
if (playerRoom < 1 || playerRoom > ROOMS_PER_FLOOR) {
    return false;
}
if (inventoryCount < 0 || inventoryCount > MAX_INVENTORY_CAPACITY) {
    return false;
}
 for (int f = 0; f < NUM_FLOORS; f++) {
    for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
        if (entityCounts[f][r] < 0 || entityCounts[f][r] > MAX_ENTITIES_PER_ROOM) {
            return false;
        }
    }
}
   file.close();
   return true;
 }
   void writeGameSummaryReport(const char* filename,
                            int score, int remainingEnergy, int remainingTime,
                            const bool objectives[NUM_OBJECTIVES], bool victory) {
    ofstream file(filename);
   if (!file) {
    return;
  }    
      file << "THE LAST TRAIN - GAME SUMMARY\n";
  if (victory) {
    file << "Result: VICTORY"<<endl;
  } else {
    file << "Result: DEFEAT"<<endl;
  }
 file << "Final Score: " << score <<endl;
 file << "Remaining Energy: " << remainingEnergy <<endl;
 file << "Remaining Time: " << remainingTime <<endl;
 file << "Objectives:"<<endl; 

  file << "Master Key Collected: ";
if (objectives[OBJ_MASTER_KEY]) {
    file << "Yes"<<endl;
} else {
    file << "No"<<endl;
} 

 file << "Clue Digit 4 found: ";
if (objectives[OBJ_CLUE_4]) {
    file << "Yes"<<endl;
} else {
    file << "No"<<endl;
}    
                  
 file << "Clue Digit 2 found: ";
if (objectives[OBJ_CLUE_2]) {
    file << "Yes"<<endl;
} else {
    file << "No"<<endl;
}    
                  
 file << "Clue Digit 7 found:";
if (objectives[OBJ_CLUE_7]) {
    file << "Yes"<<endl;
} else {
    file << "No"<<endl;
}    
                  
 file << "Cabinet Unlocked: ";
if (objectives[OBJ_CABINET_UNLOCKED]) {
    file << "Yes"<<endl;
} else {
    file << "No"<<endl;
}    
                  
 file << "Train Key Acquired: ";
if (objectives[OBJ_TRAIN_KEY]) {
    file << "Yes"<<endl;
} else {
    file << "No"<<endl;
}    
                  
 file << "Fuse Installed: ";
if (objectives[OBJ_FUSE_INSTALLED]) {
    file << "Yes"<<endl;
} else {
    file << "No"<<endl;
}    
                  
 file << "Lift Pass Collected: ";
if (objectives[OBJ_LIFT_PASS]) {
    file << "Yes"<<endl;
} else {
    file << "No"<<endl;
}  

 file << "Passenger rescued: ";
if (objectives[OBJ_PASSENGER_RESCUED]) {
    file << "Yes"<<endl;
} else {
    file << "No"<<endl;
}  
                  
 file << "Cat sescued: ";
if (objectives[OBJ_CAT_SECURED]) {
    file << "Yes"<<endl;
} else {
    file << "No"<<endl;
}    
                  
 file << "Train prepared and loaded: ";
if (objectives[OBJ_TRAIN_PREPARED]) {
    file << "Yes"<<endl;
} else {
    file << "No"<<endl;
}    
                   
                  
                                            
                           
                           
                           
                           
                           
                            }



