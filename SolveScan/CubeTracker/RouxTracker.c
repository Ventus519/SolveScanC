//
// Created by Ven on 2025/12/22.
//

#include "RouxTracker.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../Cube3/include/Cube.h"


int initialize_RouxTracker(RouxTracker* tracker)
{
    if (!tracker)
    {
        return 1;
    }
    tracker -> STEP = ROUX_SCRAMBLE;
    if (initialize_cube(&(tracker -> CUBE)))
    {
        return 1;
    }

    return 0;
}


void free_RouxTracker(RouxTracker* tracker)
{
    if (!tracker)
    {
        return;
    }
    free(tracker);
}



int is_ROUX_block_complete(const RouxTracker* tracker, const Faces FACE_LEFT_OR_FACE_RIGHT)
{
    if (!tracker)
    {
        return 0;
    }
    int x_buf; //since both FACE_LEFT and FACE_RIGHT lie on the x-axis
    switch (FACE_LEFT_OR_FACE_RIGHT)
    {
        case FACE_LEFT: x_buf = -1; break;
        case FACE_RIGHT: x_buf = 1; break;
        default: return 0;
    }

    const Cube* CUBE = &(tracker -> CUBE);
    if (!CUBE)
    {
        return 0;
    }

    const Cubie* LR_FACE_CENTER = get_cubie_at_position(CUBE, x_buf, 0, 0);
    const Cubie* B_BLOCK_EXTENSION = get_cubie_at_position(CUBE, x_buf, -1, -1);
    const Cubie* F_BLOCK_EXTENSION = get_cubie_at_position(CUBE, x_buf, -1, 1);

    if (!LR_FACE_CENTER || !B_BLOCK_EXTENSION || !F_BLOCK_EXTENSION)
    {
        return 0;
    }

    const Colors MAIN_BLOCK_COLOR = LR_FACE_CENTER -> FACE_COLORS[FACE_LEFT_OR_FACE_RIGHT];
    const Colors BACK_EXTENSION_COLOR = B_BLOCK_EXTENSION -> FACE_COLORS[FACE_BACK];
    const Colors FRONT_EXTENSION_COLOR = F_BLOCK_EXTENSION -> FACE_COLORS[FACE_FRONT];
    const Colors BLOCK_BASE_COLOR = B_BLOCK_EXTENSION -> FACE_COLORS[FACE_DOWN];

    for (int y = -1; y < 1; y++)
    {
        const Cubie* FRONT_EXTENSION_TEST = get_cubie_at_position(CUBE, x_buf, y, 1);
        const Cubie* BACK_EXTENSION_TEST = get_cubie_at_position(CUBE, x_buf, y, -1);
        if (!FRONT_EXTENSION_TEST || !BACK_EXTENSION_TEST)
        {
            return 0;
        }

        if (FRONT_EXTENSION_TEST -> FACE_COLORS[FACE_FRONT] != FRONT_EXTENSION_COLOR)
        {
            return 0;
        }
        if (BACK_EXTENSION_TEST -> FACE_COLORS[FACE_BACK] != BACK_EXTENSION_COLOR)
        {
            return 0;
        }
        for (int z = -1; z < 2; z++)
        {
            const Cubie* BLOCK_BASE_TEST = get_cubie_at_position(CUBE, x_buf, -1, z);
            const Cubie* MAIN_BLOCK_TEST = get_cubie_at_position(CUBE, x_buf, y, z);
            if (!BLOCK_BASE_TEST || !MAIN_BLOCK_TEST)
            {
                return 0;
            }
            if (MAIN_BLOCK_TEST -> FACE_COLORS[FACE_LEFT_OR_FACE_RIGHT] != MAIN_BLOCK_COLOR)
            {
                return 0;
            }
            if (BLOCK_BASE_TEST -> FACE_COLORS[FACE_DOWN] != BLOCK_BASE_COLOR)
            {
                return 0;
            }
        }
    }
    return 1;
}

int is_first_block_complete(const RouxTracker* tracker)
{
    return (is_ROUX_block_complete(tracker, FACE_LEFT) || is_ROUX_block_complete(tracker, FACE_RIGHT));
}

int is_second_block_complete(const RouxTracker* tracker)
{
    return (is_ROUX_block_complete(tracker, FACE_LEFT) && is_ROUX_block_complete(tracker, FACE_RIGHT));
}

int last_layer_corners_aligned(const RouxTracker* tracker)
{
    if (!tracker)
    {
        return 0;
    }
    const Cubie* R_BLOCK_BASE_EDGE = get_cubie_at_position(&tracker->CUBE, 1, -1, 0);
    const Cubie* L_BLOCK_BASE_EDGE = get_cubie_at_position(&tracker->CUBE, -1, -1, 0);
    if (!R_BLOCK_BASE_EDGE || !L_BLOCK_BASE_EDGE)
    {
        return 0;
    }
    const Colors UR_BASE_COLOR = getOppositeColor(R_BLOCK_BASE_EDGE -> FACE_COLORS[FACE_DOWN]);
    const Colors UL_BASE_COLOR = getOppositeColor(L_BLOCK_BASE_EDGE -> FACE_COLORS[FACE_DOWN]);
    if (UR_BASE_COLOR == COLORS_NULL || UL_BASE_COLOR == COLORS_NULL)
    {
        return 0;
    }

    Cubie* UFR_CORNER = get_cubie_at_position(&tracker->CUBE, 1, 1, 1);
    Cubie* UBR_CORNER = get_cubie_at_position(&tracker->CUBE, 1, 1, -1);
    Cubie* UFL_CORNER = get_cubie_at_position(&tracker->CUBE, -1, 1, 1);
    Cubie* UBL_CORNER = get_cubie_at_position(&tracker->CUBE, -1, 1, -1);
    if (!UFR_CORNER || !UBR_CORNER || !UFL_CORNER || !UBL_CORNER)
    {
        return 0;
    }

    if (UFR_CORNER -> FACE_COLORS[FACE_UP] != UR_BASE_COLOR && UFR_CORNER -> FACE_COLORS[FACE_UP] != UL_BASE_COLOR)
    {
        return 0;
    }
    if (UBR_CORNER -> FACE_COLORS[FACE_UP] != UR_BASE_COLOR && UBR_CORNER -> FACE_COLORS[FACE_UP] != UL_BASE_COLOR)
    {
        return 0;
    }
    if (UFL_CORNER -> FACE_COLORS[FACE_UP] != UR_BASE_COLOR && UFL_CORNER -> FACE_COLORS[FACE_UP] != UL_BASE_COLOR)
    {
        return 0;
    }
    if (UBL_CORNER -> FACE_COLORS[FACE_UP] != UR_BASE_COLOR && UBL_CORNER -> FACE_COLORS[FACE_UP] != UL_BASE_COLOR)
    {
        return 0;
    }

    const int R_COLOR_DIFFERENCE = abs((int)(UBR_CORNER -> FACE_COLORS[FACE_RIGHT] - UFR_CORNER -> FACE_COLORS[FACE_RIGHT]));
    const int F_COLOR_DIFFERENCE = abs((int)(UFR_CORNER -> FACE_COLORS[FACE_FRONT] - UFL_CORNER -> FACE_COLORS[FACE_FRONT]));
    const int L_COLOR_DIFFERENCE = abs((int)(UFL_CORNER -> FACE_COLORS[FACE_LEFT] - UBL_CORNER -> FACE_COLORS[FACE_LEFT]));
    const int B_COLOR_DIFFERENCE = abs((int)(UBL_CORNER -> FACE_COLORS[FACE_BACK] - UBR_CORNER -> FACE_COLORS[FACE_BACK]));

    if (R_COLOR_DIFFERENCE != 0 && F_COLOR_DIFFERENCE != 0)
    {
        return 0;
    }

    return ((R_COLOR_DIFFERENCE == L_COLOR_DIFFERENCE) && (F_COLOR_DIFFERENCE == B_COLOR_DIFFERENCE));
}

int update_current_step_ROUX(RouxTracker* tracker, const int continue_scramble, const int continue_inspect)
{
    if (!tracker)
    {
        return 1;
    }
    RouxMilestones* p_CURRENT_STEP = &(tracker -> STEP);
    if (!p_CURRENT_STEP)
    {
        return 1;
    }
    if (continue_scramble)
    {
        *p_CURRENT_STEP = ROUX_SCRAMBLE;
        return 0;
    }
    if (continue_inspect)
    {
        *p_CURRENT_STEP = ROUX_INSPECT;
        return 0;
    }
    if (*p_CURRENT_STEP <= ROUX_FIRST_BLOCK)
    {
        *p_CURRENT_STEP = ROUX_FIRST_BLOCK;
    }
    if (*p_CURRENT_STEP == ROUX_FIRST_BLOCK && is_first_block_complete(tracker))
    {
        *p_CURRENT_STEP = ROUX_SECOND_BLOCK;
    }
    if (*p_CURRENT_STEP == ROUX_SECOND_BLOCK && is_second_block_complete(tracker))
    {
        *p_CURRENT_STEP = ROUX_LAST_LAYER_CORNERS;
    }
    if (*p_CURRENT_STEP == ROUX_LAST_LAYER_CORNERS && last_layer_corners_aligned(tracker) && is_second_block_complete(tracker))
    {
        *p_CURRENT_STEP = ROUX_LAST_SIX_EDGES;
    }
    if (is_solved(&tracker -> CUBE))
    {
        *p_CURRENT_STEP = ROUX_SOLVED;
    }
    if (*p_CURRENT_STEP == ROUX_MILESTONE_NULL)
    {
        return 1;
    }
    return 0;
}



int track_applied_move_spec_roux(RouxTracker* tracker, MoveSpec* MOVE_TO_APPLY)
{
    if (!tracker || !MOVE_TO_APPLY)
    {
        return 1;
    }
    if (apply_move_from_spec(&tracker -> CUBE, MOVE_TO_APPLY))
    {
        return 1;
    }

    return 0;
}


void print_tracker_state(const RouxTracker* tracker)
{
    if (!tracker)
    {
        return;
    }

    printf("CURRENT STEP: ");
    switch (tracker -> STEP)
    {
        case ROUX_SCRAMBLE: printf("SCRAMBLE\n"); break;
        case ROUX_INSPECT: printf("INSPECT\n"); break;
        case ROUX_FIRST_BLOCK: printf("FB\n"); break;
        case ROUX_SECOND_BLOCK: printf("SB\n"); break;
        case ROUX_LAST_LAYER_CORNERS: printf("CMLL\n"); break;
        case ROUX_LAST_SIX_EDGES: printf("L6E\n"); break;
        case ROUX_SOLVED: printf("SOLVED\n"); break;
        case ROUX_MILESTONE_NULL: printf("NULL\n"); break;
    }
}
