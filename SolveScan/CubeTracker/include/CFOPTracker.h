//
// Created by Ven519 on 2026/01/12.
//

#ifndef SOLVESCAN_CFOPTRACKER_H
#define SOLVESCAN_CFOPTRACKER_H


#include <stdint.h>

#include "../Cube3/Cube3.h"

typedef enum CFOP_MILESTONES
{
    CFOP_SCRAMBLE,
    CFOP_INSPECT,
    CFOP_CROSS,
    CFOP_F2L_1,
    CFOP_F2L_2,
    CFOP_F2L_3,
    CFOP_F2L,
    CFOP_OLL,
    CFOP_PLL,
    CFOP_SOLVED,
    CFOP_MILESTONE_NULL = -1
} CFOPMilestones;

typedef struct CFOP_TRACKER {
    Cube CUBE;
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
    CFOPMilestones STEP;
} CFOPTracker;

int initialize_CFOPTracker(CFOPTracker* tracker);
void free_CFOPTracker(CFOPTracker* tracker);

//CFOP STEPS: CROSS, F2L, OLL, PLL
int is_cross_complete_CFOP(const CFOPTracker* tracker);
int count_complete_f2l_pairs(const CFOPTracker* tracker);
int is_f2l_pair_complete_CFOP(const CFOPTracker* tracker, Faces FACE_FRONT_OR_FACE_BACK, Faces FACE_RIGHT_OR_FACE_LEFT);
int update_completed_f2l_pairs(CFOPTracker* tracker);

int is_oll_complete(const CFOPTracker* tracker);

int update_current_step_CFOP(CFOPTracker* tracker, int continue_scramble, int continue_inspect);

#endif //SOLVESCAN_CFOPTRACKER_H