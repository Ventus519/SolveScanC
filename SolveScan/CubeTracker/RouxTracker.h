//
// Created by Ven on 2025/12/22.
//

#ifndef CUBE_REPRESENTATION_ROUXTRACKER_H
#define CUBE_REPRESENTATION_ROUXTRACKER_H

#pragma once
#include "../Cube3/Cube3.h"

#include <stdlib.h>

typedef enum RouxMilestones
{
    SCRAMBLE,
    INSPECT,
    FIRST_BLOCK,
    SECOND_BLOCK,
    LAST_LAYER_CORNERS,
    LAST_SIX_EDGES,
    SOLVED,
    MILESTONE_NULL
} RouxMilestones;

typedef struct MOVE_STACK {
    MoveSpec* MOVE_SEQUENCE;
    size_t MOVE_SEQUENCE_LENGTH;
    size_t MOVE_SEQUENCE_MAX;
} MoveStack;

typedef struct RouxTracker
{
    RouxMilestones STEP;

    MoveStack* APPLIED;

    Cube* CUBE;
} RouxTracker;

RouxTracker* create_RouxTracker();
void free_RouxTracker(RouxTracker* tracker);
Cube* get_cube(const RouxTracker* tracker);

MoveStack* create_MoveStack();
void free_MoveStack(MoveStack* stack);
int push_move_to_MoveStack(MoveStack* stack, MoveSpec* MOVE);
int resize_MoveStack(MoveStack* stack);
MoveSpec pop_move_from_MoveStack(MoveStack* stack);
char* MoveStack_to_str(MoveStack* stack);

int is_block_complete(const RouxTracker* tracker, Faces FACE_LEFT_OR_FACE_RIGHT);

int is_first_block_complete(const RouxTracker* tracker);
int is_second_block_complete(const RouxTracker* tracker);

int last_layer_corners_aligned(const RouxTracker* tracker);

int update_current_step(RouxTracker* tracker, int continue_scramble, int continue_inspect);
//int apply_moves(RouxTracker* tracker, char** SEQUENCE);
int track_applied_move(RouxTracker* tracker, char** FORMATTED_MOVE);

int backtrack_moves(RouxTracker* tracker, int amount_backtracked);

int resize_moves_applied(RouxTracker* tracker);
int normalize_moves_applied(RouxTracker* tracker);

void print_tracker_state(const RouxTracker* tracker);


#endif //CUBE_REPRESENTATION_ROUXTRACKER_H