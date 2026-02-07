//
// Created by Ven519 on 2026/01/11.
//

#ifndef SOLVESCAN_CUBETRACKER_H
#define SOLVESCAN_CUBETRACKER_H

#include "include/RouxTracker.h"
#include "include/CFOPTracker.h"
#include "include/BeginnersTracker.h"
#include "include/ZZTracker.h"

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
    BeginnersTracker* tracker_BEGINNERS;
    ZZTracker* tracker_ZZ;


    char* save_file_path;
    size_t SCRAMBLE_MOVE_LENGTH;

} CubeTracker;

int initialize_CubeTracker(CubeTracker* tracker, const char* file_path, EnabledTrackers TRACKER_TO_ENABLE);

int is_invalid_CubeTracker(const CubeTracker* tracker);

int track_scramble(CubeTracker* tracker, const MoveStack* SCRAMBLE_SEQUENCE);

int is_subtracker_cube_solved(const CubeTracker* tracker);
int track_MoveStack(CubeTracker* tracker, const MoveStack* MOVES);
int apply_move_to_subtracker_cubes(CubeTracker* tracker, const MoveSpec* MOVE_SPEC);
int track_move_from_spec(CubeTracker* tracker, const MoveSpec* MOVE_SPEC);
int backtrack_moves(CubeTracker* tracker, int count);

int resize_reconstruction(CubeTracker* tracker);
int append_to_reconstruction(CubeTracker* tracker, const char* string_entry);
int update_reconstruction(CubeTracker* tracker, int continue_inspect);


int update_current_step(CubeTracker* tracker, int continue_scramble, int continue_inspect);
CFOPMilestones get_current_CFOP_step(const CubeTracker* tracker);
RouxMilestones get_current_ROUX_step(const CubeTracker* tracker);
BeginnersMilestones get_current_BEGINNERS_step(const CubeTracker* tracker);
ZZMilestones get_current_ZZ_step(const CubeTracker* tracker);

#endif //SOLVESCAN_CUBETRACKER_H