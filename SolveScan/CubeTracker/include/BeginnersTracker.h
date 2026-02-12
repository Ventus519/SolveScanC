//
// Created by Ven519 on 2026/01/22.
//

#ifndef SOLVESCAN_BEGINNERSTRACKER_H
#define SOLVESCAN_BEGINNERSTRACKER_H

#include "../Cube3/Cube3.h"

typedef enum BEGINNERS_MILESTONES
{
    BEGINNERS_SCRAMBLE,
    BEGINNERS_INSPECT,
    BEGINNERS_CROSS,
    BEGINNERS_FIRST_LAYER,
    BEGINNERS_SECOND_LAYER,
    BEGINNERS_ORIENT_OPPOSITE_CROSS,
    BEGINNERS_PERMUTE_OPPOSITE_CROSS,
    BEGINNERS_PERMUTE_OPPOSITE_CORNERS,
    BEGINNERS_ORIENT_OPPOSITE_CORNERS,
    BEGINNERS_SOLVED,
    BEGINNERS_MILESTONE_NULL
} BeginnersMilestones;

typedef struct BEGINNERS_TRACKER
{
    Cube CUBE;
    BeginnersMilestones STEP;
} BeginnersTracker;

int initialize_BeginnersTracker(BeginnersTracker* tracker);
void free_BeginnersTracker(BeginnersTracker* tracker);

int update_current_step_BEGINNERS(BeginnersTracker* tracker, int continue_scramble, int continue_inspect);

#endif //SOLVESCAN_BEGINNERSTRACKER_H