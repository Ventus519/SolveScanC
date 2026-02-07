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
     * A bit value of 1 represents that the pair was solved at some point during the solve
     */
    uint8_t f2l_pairs_completed;

    Cube CUBE;
} ZZTracker;

int initialize_ZZTracker(ZZTracker* tracker);
void free_ZZTracker(ZZTracker* tracker);

int is_edge_orientation_complete(const ZZTracker* tracker);

int is_cross_complete_ZZ(const ZZTracker* tracker);

int count_complete_f2l_pairs_ZZ(const ZZTracker* tracker);

int is_f2l_pair_complete_ZZ(const ZZTracker* tracker, Faces FACE_FB, Faces FACE_RL);

int update_completed_f2l_pairs_ZZ(ZZTracker* tracker);

int is_ocll_complete(const ZZTracker* tracker);

int update_current_step_ZZ(ZZTracker* tracker, int continue_scramble, int continue_inspect);


#endif //SOLVESCAN_ZZTRACKER_H