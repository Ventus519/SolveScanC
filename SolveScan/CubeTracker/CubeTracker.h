//
// Created by Ven519 on 2026/01/11.
//

#ifndef SOLVESCAN_CUBETRACKER_H
#define SOLVESCAN_CUBETRACKER_H

#include "RouxTracker.h"
#include "CFOPTracker.h"

typedef enum TRACKERS_ENABLED {
    BEGINNERS,
    CFOP,
    ROUX,
    ZZ
} EnabledTrackers;


typedef struct CUBE_TRACKER {
    MoveStack* p_MOVES_APPLIED;

    char* reconstruction;
    size_t reconstruction_size;
    size_t reconstruction_max;

    EnabledTrackers ENABLED;
    RouxTracker* tracker_ROUX;
    CFOPTracker* tracker_CFOP;

    char* save_file_path;

} CubeTracker;

int initialize_CubeTracker(CubeTracker* tracker, const char* file_path, EnabledTrackers TRACKER_TO_ENABLE);

int is_invalid_CubeTracker(const CubeTracker* tracker);

int track_scramble(CubeTracker* tracker, const MoveStack* SCRAMBLE_SEQUENCE);

int is_subtracker_cube_solved(const CubeTracker* tracker);
int apply_move_to_subtracker_cubes(CubeTracker* tracker, const MoveSpec* MOVE_SPEC);
int track_move_from_spec(CubeTracker* tracker, const MoveSpec* MOVE_SPEC);
int backtrack_moves(CubeTracker* tracker, int count);

int resize_reconstruction(CubeTracker* tracker);
int append_to_reconstruction(CubeTracker* tracker, const char* string_entry);

CFOPMilestones get_current_CFOP_step(const CubeTracker* tracker);
RouxMilestones get_current_ROUX_step(const CubeTracker* tracker);

#endif //SOLVESCAN_CUBETRACKER_H