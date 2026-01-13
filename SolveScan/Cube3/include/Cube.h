#ifndef CUBE_H
#define CUBE_H

#pragma once
#include "Cubie.h"

typedef struct CUBE {
    Cubie* GRAND_CUBE[3][3][3];
}Cube;

typedef enum AXIS{
    AXIS_Z,
    AXIS_Y,
    AXIS_X,
    AXIS_NULL
} Axis;

Cube* create_cube();
int initialize_cube(Cube* cube);
void free_cube(Cube* source);

int set_face_colors(Cube* source, Faces FACE, Colors COLOR);
Colors get_face_center_color(const Cube* source, Faces FACE);
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
int apply_move_from_spec(Cube* source, const MoveSpec* MOVE_SPEC);

int sync_cubie_positions(Cube* source);

int is_solved(const Cube* source);

int parse_move(char** p_MOVE_SPEC, MoveSpec* MOVE_SPEC_DEST);
int apply_move_from_formatted_str(Cube* source, char** p_MOVE_SPEC);

void print_cube(const Cube* source);

#endif //CUBE_H