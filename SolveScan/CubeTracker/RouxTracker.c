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

    tracker -> STEP = SCRAMBLE;

    MoveStack* stack = create_MoveStack();
    if (!stack)
    {
        goto STACK_FAIL;;
    }
    tracker -> APPLIED = stack;

    return tracker;
    STACK_FAIL:
        free(tracker -> CUBE);
    CUBE_MALLOC_FAIL:
        free(tracker);
    FAIL:
        return NULL;
}

RouxTracker* create_RouxTracker_from_MoveStack(MoveStack* MOVE_SEQUENCE_DEST)
{
    if (!MOVE_SEQUENCE_DEST)
    {
        return create_RouxTracker();
    }

    RouxTracker* tracker = malloc(sizeof(RouxTracker));
    if (!tracker)
    {
        return NULL;
    }
    tracker -> APPLIED = MOVE_SEQUENCE_DEST;

    Cube* TEST = create_cube();
    if (!TEST)
    {
        free(tracker);
        return NULL;
    }
    tracker -> CUBE = TEST;
    tracker -> STEP = SCRAMBLE;

    return tracker;
}

void free_RouxTracker(RouxTracker* tracker)
{
    if (!tracker)
    {
        return;
    }
    free(tracker -> CUBE);
    free_MoveStack(tracker -> APPLIED);
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

MoveStack* create_MoveStack()
{
    MoveStack* stack = malloc(sizeof(MoveStack));
    if (!stack)
    {
        return NULL;
    }
    stack -> MOVE_SEQUENCE_MAX = 4; //assume the stack will store a common trigger move
    stack -> MOVE_SEQUENCE_LENGTH = 0; //no moves have been added to the stack
    MoveSpec* sequence = malloc(sizeof(MoveSpec) * (stack -> MOVE_SEQUENCE_MAX));
    if (!sequence)
    {
        free(stack);
        return NULL;
    }
    stack -> MOVE_SEQUENCE = sequence;
    const MoveSpec DEFAULT_MOVES = {MOVE_NULL, 0, 1};
    for (int i = 0; i < stack -> MOVE_SEQUENCE_MAX; i++)
    {
        sequence[i] = DEFAULT_MOVES;
    }
    return stack;
}

void free_MoveStack(MoveStack* stack)
{
    if (!stack)
    {
        return;
    }
    free(stack -> MOVE_SEQUENCE);
    free(stack);
}

int push_move_to_MoveStack(MoveStack* stack, MoveSpec* MOVE)
{
    if (!stack || !MOVE || !(stack -> MOVE_SEQUENCE))
    {
        return 1;
    }
    if (stack -> MOVE_SEQUENCE_LENGTH + 1 >= stack -> MOVE_SEQUENCE_MAX)
    {
        if (resize_MoveStack(stack))
        {
            return 1;
        }
    }
    stack -> MOVE_SEQUENCE[stack -> MOVE_SEQUENCE_LENGTH] = *MOVE;
    stack -> MOVE_SEQUENCE_LENGTH++;
    return 0;
}

int resize_MoveStack(MoveStack* stack)
{
    if (!stack || !(stack -> MOVE_SEQUENCE))
    {
        return 1;
    }
    MoveSpec* new_stack = malloc(sizeof(MoveSpec) * (stack -> MOVE_SEQUENCE_MAX * 2));
    if (!new_stack)
    {
        return 1;
    }
    stack -> MOVE_SEQUENCE_MAX *= 2;
    for (size_t i = 0;  i < stack -> MOVE_SEQUENCE_LENGTH; i++)
    {
        new_stack[i] = stack -> MOVE_SEQUENCE[i];
    }
    free(stack -> MOVE_SEQUENCE);
    stack -> MOVE_SEQUENCE = new_stack;
    return 0;
}

MoveSpec pop_move_from_MoveStack(MoveStack* stack)
{
    const MoveSpec DEFAULT_MOVES = {MOVE_NULL, 0, 1};
    if (!stack || !(stack -> MOVE_SEQUENCE))
    {
        return DEFAULT_MOVES;
    }
    if (stack -> MOVE_SEQUENCE_LENGTH == 0)
    {
        return DEFAULT_MOVES;
    }
    stack -> MOVE_SEQUENCE_LENGTH--;
    const MoveSpec popped = (stack -> MOVE_SEQUENCE[stack -> MOVE_SEQUENCE_LENGTH]);
    return popped;
}

char* MoveStack_to_str(MoveStack* stack)
{
    if (!stack || !(stack -> MOVE_SEQUENCE))
    {
        return NULL;
    }
    char* result_str = malloc(sizeof(char) * (stack -> MOVE_SEQUENCE_LENGTH * 3 + 1));
    result_str[0] = '\0';
    for (size_t i = 0;  i < stack -> MOVE_SEQUENCE_LENGTH; i++)
    {
        MoveSpec* MOVE_SPEC = &(stack -> MOVE_SEQUENCE[i]);
        const int move_full_cycle = MOVE_SPEC -> count >= 4;
        if (MOVE_SPEC -> count == 0)
        {
            continue;
        }
        MOVE_SPEC -> count %= 4;
        switch (MOVE_SPEC -> MOVE)
        {
            case MOVE_FRONT: strcat(result_str, "F"); break;
            case MOVE_UP: strcat(result_str, "U"); break;
            case MOVE_RIGHT: strcat(result_str,"R"); break;
            case MOVE_BACK: strcat(result_str,"B"); break;
            case MOVE_DOWN: strcat(result_str,"D"); break;
            case MOVE_LEFT: strcat(result_str,"L"); break;

            case WIDE_FRONT: strcat(result_str, "f"); break;
            case WIDE_UP: strcat(result_str, "u"); break;
            case WIDE_RIGHT: strcat(result_str, "r"); break;
            case WIDE_BACK: strcat(result_str, "b"); break;
            case WIDE_DOWN: strcat(result_str, "d"); break;
            case WIDE_LEFT: strcat(result_str, "l"); break;

            case SLICE_SIDE: strcat(result_str, "S"); break;
            case SLICE_EQUATOR: strcat(result_str, "E"); break;
            case SLICE_MIDDLE: strcat(result_str, "M"); break;

            case ROT_Z: strcat(result_str, "z"); break;
            case ROT_Y: strcat(result_str, "y"); break;
            case ROT_X: strcat(result_str, "x"); break;

            case MOVE_NULL: continue;
            default: free(result_str); return NULL;
        }
        if (move_full_cycle)
        {
            switch (MOVE_SPEC -> count)
            {
                case 0: strcat(result_str, "4"); break;
                case 1: strcat(result_str, "5"); break;
                case 2: strcat(result_str, "6"); break;
                case 3: strcat(result_str, "7"); break;
                default: free(result_str); return NULL;
            }
        }
        else
        {
            switch (MOVE_SPEC -> count)
            {
                case 1: break;
                case 2: strcat(result_str, "2"); break;
                case 3: strcat(result_str, "3"); break;
                default: free(result_str); return NULL;
            }
        }

        if (!(MOVE_SPEC -> clockwise))
        {
            strcat(result_str, "\'");
        }
        strcat(result_str, " ");
    }


    return result_str;
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

int track_scramble(RouxTracker* tracker, MoveStack* SCRAMBLE_SEQUENCE)
{
    if (!tracker || !tracker -> APPLIED || !tracker -> CUBE || !SCRAMBLE_SEQUENCE)
    {
        return 1;
    }
    for (size_t i = 0; i < SCRAMBLE_SEQUENCE -> MOVE_SEQUENCE_LENGTH; i++)
    {
        MoveSpec* p_MOVE = &(SCRAMBLE_SEQUENCE -> MOVE_SEQUENCE[i]);
        if (push_move_to_MoveStack(tracker -> APPLIED, p_MOVE))
        {
            return 1;
        }
        if (apply_move_from_spec(tracker -> CUBE, p_MOVE))
        {
            return 1;
        }
    }
    if (update_current_step(tracker, 0, 1))
    {
        return 1;
    }

    return 0;
}


int track_applied_move_formatted_str(RouxTracker* tracker, char** FORMATTED_MOVE)
{
    if (!tracker || !FORMATTED_MOVE || !(*FORMATTED_MOVE) || !(tracker -> APPLIED))
    {
        return 1;
    }

    MoveSpec MOVE;
    if (parse_move(FORMATTED_MOVE, &MOVE))
    {
        return 1;
    }
    if (push_move_to_MoveStack(tracker -> APPLIED, &MOVE))
    {
        return 1;
    }
    if (apply_move_from_spec(tracker -> CUBE, &MOVE))
    {
        return 1;
    }

    //step has to be manually updated afterward

    return 0;
}


void print_tracker_state(const RouxTracker* tracker)
{
    if (!tracker)
    {
        return;
    }
    if (!tracker -> CUBE || !tracker -> APPLIED)
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
    char* reconstruct = MoveStack_to_str(tracker -> APPLIED);
    printf("MOVES APPLIED: %s\n", reconstruct);
    free(reconstruct);
}