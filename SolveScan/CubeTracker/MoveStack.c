//
// Created by Ven519 on 2026/01/11.
//

#include "MoveStack.h"
#include <string.h>

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

int push_move_to_MoveStack(MoveStack* stack, const MoveSpec* MOVE)
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
    MoveSpec* COPIED_MOVE = malloc(sizeof(MoveSpec));
    stack -> MOVE_SEQUENCE[stack -> MOVE_SEQUENCE_LENGTH] = *COPIED_MOVE;
    stack -> MOVE_SEQUENCE[stack -> MOVE_SEQUENCE_LENGTH].MOVE = MOVE -> MOVE;
    stack -> MOVE_SEQUENCE[stack -> MOVE_SEQUENCE_LENGTH].count = MOVE -> count;
    stack -> MOVE_SEQUENCE[stack -> MOVE_SEQUENCE_LENGTH].clockwise = MOVE -> clockwise;
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
    return (stack -> MOVE_SEQUENCE[stack -> MOVE_SEQUENCE_LENGTH]);
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
