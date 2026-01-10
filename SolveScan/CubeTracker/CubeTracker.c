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
    if (create_trackers(tracker, ROUX))
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

int create_trackers(CubeTracker* tracker, const EnabledTrackers USER_ENABLED_TRACKER)
{
    if (!tracker)
    {
        return 1;
    }
    if (USER_ENABLED_TRACKER == ROUX)
    {
        RouxTracker* ROUX = create_RouxTracker();
        if (!ROUX)
        {
            return 1;
        }
        tracker -> tracker_ROUX = ROUX;
    }

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

int append_to_reconstruction(CubeTracker* tracker, const char* ENTRY)
{
    if (!tracker || !tracker -> RECONSTRUCTION || !ENTRY)
    {
        return 1;
    }
    while (strlen(ENTRY) + tracker -> RECONSTRUCTION_size > tracker -> RECONSTRUCTION_max)
    {
        if (resize_reconstruction(tracker))
        {
            return 1;
        }
    }
    strcat(tracker -> RECONSTRUCTION, ENTRY);
    tracker -> RECONSTRUCTION_size += strlen(ENTRY);
    return 0;
}

int update_reconstruction(CubeTracker* tracker, char** FORMATTED_MOVE_TO_APPLY)
{
    if (!tracker || !tracker -> RECONSTRUCTION || !FORMATTED_MOVE_TO_APPLY || !(*FORMATTED_MOVE_TO_APPLY))
    {
        return 1;
    }

    if (tracker -> tracker_ROUX != NULL)
    {
        return update_ROUX_reconstruction(tracker, FORMATTED_MOVE_TO_APPLY);
    }

    return 0;
}

int update_ROUX_reconstruction(CubeTracker* TRACK, char** FORMATTED_MOVE_TO_APPLY)
{
    if (!TRACK || !(TRACK -> tracker_ROUX) || !FORMATTED_MOVE_TO_APPLY || !(*FORMATTED_MOVE_TO_APPLY))
    {
        return 1;
    }

    const RouxMilestones CURRENT_STEP = TRACK -> tracker_ROUX -> STEP;

    if (track_applied_move_formatted_str(TRACK -> tracker_ROUX, FORMATTED_MOVE_TO_APPLY))
    {
        return 1;
    }
    if (update_current_step(TRACK -> tracker_ROUX, 0, 0))
    {
        return 1;
    }
    const RouxMilestones NEW_STEP = TRACK -> tracker_ROUX -> STEP;

    if (CURRENT_STEP != NEW_STEP)
    {
        append_to_reconstruction(TRACK, "\n");
        switch (NEW_STEP)
        {
            case SCRAMBLE: append_to_reconstruction(TRACK, "SCRAMBLE: "); break;
            case INSPECT: append_to_reconstruction(TRACK, "INSPECT: "); break;
            case FIRST_BLOCK: append_to_reconstruction(TRACK, "FB: "); break;
            case SECOND_BLOCK: append_to_reconstruction(TRACK, "SB: "); break;
            case LAST_LAYER_CORNERS: append_to_reconstruction(TRACK, "CMLL: "); break;
            case LAST_SIX_EDGES: append_to_reconstruction(TRACK, "L6E: "); break;
            case SOLVED: append_to_reconstruction(TRACK, "SOLVED"); break;
            default: break;
        }
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