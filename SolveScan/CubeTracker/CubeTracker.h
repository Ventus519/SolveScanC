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

CubeTracker* create_CubeTracker(char* file_path);
int create_trackers(CubeTracker* tracker);

void free_CubeTracker(CubeTracker* tracker);
void free_trackers(CubeTracker* tracker);

int resize_reconstruction(CubeTracker* tracker);

int update_reconstruction(CubeTracker* tracker);
int read_reconstruction_from_file(CubeTracker* tracker);
int write_reconstruction_to_file(CubeTracker* tracker);

int update_ROUX_reconstruction(RouxTracker* ROUX_TRACK);

#endif //SOLVESCAN_CUBETRACKER_H