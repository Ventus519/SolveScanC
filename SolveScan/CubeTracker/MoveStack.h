//
// Created by Ven519 on 2026/01/11.
//

#ifndef SOLVESCAN_MOVESTACK_H
#define SOLVESCAN_MOVESTACK_H

#include <stdlib.h>
#include "../Cube3/Cube3.h"

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



#endif //SOLVESCAN_MOVESTACK_H