//
// Created by Ven519 on 2026/01/11.
//

#ifndef SOLVESCAN_MOVESTACK_H
#define SOLVESCAN_MOVESTACK_H

#include <stdlib.h>
#include "../Cube3/Cube3.h"


/**
 * @struct MOVE_STACK
 *
 * Represents a sequence of moves that can be applied to a 3x3x3 Rubik's Cube.
 *
 * The MOVE_SEQUENCE member is a heap-allocated, dynamic array of MoveSpec structures. Since this is heap-allocated, it
 * is recommended to use create_MoveStack(); when using a MoveStack to represent a move sequence. It is not recommended
 * to create a MoveStack on stack memory.
 *
 * If there isn't enough space allocated to push a new move onto the stack, the stack reallocates double its current
 * maximum length.
 *
 * If there is a preexisting move string, move_str_to_MoveStack(char* move_str) can be used to create a MoveStack with
 * those moves already loaded. The provided move string must be completely expanded using standard WCA move notation.
 * For more information, see move_str_to_MoveStack(char* move_str).
 **/
typedef struct MOVE_STACK {
    MoveSpec* MOVE_SEQUENCE;
    size_t MOVE_SEQUENCE_LENGTH;
    size_t MOVE_SEQUENCE_MAX;
} MoveStack;

MoveStack* create_MoveStack();
void free_MoveStack(MoveStack* stack);
int push_move_to_MoveStack(MoveStack* stack, const MoveSpec* MOVE);
int resize_MoveStack(MoveStack* stack);
MoveSpec pop_move_from_MoveStack(MoveStack* stack);
char* MoveStack_to_str(MoveStack* stack);
MoveStack* move_str_to_MoveStack(char* move_str);



#endif //SOLVESCAN_MOVESTACK_H