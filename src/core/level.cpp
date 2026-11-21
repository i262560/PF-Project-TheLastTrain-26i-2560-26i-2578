#include "level.h"
#include "util.h"

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
