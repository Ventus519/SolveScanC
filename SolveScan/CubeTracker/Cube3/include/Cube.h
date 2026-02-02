#ifndef CUBE_H
#define CUBE_H

#include "Cubie.h"

/** @struct CUBE
 *      Represents a 3x3x3 Rubik's Cube and operates in the same way. (Does not easily expand to larger dimensions)
 *      Each piece of the Cube is considered a cubie (including the core).
 *      Retrieving Cubies of the Cube should be done with get_cubie_at_position() or get_cubie_from_faces()
 *      and not with manual indexing
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
 * Initializes the Cube pointed by using heap-allocated cubies. Do not free the cubies manually, use free_cube().
 * Each cubie is given a position such that the initialized cube is symmetric about the origin.
 * These cubies have positions ranging from (-1, -1, -1) to (1, 1, 1)
 *
 * Cubes are typically not found alone, and are usually members of structs that track the state of the Cube.
 * For that reason, do not call free_cube() on Cube members of those structs except when freeing the struct.
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
 *  The color of a face is determined solely by the color indicated by its center-piece,
 *  this function retrieves that color.
 *
 *  @param source The cube to use when determining the color of the face provided. NULL pointer is considered an error.
 *  @param FACE The face to determine the "color" of. If the Face provided is invalid for whatever reason,
 *              it is considered an error.
 *
 *  @return The color of the face provided, determined by the center-piece on said face.
 *          Returns COLORS_NULL on any error.
 **/
Colors get_face_center_color(const Cube* source, Faces FACE);

/**
 * Borrows the cubie found at the position specified in the cube provided. Use this to work with the Cubies inside
 * the Cube, and refrain from manual Cubie indexing.
 *
 * This function should only be used when iterating over Cubies in the Cube.
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
 * @return Borrowed Cubie* that represents the Cubie at the position specified in the Cube provided.
 *         This Cubie* must not be freed.
 *
 * On the event that invalid coordinates are provided, or that the Cube pointer provided was invalid,
 * NULL will be returned.
 **/
Cubie* get_cubie_at_position(const Cube* source, int x, int y, int z);


/**
 * Borrows the cubie found at the position specified in the cube provided. Use this to work with the Cubies inside
 * the Cube, and refrain from manual Cubie indexing.
 *
 * This function is preferred over get_cubie_at_position for getting known Cubies.
 *
 * The ordering of arguments is meant to match how the pieces are referred to by Cubers. The corner at the intersection
 * of the Front face, Up face, and Right face would be called the UFR corner (the U is for U-face, F for F-face, and R
 * for R-face).
 *
 * Using FACES_NULL for all Face arguments will retrieve the core of the Cube provided.
 *
 * @param source The cube used to borrow the returned Cubie*
 * @param FACE_UD The face along the y-axis that the Cubie lies on. Accepts only: FACE_UP, FACE_DOWN, FACES_NULL
 * @param FACE_FB The face along the z-axis that the Cubie lies on. Accepts only: FACE_FRONT, FACE_BACK, FACES_NULL
 * @param FACE_RL The face along the x-axis that the Cubie lies on. Accepts only: FACE_RIGHT, FACE_LEFT, FACES_NULL
 *
 * @return Borrowed Cubie* that represents the Cubie at the intersection of the faces provided.
 *         This Cubie* must not be freed.
 *
 * On the event that an invalid face combination is provided, or that the Cube pointer provided was invalid,
 * NULL will be returned.
 **/
Cubie* get_cubie_from_faces(const Cube* source, Faces FACE_UD, Faces FACE_FB, Faces FACE_RL);


/**
 * Converts a Cubie* into face notation. Face notation is defined to use the faces that are colored. A face is colored
 * if its color is not COLORS_NULL. If none of the faces on a given axis are colored, then the corresponding destination
 * will be set to FACES_NULL.
 *
 * As an example:
 * @code
 *      Cube cube;
 *      if (initialize_cube(&cube))
 *      {
 *          //error handling
 *      }
 *      Cubie* UFR_PIECE = get_cubie_from_position(&cube, 1, 1, 1);
 *
 *      Faces FACE_UD = FACES_NULL;
 *      Faces FACE_FB = FACES_NULL;
 *      Faces FACE_RL = FACES_NULL;
 *
 *      if (cubie_to_face_notation(UFR_PIECE, &FACE_UD, &FACE_FB, &FACE_RL))
 *      {
 *         //error handling
 *      }
 *
 *      //FACE_UD is set to FACE_UP
 *      //FACE_FB is set to FACE_FRONT
 *      //FACE_RL is set to FACE_RIGHT
 * @endcode
 *
 * @param source The Cubie to convert to face notation
 * @param FACE_UD_DEST  The destination to write which face on the y-axis is colored
 * @param FACE_FB_DEST  The destination to write which face on the z-axis is colored
 * @param FACE_RL_DEST  The destination to write which face on the x-axis is colored
 * @return Error code: 0 on success, 1 on failure (includes NULL pointers and invalid cubies).
 *
 * Cubies are considered invalid if 2 different faces on the same axis are colored. This is because it is impossible
 * for a cubie to have that color scheme while being a part of a larger cube.
 */
int cubie_to_face_notation(const Cubie* source, Faces* FACE_UD_DEST, Faces* FACE_FB_DEST, Faces* FACE_RL_DEST);

/**
 * Applies the move specified by MOVE_SPEC onto the Cube provided. Unlike on the cubie level, this results in the
 * rotation of a single layer, dictated by the MOVE member of MOVE_SPEC. The amount of times this rotation is applied
 * is determined by the count member of MOVE_SPEC modulo 4. The clockwise member indicates the direction to
 * rotate the layer.
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

//Use only for testing the Cube structure itself. Never use if the Cube is a member of another struct
int apply_move_from_formatted_str(Cube* source, const char* MOVE_STR);

void print_cube(const Cube* source); //used for debugging

#endif //CUBE_H