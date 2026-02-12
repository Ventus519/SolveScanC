//
// Created by Ven on 2025/12/22.
//

#include "RouxTracker.h"

#include <stdlib.h>



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



static int is_ROUX_block_complete(const RouxTracker* tracker, const Faces FACE_RL)
{
    int x_buf; //since both FACE_LEFT and FACE_RIGHT lie on the x-axis
    switch (FACE_RL)
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

    const Cubie* LR_FACE_CENTER = get_cubie_from_faces(CUBE, FACES_NULL, FACES_NULL, FACE_RL);
    const Cubie* B_BLOCK_EXTENSION = get_cubie_from_faces(CUBE, FACE_DOWN, FACE_BACK, FACE_RL);
    const Cubie* F_BLOCK_EXTENSION = get_cubie_from_faces(CUBE, FACE_DOWN, FACE_FRONT, FACE_RL);

    if (!LR_FACE_CENTER || !B_BLOCK_EXTENSION || !F_BLOCK_EXTENSION)
    {
        return 0;
    }

    const Colors MAIN_BLOCK_COLOR = LR_FACE_CENTER -> FACE_COLORS[FACE_RL];
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
            if (MAIN_BLOCK_TEST -> FACE_COLORS[FACE_RL] != MAIN_BLOCK_COLOR)
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

static int is_first_block_complete(const RouxTracker* tracker)
{
    return (is_ROUX_block_complete(tracker, FACE_LEFT) || is_ROUX_block_complete(tracker, FACE_RIGHT));
}

static int is_second_block_complete(const RouxTracker* tracker)
{
    return (is_ROUX_block_complete(tracker, FACE_LEFT) && is_ROUX_block_complete(tracker, FACE_RIGHT));
}

static int last_layer_corners_aligned(const RouxTracker* tracker)
{
    const Cubie* R_BLOCK_BASE_EDGE = get_cubie_from_faces(&tracker->CUBE, FACE_DOWN, FACES_NULL, FACE_RIGHT);
    const Cubie* L_BLOCK_BASE_EDGE = get_cubie_from_faces(&tracker->CUBE, FACE_DOWN, FACES_NULL, FACE_LEFT);
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

    const Cubie* UFR_CORNER = get_cubie_from_faces(&tracker->CUBE, FACE_UP, FACE_FRONT, FACE_RIGHT);
    const Cubie* UBR_CORNER = get_cubie_from_faces(&tracker->CUBE, FACE_UP, FACE_BACK, FACE_RIGHT);
    const Cubie* UFL_CORNER = get_cubie_from_faces(&tracker->CUBE, FACE_UP, FACE_FRONT, FACE_LEFT);
    const Cubie* UBL_CORNER = get_cubie_from_faces(&tracker->CUBE, FACE_UP, FACE_BACK, FACE_LEFT);
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
    if (*p_CURRENT_STEP == ROUX_LAST_LAYER_CORNERS && last_layer_corners_aligned(tracker) &&
        is_second_block_complete(tracker))
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





