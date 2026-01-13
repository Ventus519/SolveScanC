//
// Created by Ven519 on 2026/01/11.
//

#include "CubeTracker.h"
#include <string.h>
#include <stdio.h>

int initialize_CubeTracker(CubeTracker* tracker, const char* file_path, const EnabledTrackers TRACKER_TO_ENABLE)
{
    if (!tracker || !file_path)
    {
        return 1;
    }
    tracker -> save_file_path = file_path;

    tracker -> ENABLED = TRACKER_TO_ENABLE;

    MoveStack* STACK = create_MoveStack();
    if (!STACK)
    {
        return 1;
    }
    tracker -> p_MOVES_APPLIED = STACK;

    tracker -> reconstruction_size = 11;
    tracker -> reconstruction_max = 22;
    char* RECONSTRUCT_TEST = malloc(sizeof(char) * tracker -> reconstruction_max);
    if (!RECONSTRUCT_TEST)
    {
        goto RECONSTRUCTION_FAIL;
    }
    RECONSTRUCT_TEST[0] = '\0';
    strcat (RECONSTRUCT_TEST, "SCRAMBLE: ");
    tracker -> reconstruction = RECONSTRUCT_TEST;

    switch (TRACKER_TO_ENABLE)
    {
        case BEGINNERS: break;
        case CFOP: break;
        case ROUX:
            {
                RouxTracker* TRACK_ROUX = malloc(sizeof(RouxTracker));
                if (initialize_RouxTracker(TRACK_ROUX))
                {
                    goto ENABLED_TRACKER_FAIL;
                }
                tracker -> tracker_ROUX = TRACK_ROUX;
            }
        case ZZ: break;
    }



    return 0;

    ENABLED_TRACKER_FAIL:
        free(RECONSTRUCT_TEST);
    RECONSTRUCTION_FAIL:
        free(STACK);
        return 1;
}

int is_invalid_CubeTracker(const CubeTracker* tracker)
{
    if (!tracker)
    {
        return 1;
    }
    if (!tracker -> reconstruction)
    {
        return 1;
    }
    switch (tracker -> ENABLED)
    {
        case BEGINNERS: return 0;
        case CFOP: return 0;
        case ROUX: return (!tracker -> tracker_ROUX);
        default: return 0;
    }
}

int track_scramble(CubeTracker* tracker, const MoveStack* SCRAMBLE_SEQUENCE)
{
    if (is_invalid_CubeTracker(tracker) || !SCRAMBLE_SEQUENCE)
    {
        return 1;
    }

    int moves_successful = 0; //this will be used once backtracking has been implemented
    for (int i = 0; i < SCRAMBLE_SEQUENCE -> MOVE_SEQUENCE_LENGTH; i++)
    {
        if (track_move_from_spec(tracker, &SCRAMBLE_SEQUENCE -> MOVE_SEQUENCE[i]))
        {
            (void) backtrack_moves(tracker, moves_successful);
            return 1;
        }
        moves_successful++;
    }

    return 0;
}

int track_move_from_spec(CubeTracker* tracker, const MoveSpec* MOVE_SPEC)
{
    if (is_invalid_CubeTracker(tracker) || !MOVE_SPEC)
    {
        return 1;
    }

    if (apply_move_from_spec(&tracker -> tracker_ROUX -> CUBE, MOVE_SPEC))
    {
        return 1;
    }
    if (push_move_to_MoveStack(tracker -> p_MOVES_APPLIED, MOVE_SPEC))
    {
        return 1;
    }

    return 0;
}

int backtrack_moves(CubeTracker* tracker, const int count)
{
    if (is_invalid_CubeTracker(tracker))
    {
        return 1;
    }
    for (int i = 0; i < count; i++)
    {
        MoveSpec MOVE_TO_INVERT = pop_move_from_MoveStack(tracker -> p_MOVES_APPLIED);
        MOVE_TO_INVERT.clockwise = !MOVE_TO_INVERT.clockwise;
        if (apply_move_from_spec(&tracker -> tracker_ROUX -> CUBE, &MOVE_TO_INVERT))
        {
            return 1;
        }
    }
    return 0;
}

int resize_reconstruction(CubeTracker* tracker)
{
    if (is_invalid_CubeTracker(tracker))
    {
        return 1;
    }
    char* RECONSTRUCT_TEST = malloc(sizeof(char) * tracker -> reconstruction_max * 2);
    if (!RECONSTRUCT_TEST)
    {
        return 1;
    }
    for (int i = 0; i < tracker -> reconstruction_max; i++)
    {
        RECONSTRUCT_TEST[i] = tracker -> reconstruction[i];
    }
    free(tracker -> reconstruction);
    tracker -> reconstruction = RECONSTRUCT_TEST;
    return 0;
}

int append_to_reconstruction(CubeTracker* tracker, const char* string_entry)
{
    if (is_invalid_CubeTracker(tracker) || !string_entry)
    {
        return 1;
    }
    while (tracker -> reconstruction_size + strlen(string_entry) >= tracker -> reconstruction_max)
    {
        if (resize_reconstruction(tracker))
        {
            return 1;
        }
    }
    strcat(tracker -> reconstruction, string_entry);
    return 0;
}
