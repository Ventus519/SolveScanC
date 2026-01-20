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
        case CFOP:
            {
                CFOPTracker* TRACK_CFOP = malloc(sizeof(CFOPTracker));
                if (initialize_CFOPTracker(TRACK_CFOP))
                {
                    goto ENABLED_TRACKER_FAIL;
                }
                tracker -> tracker_CFOP = TRACK_CFOP;
                break;
            }
        case ROUX:
            {
                RouxTracker* TRACK_ROUX = malloc(sizeof(RouxTracker));
                if (initialize_RouxTracker(TRACK_ROUX))
                {
                    goto ENABLED_TRACKER_FAIL;
                }
                tracker -> tracker_ROUX = TRACK_ROUX;
                break;
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
        case CFOP: return (!tracker -> tracker_CFOP);
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
        append_to_reconstruction(tracker, MoveSpec_to_str(&SCRAMBLE_SEQUENCE -> MOVE_SEQUENCE[i]));
    }
    if (moves_successful != SCRAMBLE_SEQUENCE -> MOVE_SEQUENCE_LENGTH)
    {
        return 1;
    }

    if (append_to_reconstruction(tracker, "\nINSPECT: "))
    {
        return 1;
    }

    return update_current_step(tracker, 0, 1);
}

int is_subtracker_cube_solved(const CubeTracker* tracker)
{
    if (is_invalid_CubeTracker(tracker))
    {
        return 0;
    }
    switch (tracker -> ENABLED)
    {
        case BEGINNERS: break;
        case CFOP: return is_solved(&tracker -> tracker_CFOP -> CUBE);
        case ROUX: return is_solved(&tracker -> tracker_ROUX -> CUBE);
        case ZZ: break;
    }
    return 0;
}

int track_MoveStack(CubeTracker* tracker, const MoveStack* MOVES)
{
    if (is_invalid_CubeTracker(tracker) || !MOVES)
    {
        return 1;
    }
    int moves_successful = 0;
    for (int i = 0; i < MOVES -> MOVE_SEQUENCE_LENGTH; i++)
    {
        if (track_move_from_spec(tracker, &MOVES -> MOVE_SEQUENCE[i]))
        {
            (void) backtrack_moves(tracker, moves_successful);
            return 1;
        }
        moves_successful++;
    }
    if (moves_successful != MOVES -> MOVE_SEQUENCE_LENGTH)
    {
        return 1;
    }

    return 0;
}

int apply_move_to_subtracker_cubes(CubeTracker* tracker, const MoveSpec* MOVE_SPEC)
{
    if (is_invalid_CubeTracker(tracker) || !MOVE_SPEC)
    {
        return 1;
    }

    int apply_move_error_code = 0;

    switch (tracker -> ENABLED)
    {
        case BEGINNERS: return 1;
        case CFOP: apply_move_error_code = apply_move_from_spec(&tracker -> tracker_CFOP -> CUBE, MOVE_SPEC); break;
        case ROUX: apply_move_error_code = apply_move_from_spec(&tracker -> tracker_ROUX -> CUBE, MOVE_SPEC); break;
        case ZZ: return 1;
    }
    if (apply_move_error_code != 0)
    {
        return 1;
    }


    return 0;
}

int track_move_from_spec(CubeTracker* tracker, const MoveSpec* MOVE_SPEC)
{
    if (is_invalid_CubeTracker(tracker) || !MOVE_SPEC)
    {
        return 1;
    }

    int continue_inspect = 0;
    switch (tracker -> ENABLED)
    {
        case BEGINNERS: break;
        case CFOP:
        {
            const CFOPMilestones CURRENT_STEP = get_current_CFOP_step(tracker);
            if (CURRENT_STEP == CFOP_SCRAMBLE)
            {
                return (apply_move_to_subtracker_cubes(tracker, MOVE_SPEC) || push_move_to_MoveStack(tracker -> p_MOVES_APPLIED, MOVE_SPEC));
            }
            if (CURRENT_STEP == CFOP_INSPECT && isMoveSpecRotation(MOVE_SPEC))
            {
                continue_inspect = 1;
            }
            break;
        }
        case ROUX:
        {
            const RouxMilestones CURRENT_STEP = get_current_ROUX_step(tracker);
            if (CURRENT_STEP == ROUX_SCRAMBLE)
            {
                return (apply_move_to_subtracker_cubes(tracker, MOVE_SPEC) || push_move_to_MoveStack(tracker -> p_MOVES_APPLIED, MOVE_SPEC));
            }
            if (CURRENT_STEP == ROUX_INSPECT && isMoveSpecRotation(MOVE_SPEC))
            {
                continue_inspect = 1;
            }
            break;
        }
        case ZZ: break;
    }

    if (update_reconstruction(tracker, continue_inspect))
    {
        return 1;
    }
    if (append_to_reconstruction(tracker, MoveSpec_to_str(MOVE_SPEC)))
    {
        return 1;
    }
    if (apply_move_to_subtracker_cubes(tracker, MOVE_SPEC))
    {
        return 1;
    }
    if (push_move_to_MoveStack(tracker -> p_MOVES_APPLIED, MOVE_SPEC))
    {
        return 1;
    }

    if (update_reconstruction(tracker, continue_inspect))
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
        if (apply_move_to_subtracker_cubes(tracker, &MOVE_TO_INVERT))
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
    tracker -> reconstruction_max *= 2;
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
    tracker -> reconstruction_size += strlen(string_entry);
    return 0;
}

int update_reconstruction(CubeTracker* tracker, const int continue_inspect)
{
    if (is_invalid_CubeTracker(tracker))
    {
        return 1;
    }
    const CFOPMilestones CURRENT_CFOP_STEP = get_current_CFOP_step(tracker);
    const RouxMilestones CURRENT_ROUX_STEP = get_current_ROUX_step(tracker);

    if (update_current_step(tracker, 0, continue_inspect))
    {
        return 1;
    }
    switch (tracker -> ENABLED)
    {
        case BEGINNERS: return 1;
        case CFOP:
            {
                const CFOPMilestones NEW_STEP = get_current_CFOP_step(tracker);
                if (CURRENT_CFOP_STEP == NEW_STEP)
                {
                    break;
                }
                switch (NEW_STEP)
                {
                    case CFOP_INSPECT: return append_to_reconstruction(tracker, "\nINSPECT: ");
                    case CFOP_CROSS: return append_to_reconstruction(tracker, "\nCROSS: ");
                    case CFOP_F2L_1: return append_to_reconstruction(tracker, "\n(F2L) Pair 1: ");
                    case CFOP_F2L_2: return append_to_reconstruction(tracker, "\n(F2L) Pair 2: ");
                    case CFOP_F2L_3: return append_to_reconstruction(tracker, "\n(F2L) Pair 3: ");
                    case CFOP_F2L: return append_to_reconstruction(tracker, "\n(F2L) Pair 4: ");
                    case CFOP_OLL: return append_to_reconstruction(tracker, "\nOLL: ");
                    case CFOP_PLL: return append_to_reconstruction(tracker, "\nPLL: ");
                    case CFOP_SOLVED: return append_to_reconstruction(tracker, "\nSOLVED");
                    default: return 1;
                }
            }
        case ROUX:
            {
                const RouxMilestones NEW_STEP = get_current_ROUX_step(tracker);
                if (CURRENT_ROUX_STEP == NEW_STEP)
                {
                    break;
                }
                switch (NEW_STEP)
                {
                    case ROUX_INSPECT: return append_to_reconstruction(tracker, "\nINSPECT: ");
                    case ROUX_FIRST_BLOCK: return append_to_reconstruction(tracker, "\nFB: ");
                    case ROUX_SECOND_BLOCK: return append_to_reconstruction(tracker, "\nSB: ");
                    case ROUX_LAST_LAYER_CORNERS: return append_to_reconstruction(tracker, "\nCMLL: ");
                    case ROUX_LAST_SIX_EDGES: return append_to_reconstruction(tracker, "\nL6E: ");
                    case ROUX_SOLVED: return append_to_reconstruction(tracker, "\nSOLVED");
                    default: return 1;
                }
            }
        case ZZ: return 1;
    }

    return 0;
}

int update_current_step(CubeTracker* tracker, const int continue_scramble, const int continue_inspect)
{
    if (is_invalid_CubeTracker(tracker))
    {
        return 1;
    }
    switch (tracker -> ENABLED)
    {
        case BEGINNERS: return 1;
        case CFOP: return update_current_step_CFOP(tracker -> tracker_CFOP, continue_scramble, continue_inspect);
        case ROUX: return update_current_step_ROUX(tracker -> tracker_ROUX, continue_scramble, continue_inspect);
        case ZZ: return 1;
        default: return 1;
    }
}

CFOPMilestones get_current_CFOP_step(const CubeTracker* tracker)
{
    if (is_invalid_CubeTracker(tracker))
    {
        return CFOP_MILESTONE_NULL;
    }
    if (tracker -> ENABLED != CFOP)
    {
        return CFOP_MILESTONE_NULL;
    }
    return tracker -> tracker_CFOP -> STEP;
}

RouxMilestones get_current_ROUX_step(const CubeTracker* tracker)
{
    if (is_invalid_CubeTracker(tracker))
    {
        return ROUX_MILESTONE_NULL;
    }
    if (tracker -> ENABLED != ROUX)
    {
        return ROUX_MILESTONE_NULL;
    }
    return tracker -> tracker_ROUX -> STEP;
}

