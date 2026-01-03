//
// Created by Ven on 2025/12/22.
//

#ifndef CUBE_REPRESENTATION_ROUXTRACKER_H
#define CUBE_REPRESENTATION_ROUXTRACKER_H

#include "../Cube3/Cube3.h"

typedef enum
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

typedef struct
{
    RouxMilestones STEP;
    char* MOVES_APPLIED;
    int MOVES_APPLIED_SIZE;
    int MOVES_APPLIED_MAX;

    Cube* CUBE;
} RouxTracker;

RouxTracker* create_RouxTracker();
void free_RouxTracker(RouxTracker* tracker);
Cube* get_cube(const RouxTracker* tracker);

int is_block_complete(const RouxTracker* tracker, Faces FACE_LEFT_OR_FACE_RIGHT);

int is_first_block_complete(const RouxTracker* tracker);
int is_second_block_complete(const RouxTracker* tracker);

int last_layer_corners_aligned(const RouxTracker* tracker);

int update_current_step(RouxTracker* tracker, int continue_scramble, int continue_inspect);
int apply_moves(RouxTracker* tracker, char** SEQUENCE);

int resize_moves_applied(RouxTracker* tracker);
int normalize_moves_applied(RouxTracker* tracker);

void print_tracker_state(const RouxTracker* tracker);


#endif //CUBE_REPRESENTATION_ROUXTRACKER_H