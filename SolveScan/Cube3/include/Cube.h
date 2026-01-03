#ifndef CUBE_H
#define CUBE_H

#pragma once
#include "Cubie.h"

typedef struct CUBE {
    Cubie* GRAND_CUBE[3][3][3];
}Cube;

typedef enum {
    AXIS_Z,
    AXIS_Y,
    AXIS_X,
    AXIS_NULL
} Axis;

Cube* create_cube();
void free_cube(Cube* source);

int set_face_colors(Cube* source, Faces FACE, Colors COLOR);
Cubie* get_cubie_at_position(const Cube* source, int x, int y, int z);

int move_f(Cube* source, int count, int clockwise);
int move_u(Cube* source, int count, int clockwise);
int move_r(Cube* source, int count, int clockwise);
int move_b(Cube* source, int count, int clockwise);
int move_d(Cube* source, int count, int clockwise);
int move_l(Cube* source, int count, int clockwise);

int slice_s(Cube* source, int count, int clockwise);
int slice_e(Cube* source, int count, int clockwise);
int slice_m(Cube* source, int count, int clockwise);

int rotate_about_axis(Cube* source, Axis AXIS_OF_ROTATION, int count, int clockwise);
int apply_move(Cube* source, Moves MOVE, int count, int clockwise);

int sync_cubie_positions(Cube* source);

int is_solved(const Cube* source);

int parse_move(char** p_MOVE_SPEC, Moves* p_MOVE, int* p_count, int* p_clockwise);
int apply_move_sequence(Cube* source, char** p_MOVE_SEQ);

void print_cube(const Cube* source);

#endif //CUBE_H