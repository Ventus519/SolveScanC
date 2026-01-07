//
// Created by Ven on 2025/12/22.
//

#include "RouxTracker.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../Cube3/include/Cube.h"

RouxTracker* create_RouxTracker()
{
    RouxTracker* tracker = malloc(sizeof(RouxTracker));
    if (!tracker)
    {
        goto FAIL;
    }

    Cube* TEST = create_cube();
    if (!TEST)
    {
        goto CUBE_MALLOC_FAIL;
    }

    tracker -> CUBE = TEST;
    tracker -> MOVES_APPLIED_MAX = 3;

    char* MOVES_APPLIED_DEST = malloc(sizeof(char) * tracker -> MOVES_APPLIED_MAX);
    if (!MOVES_APPLIED_DEST)
    {
        goto MOVES_APPLIED_MALLOC_FAIL;
    }
    MOVES_APPLIED_DEST[0] = '\0';

    tracker -> MOVES_APPLIED = MOVES_APPLIED_DEST;

    tracker -> MOVES_APPLIED_SIZE = 1;

    tracker -> STEP = SCRAMBLE;

    return tracker;

    MOVES_APPLIED_MALLOC_FAIL:
        free(tracker -> CUBE);
    CUBE_MALLOC_FAIL:
        free(tracker);
    FAIL:
        return NULL;
}

void free_RouxTracker(RouxTracker* tracker)
{
    if (!tracker)
    {
        return;
    }
    free(tracker -> MOVES_APPLIED);
    free(tracker -> CUBE);
    free(tracker);
}

Cube* get_cube(const RouxTracker* tracker)
{
    if (!tracker)
    {
        return NULL;
    }
    return tracker -> CUBE;
}

int is_block_complete(const RouxTracker* tracker, const Faces FACE_LEFT_OR_FACE_RIGHT)
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

    const Cube* CUBE = get_cube(tracker);
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
    return (is_block_complete(tracker, FACE_LEFT) || is_block_complete(tracker, FACE_RIGHT));
}

int is_second_block_complete(const RouxTracker* tracker)
{
    return (is_block_complete(tracker, FACE_LEFT) && is_block_complete(tracker, FACE_RIGHT));
}

int last_layer_corners_aligned(const RouxTracker* tracker)
{
    if (!tracker || !(tracker -> CUBE))
    {
        return 0;
    }
    const Cubie* R_BLOCK_BASE_EDGE = get_cubie_at_position(tracker->CUBE, 1, -1, 0);
    const Cubie* L_BLOCK_BASE_EDGE = get_cubie_at_position(tracker->CUBE, -1, -1, 0);
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

    Cubie* UFR_CORNER = get_cubie_at_position(tracker->CUBE, 1, 1, 1);
    Cubie* UBR_CORNER = get_cubie_at_position(tracker->CUBE, 1, 1, -1);
    Cubie* UFL_CORNER = get_cubie_at_position(tracker->CUBE, -1, 1, 1);
    Cubie* UBL_CORNER = get_cubie_at_position(tracker->CUBE, -1, 1, -1);
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
    if (UFL_CORNER -> FACE_COLORS[FACE_UP] != UR_BASE_COLOR && UFR_CORNER -> FACE_COLORS[FACE_UP] != UL_BASE_COLOR)
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

int update_current_step(RouxTracker* tracker, const int continue_scramble, const int continue_inspect)
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
        *p_CURRENT_STEP = SCRAMBLE;
        return 0;
    }
    if (continue_inspect)
    {
        *p_CURRENT_STEP = INSPECT;
        return 0;
    }
    if (*p_CURRENT_STEP <= FIRST_BLOCK)
    {
        *p_CURRENT_STEP = FIRST_BLOCK;
    }
    if (*p_CURRENT_STEP == FIRST_BLOCK && is_first_block_complete(tracker))
    {
        *p_CURRENT_STEP = SECOND_BLOCK;
    }
    if (*p_CURRENT_STEP == SECOND_BLOCK && is_second_block_complete(tracker))
    {
        *p_CURRENT_STEP = LAST_LAYER_CORNERS;
    }
    if (*p_CURRENT_STEP == LAST_LAYER_CORNERS && last_layer_corners_aligned(tracker))
    {
        *p_CURRENT_STEP = LAST_SIX_EDGES;
    }
    if (is_solved(tracker -> CUBE))
    {
        *p_CURRENT_STEP = SOLVED;
    }
    if (*p_CURRENT_STEP == MILESTONE_NULL)
    {
        return 1;
    }
    return 0;
}

int apply_moves(RouxTracker* tracker, char** SEQUENCE)
{
    if (!tracker || !SEQUENCE || !(*SEQUENCE) || !(tracker -> MOVES_APPLIED))
    {
        return 1;
    }
    if (normalize_moves_applied(tracker))
    {
        return 1;
    }


    const size_t SEQUENCE_SIZE = strlen(*SEQUENCE);
    //MOVES_APPLIED_SIZE is the number of bytes used, MOVES_APPLIED_MAX is the max amount of bytes
    while (tracker -> MOVES_APPLIED_SIZE + SEQUENCE_SIZE > tracker -> MOVES_APPLIED_MAX)
    {
        if (resize_moves_applied(tracker))
        {
            return 1;
        }
    }

    if (apply_move_from_formatted_str(tracker -> CUBE, SEQUENCE))
    {
        return 1;
    }

    memcpy(tracker -> MOVES_APPLIED + tracker -> MOVES_APPLIED_SIZE - 1, *SEQUENCE, SEQUENCE_SIZE + 1);
    tracker -> MOVES_APPLIED_SIZE += (int)SEQUENCE_SIZE;

    return 0;
}

int track_applied_move(RouxTracker* tracker, char** FORMATTED_MOVE)
{
    if (!tracker || !FORMATTED_MOVE || !(*FORMATTED_MOVE) || !(tracker -> MOVES_APPLIED))
    {
        return 1;
    }

    if (normalize_moves_applied(tracker))
    {
        return 1;
    }

    if (apply_move_from_formatted_str(tracker -> CUBE, FORMATTED_MOVE))
    {
        return 1;
    }

    const size_t FORMATTED_MOVE_SIZE = strlen(*FORMATTED_MOVE);
    while (tracker -> MOVES_APPLIED_SIZE + FORMATTED_MOVE_SIZE > tracker -> MOVES_APPLIED_MAX)
    {
        if (resize_moves_applied(tracker))
        {
            return 1;
        }
    }

    memcpy(tracker -> MOVES_APPLIED + tracker -> MOVES_APPLIED_SIZE - 1, *FORMATTED_MOVE, FORMATTED_MOVE_SIZE + 1);
    tracker -> MOVES_APPLIED_SIZE += (int)FORMATTED_MOVE_SIZE;

    //step has to be updated manually after calling since there is no guarantee that the move is not a part of scramble/inspection

    return 0;
}

int resize_moves_applied(RouxTracker* tracker)
{
    if (!tracker || !tracker -> MOVES_APPLIED)
    {
        return 1;
    }

    const size_t new_size = tracker -> MOVES_APPLIED_MAX * 2;
    char* p = malloc(sizeof(char)* new_size);
    if (!p)
    {
        return 1;
    }
    memcpy(p, tracker -> MOVES_APPLIED, tracker -> MOVES_APPLIED_SIZE);
    tracker -> MOVES_APPLIED = p;
    tracker -> MOVES_APPLIED_MAX = new_size;
    return 0;
}

int normalize_moves_applied(RouxTracker* tracker)
{
    if (!tracker || !tracker -> MOVES_APPLIED)
    {
        return 1;
    }
    if (tracker -> MOVES_APPLIED_SIZE <= 1)
    {
        return 0;
    }
    if (tracker -> MOVES_APPLIED[tracker -> MOVES_APPLIED_SIZE - 2] == ' ')
    {
        return 0;
    }
    if (tracker -> MOVES_APPLIED_SIZE + 1 > tracker -> MOVES_APPLIED_MAX)
    {
        if (resize_moves_applied(tracker))
        {
            return 1;
        }
    }
    tracker -> MOVES_APPLIED[tracker -> MOVES_APPLIED_SIZE - 1] = ' ';
    tracker -> MOVES_APPLIED[tracker -> MOVES_APPLIED_SIZE] = '\0';
    tracker -> MOVES_APPLIED_SIZE++;

    return 0;

}

void print_tracker_state(const RouxTracker* tracker)
{
    if (!tracker)
    {
        return;
    }
    if (!tracker -> CUBE || !tracker -> MOVES_APPLIED)
    {
        return;
    }
    printf("CURRENT STEP: ");
    switch (tracker -> STEP)
    {
        case SCRAMBLE: printf("SCRAMBLE\n"); break;
        case INSPECT: printf("INSPECT\n"); break;
        case FIRST_BLOCK: printf("FB\n"); break;
        case SECOND_BLOCK: printf("SB\n"); break;
        case LAST_LAYER_CORNERS: printf("CMLL\n"); break;
        case LAST_SIX_EDGES: printf("L6E\n"); break;
        case SOLVED: printf("SOLVED\n"); break;
        case MILESTONE_NULL: printf("NULL\n"); break;
    }
    printf("MOVES APPLIED: %s\n", tracker -> MOVES_APPLIED);
}