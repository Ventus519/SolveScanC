#ifndef CUBE_H
#define CUBE_H

#pragma once
#include "Cubie.h"

/** @struct CUBE
 *      Represents a 3x3x3 Rubik's Cube and operates in the same way. (Does not easily expand to larger dimensions)
 *      Each piece of the Cube is considered a cubie (including the core).
 *      Retrieving Cubies of the Cube should be done with get_cubie_at_position() and not with manual indexing
 *
 *      The coordinate system follows the axes of rotation from standard WCA moves:
 *          -Core: The colorless origin of the Cube (0, 0, 0)
 *          -+x : direction from the core towards the R face
 *          -+y : direction from the core towards the U face
 *          -+z : direction from the core towards the F face
 *
 *      A clockwise rotation applied on a move maintains its same definition as by the WCA standard.
 *      Thus, moving a face clockwise is equivalent to rotating it in the clockwise direction when looking at said face.
 *
 *      The center-piece of a face determines its color. To get the color of a face, use get_face_center_color instead
 *      of getting the center-piece cubie manually. That is, unless the cubie has extra information that could be useful
 *      (which 90% of the time it won't)
 *
 *      A Cube is considered solved when all cubies match the centers they touch.
 *      (Red-White-Green should go between the Red, White, and Green centers). It is considered solved regardless of
 *      if it should be rotated differently to the default state (Front: GREEN, Up: WHITE)
 **/

typedef struct CUBE {
    Cubie* GRAND_CUBE[3][3][3];
}Cube;

typedef enum AXIS{
    AXIS_Z,
    AXIS_Y,
    AXIS_X,
    AXIS_NULL
} Axis;

int initialize_cube(Cube* cube);

int set_face_colors(Cube* source, Faces FACE, Colors COLOR);
Colors get_face_center_color(const Cube* source, Faces FACE);
Cubie* get_cubie_at_position(const Cube* source, int x, int y, int z);

int apply_move_from_spec(Cube* source, const MoveSpec* MOVE_SPEC);

int is_solved(const Cube* source);

int parse_move(char** p_MOVE_SPEC, MoveSpec* MOVE_SPEC_DEST);
int apply_move_from_formatted_str(Cube* source, char** p_MOVE_SPEC); //Use only for testing the Cube structure itself.

void print_cube(const Cube* source);

#endif //CUBE_H