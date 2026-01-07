//
// Created by Ven519 on 2026/01/03.
//

#include "CubeTracker.h"
#include "RouxTracker.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

CubeTracker* create_CubeTracker(char* file_path)
{
    if (!file_path)
    {
        return NULL;
    }
    CubeTracker* tracker = malloc(sizeof(CubeTracker));
    if (!tracker)
    {
        goto CUBE_TRACKER_ALLOC_FAIL;
    }
    if (create_trackers(tracker))
    {
        goto SUB_TRACKER_ALLOC_FAIL;
    }
    tracker -> RECONSTRUCTION = "SCRAMBLE: \0"; //assumes default state is scramble
    tracker -> RECONSTRUCTION_size = 11; //size of "SCRAMBLE: \0"
    tracker -> RECONSTRUCTION_max = 22; //2 * (RECONSTRUCTION_size) for safety
    tracker -> save_file_path = file_path;

    return tracker;
    SUB_TRACKER_ALLOC_FAIL:
        free_CubeTracker(tracker);
    CUBE_TRACKER_ALLOC_FAIL:
        return NULL;
}

int create_trackers(CubeTracker* tracker)
{
    if (!tracker)
    {
        return 1;
    }
    RouxTracker* ROUX = create_RouxTracker();
    if (!ROUX)
    {
        return 1;
    }
    tracker -> tracker_ROUX = ROUX;
    return 0;
}

void free_CubeTracker(CubeTracker* tracker)
{
    if (!tracker)
    {
        return;
    }
    free_trackers(tracker);
    free(tracker -> RECONSTRUCTION);

    free(tracker);
}

void free_trackers(CubeTracker* tracker)
{
    if (!tracker)
    {
        return;
    }

    if (tracker -> tracker_ROUX != NULL)
    {
        free_RouxTracker(tracker -> tracker_ROUX);
    }
    //this space is for other trackers when their source files have been created and tested

}

int resize_reconstruction(CubeTracker* tracker)
{
    if (!tracker || !tracker -> RECONSTRUCTION)
    {
        return 1;
    }

    const size_t new_size = tracker -> RECONSTRUCTION_max * 2;
    char* p = malloc(sizeof(char)* new_size);
    if (!p)
    {
        return 1;
    }
    memcpy(p, tracker -> RECONSTRUCTION, tracker -> RECONSTRUCTION_size);
    tracker -> RECONSTRUCTION = p;
    tracker -> RECONSTRUCTION_max = new_size;
    return 0;
}

int update_reconstruction(CubeTracker* tracker)
{
    if (!tracker || !tracker -> RECONSTRUCTION)
    {
        return 1;
    }

    if (tracker -> tracker_ROUX != NULL)
    {
        return update_ROUX_reconstruction(tracker -> tracker_ROUX);
    }

    return 0;
}

int update_ROUX_reconstruction(RouxTracker* ROUX_TRACK)
{
    if (!ROUX_TRACK)
    {
        return 1;
    }
    //need to actually implement the updating
    /*
     *Current plan: make the update reconstruction methods take the move sequence as a parameter
     *Check the milestone before the sequence and compare it as each move is applied
     *If the move sequence results in a different milestone, then edit it at the point of concern
     *
     *this will likely feel very redundant since I already have an apply move sequence function in Cube.c
     *
     */

    return 0;
}