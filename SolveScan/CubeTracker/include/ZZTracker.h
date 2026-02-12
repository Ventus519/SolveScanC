//
// Created by Ven519 on 2026/01/31.
//

#ifndef SOLVESCAN_ZZTRACKER_H
#define SOLVESCAN_ZZTRACKER_H

#include <stdint.h>

#include "../Cube3/Cube3.h"

typedef enum ZZ_MILESTONES
{
    ZZ_SCRAMBLE,
    ZZ_INSPECT,
    ZZ_EO_CROSS,
    ZZ_F2L_1,
    ZZ_F2L_2,
    ZZ_F2L_3,
    ZZ_F2L,
    ZZ_OCLL,
    ZZ_PLL,
    ZZ_SOLVED,
    ZZ_MILESTONE_NULL = -1
} ZZMilestones;

typedef struct ZZ_TRACKER {
    ZZMilestones STEP;
    /*
     * 4 bits of f2l_pairs_completed are used to determine if the pairs were solved at some point.
     * bit 3: BR F2L pair
     * bit 2: BL F2L pair
     * bit 1: FR F2L pair
     * bit 0: FL F2L pair
     *
     */
    uint8_t f2l_pairs_completed;

    Cube CUBE;
} ZZTracker;

int initialize_ZZTracker(ZZTracker* tracker); //all ZZ trackers are heap allocated
void free_ZZTracker(ZZTracker* tracker);

int update_current_step_ZZ(ZZTracker* tracker, int continue_scramble, int continue_inspect);


#endif //SOLVESCAN_ZZTRACKER_H