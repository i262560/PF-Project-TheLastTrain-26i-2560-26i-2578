#include <iostream>
#include "level.h"
#include "logger.h"

int main() {
    char conn[NUM_FLOORS][ROOMS_PER_FLOOR - 1];
    char ent[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM];
    int cnt[NUM_FLOORS][ROOMS_PER_FLOOR];
    loadBuildingMap("data/building.txt", conn, ent, cnt);

    char inv[MAX_INVENTORY_CAPACITY];
    for (int i = 0; i < MAX_INVENTORY_CAPACITY; i++) inv[i] = ENT_NONE;
    inv[0] = ENT_MASTER_KEY;
    bool obj[NUM_OBJECTIVES] = { false };
    obj[OBJ_CLUE_4] = true;

    saveGameStateToFile("out/test_save.txt", 3, 2, 77, 33, 40, inv, 1, obj, 2, ent, cnt);

    int fl = 0, rm = 0, en = 0, tm = 0, sc = 0, ic = 0, trips = 0;
    char inv2[MAX_INVENTORY_CAPACITY];
    bool obj2[NUM_OBJECTIVES] = { false };
    char ent2[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM];
    int cnt2[NUM_FLOORS][ROOMS_PER_FLOOR];

    bool ok = loadGameStateFromFile("out/test_save.txt", fl, rm, en, tm, sc, inv2, ic, obj2, trips, ent2, cnt2);

    std::cout << "load ok: " << ok << "\n";
    std::cout << fl << " " << rm << " " << en << " " << tm << " " << sc << " " << ic << " " << trips << "\n";
    std::cout << "inv0: " << inv2[0] << "  clue4: " << obj2[OBJ_CLUE_4] << "\n";
    std::cout << "F3-R2 first item: " << ent2[2][1][0] << "\n";
    return 0;
}