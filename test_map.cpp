#include <iostream>
#include "level.h"

int main() {
    char conn[NUM_FLOORS][ROOMS_PER_FLOOR - 1];
    char ent[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM];
    int cnt[NUM_FLOORS][ROOMS_PER_FLOOR];
    if (!loadBuildingMap("data/building.txt", conn, ent, cnt)) {
        std::cout << "LOAD FAILED\n";
        return 1;
    }
    for (int f = NUM_FLOORS - 1; f >= 0; f--) {
        std::cout << "Floor " << f + 1 << " gaps: ";
        for (int r = 0; r < ROOMS_PER_FLOOR - 1; r++) {
            std::cout << "[" << conn[f][r] << "]";
        }
        std::cout << "\n";
        for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
            std::cout << "  R" << r + 1 << ": ";
            for (int e = 0; e < cnt[f][r]; e++) {
                std::cout << ent[f][r][e] << " ";
            }
            std::cout << "\n";
        }
    }
    return 0;
}