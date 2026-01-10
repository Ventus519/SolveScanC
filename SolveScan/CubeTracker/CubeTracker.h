//
// Created by Ven519 on 2026/01/03.
//

#ifndef SOLVESCAN_CUBETRACKER_H
#define SOLVESCAN_CUBETRACKER_H

#pragma once
#include "RouxTracker.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    RouxTracker* tracker_ROUX;

    char* RECONSTRUCTION;
    size_t RECONSTRUCTION_max;
    size_t RECONSTRUCTION_size;

    char* save_file_path;
} CubeTracker;

typedef enum ENABLED_TRACKERS {
    BEGINNERS_CFOP,
    CFOP,
    ROUX,
    ZZ
} EnabledTrackers;

CubeTracker* create_CubeTracker(char* file_path);
int create_trackers(CubeTracker* tracker, EnabledTrackers USER_ENABLED_TRACKER);

void free_CubeTracker(CubeTracker* tracker);
void free_trackers(CubeTracker* tracker);

int resize_reconstruction(CubeTracker* tracker);
int append_to_reconstruction(CubeTracker* tracker, const char* ENTRY);

int update_reconstruction(CubeTracker* tracker, char** FORMATTED_MOVE_TO_APPLY);
int read_reconstruction_from_file(CubeTracker* tracker);
int write_reconstruction_to_file(CubeTracker* tracker);

int update_ROUX_reconstruction(CubeTracker* TRACK, char** FORMATTED_MOVE_TO_APPLY);

#endif //SOLVESCAN_CUBETRACKER_H