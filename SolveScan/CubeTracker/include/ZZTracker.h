//
// Created by Ven519 on 2026/01/31.
//

#ifndef SOLVESCAN_ZZTRACKER_H
#define SOLVESCAN_ZZTRACKER_H

#include "Cube3/Cube3.h"

typedef enum ZZ_MILESTONES
{
    ZZ_SCRAMBLE,
    ZZ_INSPECT,
    ZZ_EO_LINE,
    ZZ_F2L,
    ZZ_LAST_LAYER,
    ZZ_SOLVED,
    ZZ_MILESTONE_NULL = -1
} ZZMilestones;

typedef struct ZZ_TRACKER {
    ZZMilestones STEP;
    Cube CUBE;
} ZZTracker;


#endif //SOLVESCAN_ZZTRACKER_H