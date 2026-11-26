#include "level.h"
#include "util.h"
#include <fstream>
using namespace std;

// Modular implementation using primitive arrays for map parsing and verification
const char* getEntityName(char entitySymbol) {
    switch (entitySymbol) {
        case ENT_MASTER_KEY: return "Master key";
        case ENT_ENERGY_PACK: return "Energy Pack";
	    case ENT_EMERGENCY_TOOLKIT: return "Emergency Toolkit";
	    case ENT_CLUE_4:	return "Clue Digit 4";
        case ENT_FUSE:	return "Fuse";
	    case ENT_CAT:	return "Cat";
	    case ENT_CONTROL_PANEL:	return "Control Panel";
        case ENT_LIFT_PASS: return	"Lift Pass";
	    case ENT_PASSENGER: return	"Passenger";
	    case ENT_MEDICAL_KIT: return	"Medical Kit";
        case ENT_EMERGENCY_CARD:	return "Emergency Access Card";
	    case ENT_CLUE_2: return "Clue Digit 2";
	    case ENT_CLUE_7:	return "Clue Digit 7";
	    case ENT_CABINET_TRAIN_KEY:	return "Cabinet / Train Key";
	    case ENT_CAT_CARRIER:	return "Cat Carrier";
	    case ENT_OBSERVATION_TOKEN: return "Observation Token";
         default:	return "Unknown";
    }
}
  int textToInt(const char* text){
	int result=0;
	int i=0;
	while (text[i] >= '0' && text[i] <= '9') {
    result = result * 10 + (text[i] - '0');
    i++;
	}
	return result;

 }


bool loadInitialState(const char* filename,int& playerFloor, int& playerRoom, int& energy, int& time, int& score, char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft) {
    playerFloor=1;
	playerRoom=1;
	energy=INITIAL_ENERGY;
	time=INITIAL_TIME;
	score=INITIAL_SCORE;
	inventoryCount= 0;
    emergencyTripsLeft= 0;
	  for (int i=0;i<NUM_OBJECTIVES;i++) {
		objectives[i]= false;
	  }
	  for (int i = 0; i < MAX_INVENTORY_CAPACITY; i++) {
    inventory[i] = ENT_NONE;
}
	  ifstream file(filename);
	   if (!file) {
		return false;
	   }
	   char line[MAX_LINE_LEN];
        while (file.getline(line, MAX_LINE_LEN)) {
			trimWhitespace(line);
			if (line[0]=='\0') {
				continue;
			}
			int colon = -1;
			for (int k=0;line[k] !='\0';k++) {
				if (line[k] == ':') {
                colon = k;
				break;
				}
			}
				if (colon == -1) {
             continue;
      }
	      line[colon] = '\0';
       char* name = line;
       char* value = line + colon + 1;
	   trimWhitespace(name);
       trimWhitespace(value);
       if (stringEquals(name, "INITIAL_ENERGY")) {
    energy = textToInt(value);
   }
   if (stringEquals(name, "INITIAL_TIME")) {
    time = textToInt(value);
  }
  if (stringEquals(name, "INITIAL_SCORE")) {
    score = textToInt(value);
   }
   if (stringEquals(name, "PASSENGER_RESCUED")) {
    objectives[OBJ_PASSENGER_RESCUED] = stringEquals(value, "YES");
 }
  if (stringEquals(name, "CAT_RESCUED")) {
    objectives[OBJ_PASSENGER_RESCUED] = stringEquals(value, "YES");
 }
  if (stringEquals(name, "CABINET_UNLOCKED")) {
    objectives[OBJ_CABINET_UNLOCKED] = stringEquals(value, "YES");
 }
  if (stringEquals(name, "TRAIN_KEY_UNLOCKED")) {
    objectives[OBJ_TRAIN_KEY] = stringEquals(value, "YES");
 }
  if (stringEquals(name, "FUSE_INSTALLED")) {
    objectives[OBJ_FUSE_INSTALLED] = stringEquals(value, "YES");
 }
    if (stringEquals(name, "LIFT_PASS_COLLECTED")) {
    objectives[OBJ_LIFT_PASS] = stringEquals(value, "YES");
 }
   if (stringEquals(name, "TRAIN_PREPARED")) {
    objectives[OBJ_TRAIN_PREPARED] = stringEquals(value, "YES");
 }
      
			}
		
	   file.close();
	   return true;
 }
 bool validateStartingRoom(int startFloor, int startRoom, const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1]){
  if (startFloor<1 || startFloor>NUM_FLOORS) {
	   return false;
  }
  int f = startFloor-1;
  bool leftBlocked = (startRoom==1 || horizontalConn[f][startRoom-2] == CONN_SOLID_WALL);
   bool rightBlocked = (startRoom == ROOMS_PER_FLOOR || horizontalConn[f][startRoom-1] == CONN_SOLID_WALL);
   if (leftBlocked==false && rightBlocked==false) {
    return false;
}
return true;

  }
  bool loadBuildingMap(const char* filename,
	         char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1], 
	          char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM], int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){
   for (int f = 0;f<NUM_FLOORS; f++ ) {
     for (int r = 0;r<ROOMS_PER_FLOOR-1 ; r++) {
		  horizontalConn[f][r] = CONN_NONE;

	 }
	  for (int r=0; r<ROOMS_PER_FLOOR; r++) {
		entityCounts[f][r] = 0;
		 for (int j = 0; j < MAX_ENTITIES_PER_ROOM; j++) {
            roomEntities[f][r][j] = ENT_NONE;
        }
    }
}
  ifstream file(filename);
  if (!file) {
    return false;
}
  int floorsLoaded = 0;
  char line[MAX_LINE_LEN];
  char itemLine[MAX_LINE_LEN];
    while (file.getline(line, MAX_LINE_LEN)) {

}
file.close();
if (floorsLoaded != NUM_FLOORS) {
    return false;
}
return true;

   }