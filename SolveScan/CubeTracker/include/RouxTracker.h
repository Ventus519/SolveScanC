//
// Created by Ven on 2025/12/22.
//

#ifndef CUBE_REPRESENTATION_ROUXTRACKER_H
#define CUBE_REPRESENTATION_ROUXTRACKER_H

#pragma once
#include "../Cube3/Cube3.h"
#include "MoveStack.h"

#include <stdlib.h>

typedef enum RouxMilestones
{
    ROUX_SCRAMBLE,
    ROUX_INSPECT,
    ROUX_FIRST_BLOCK,
    ROUX_SECOND_BLOCK,
    ROUX_LAST_LAYER_CORNERS,
    ROUX_LAST_SIX_EDGES,
    ROUX_SOLVED,
    ROUX_MILESTONE_NULL = -1
} RouxMilestones;

typedef struct RouxTracker
{
    RouxMilestones STEP;

    Cube CUBE;
} RouxTracker;

int initialize_RouxTracker(RouxTracker* tracker);
void free_RouxTracker(RouxTracker* tracker);

int update_current_step_ROUX(RouxTracker* tracker, int continue_scramble, int continue_inspect);

#endif //CUBE_REPRESENTATION_ROUXTRACKER_H