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
    FILE* file = fopen(file_path, "wr");
    if (!file)
    {
        return NULL;
    }

    CubeTracker* tracker = malloc(sizeof(CubeTracker));
    if (!tracker)
    {
        goto CUBETRACKER_ALLOC_FAIL;
    }
    tracker -> save_file = file;

    if (create_trackers(tracker))
    {
        goto TRACKER_ALLOC_FAIL;
    }

    tracker -> RECONSTRUCTION_max = 3;
    char* reconstruct = malloc(sizeof(char) * tracker -> RECONSTRUCTION_max);
    if (!reconstruct)
    {
        goto RECONSTRUCT_ALLOC_FAIL;
    }
    reconstruct[0] = '\0';
    tracker -> RECONSTRUCTION = reconstruct;

    return tracker;

    RECONSTRUCT_ALLOC_FAIL:
        free_trackers(tracker);
    TRACKER_ALLOC_FAIL:
        free(tracker);
    CUBETRACKER_ALLOC_FAIL:
        fclose(file);
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
    fclose(tracker -> save_file);
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