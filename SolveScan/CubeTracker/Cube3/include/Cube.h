#ifndef CUBE_H
#define CUBE_H

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

/**
 * @enum AXIS
 *       This represents the axes of the Cube. They are ordered such that the axes correspond to the order of Faces.
 *       (FACE_FRONT is on AXIS_Z, FACE_UP is on AXIS_Y, and FACE_RIGHT is on AXIS_X)
 *
 *       AXIS NULL should be seen only on an error when determining axis of a face.
 */
typedef enum AXIS{
    AXIS_Z,
    AXIS_Y,
    AXIS_X,
    AXIS_NULL
} Axis;

/**
 * Initializes the Cube pointed by using heap-allocated cubies. Do not free the cubies manually, use free_cube(). Each cubie is given a position such that
 * the initialized cube is symmetric about the origin. These cubies have positions ranging from (-1, -1, -1) to (1, 1, 1)
 *
 * Cubes are typically not found alone, and are usually members of structs that track the state of the Cube. For that reason,
 * do not call free_cube() on Cube members of those structs except when freeing the struct.
 *
 * On the event that a NULL pointer is provided, the function will return 1.
 * @param cube Pointer to the Cube struct whose members are to be initialized
 *
 * @return Error Code: 0 on success, 1 on failure
 **/
int initialize_cube(Cube* cube);


/**
 * Frees the heap-allocated Cubies. Call only when the Cube is no longer needed.
 *
 * On the event that a NULL pointer is passed, nothing will be freed.
 **/
void free_cube(Cube* cube);

/**
 *  The color of a face is determined solely by the color indicated by its center-piece, this function retrieves that color.
 *
 *  @param source The cube to use when determining the color of the face provided. NULL pointer is considered an error.
 *  @param FACE The face to determine the "color" of. If the Face provided is invalid for whatever reason, it is considered an error.
 *
 *  @return The color of the face provided, determined by the center-piece on said face. Returns COLORS_NULL on any error.
 **/
Colors get_face_center_color(const Cube* source, Faces FACE);

/**
 * Borrows the cubie found at the position specified in the cube provided. Use this to work with the Cubies inside the Cube, and
 * refrain from manual Cubie indexing.
 *
 * Recall:
 *      The core is at (0, 0, 0) and the cube must be symmetric about it.
 *      +x is the direction from the core to the R face
 *      +y is the direction from the core to the U face
 *      +z is the direction from the core to the F face
 *
 * @param source The cube used to borrow the returned Cubie*
 * @param x The x-coordinate of the Cubie* to obtain (ranges from -1 to 1)
 * @param y The y-coordinate of the Cubie* to obtain (ranges from -1 to 1)
 * @param z The z-coordinate of the Cubie* to obtain (ranges from -1 to 1)
 *
 * @return Borrowed Cubie* that represents the Cubie at the position specified in the Cube provided. This Cubie* must not be freed.
 *
 * On the event that invalid coordinates are provided, or that the Cube provided was invalid, NULL will be returned.
 **/
Cubie* get_cubie_at_position(const Cube* source, int x, int y, int z);


/**
 * Applies the move specified by MOVE_SPEC onto the Cube provided. Unlike on the cubie level, this results in the rotation of a single layer,
 * dictated by the MOVE member of MOVE_SPEC. The amount of times this rotation is applied is determined by the count member of MOVE_SPEC modulo 4.
 * The clockwise member indicates the direction to rotate the layer.
 *
 * @param source The Cube to apply the move specified
 * @param MOVE_SPEC The specified move to apply
 *
 * @return Error Code: 0 on success, 1 on failure
 **/
int apply_move_from_spec(Cube* source, const MoveSpec* MOVE_SPEC);


/**
 * Determines if a Cube is solved, regardless of orientation.
 *
 * A Cube is considered solved if the colors on each face agree with the face's respective center.
 *
 * @param source The cube queried. NULL pointer is considered invalid
 * @return Boolean: 0 if the Cube provided is unsolved or invalid, 1 if the Cube is solved.
 **/
int is_solved(const Cube* source);


/**
 * Parses the null-terminated MOVE_STR.
 * If the string is a valid move to apply on a cube (in WCA notation), the Move described is written to MOVE_SPEC_DEST.
 *
 * This function should be used when applying moves from a string.
 *
 * @param MOVE_STR the null-terminated string to parse. (causes failure on null-pointer)
 * @param MOVE_SPEC_DEST the destination to send the parsed move. (causes failure on null-pointer)
 *
 * @return Error code: 0 on success, 1 on failure.
 */
int parse_move(const char* MOVE_STR, MoveSpec* MOVE_SPEC_DEST);
int apply_move_from_formatted_str(Cube* source, const char* MOVE_STR); //Use only for testing the Cube structure itself. Never use if the Cube is a member of another struct

void print_cube(const Cube* source); //used for debugging

#endif //CUBE_H