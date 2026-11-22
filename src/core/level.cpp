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
	   file.close();
	   return true;
 }
 
