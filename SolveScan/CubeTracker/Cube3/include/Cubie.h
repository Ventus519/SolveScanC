#ifndef CUBIE_H
#define CUBIE_H

/**
 * @enum MOVES
 *      Represents the valid moves that can be applied to a 3x3x3 Rubik's Cube.
 *      MOVE_NULL represents no move being applied to the cube. This is different semantically to changing the count
 *      to 0, but not different functionally.
 *
 *      MOVE_NULL usually is never seen unless:
 *              - There was an error
 *              - It was manually set
 *              - A MoveStack* has just been initialized and no moves have been added
 *
 **/

typedef enum MOVES
{
    //FACE TURNS
    MOVE_FRONT,
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_BACK,
    MOVE_DOWN,
    MOVE_LEFT,

    //WIDE MOVES
    WIDE_FRONT,
    WIDE_UP,
    WIDE_RIGHT,
    WIDE_BACK,
    WIDE_DOWN,
    WIDE_LEFT,

    //SLICE MOVES
    SLICE_SIDE,
    SLICE_EQUATOR,
    SLICE_MIDDLE,

    //CUBE ROTATIONS
    ROT_Z,
    ROT_Y,
    ROT_X,

    //NO MOVE
    MOVE_NULL
} Moves;

/**
 * @enum COLORS
 *          Represents the 6 standard colors on a Rubik's Cube, as well as the colorless COLORS_NULL
 *
 *          For the purposes of a 3x3x3 Cube, all pieces have at least 3 colorless faces (face colors of value COLORS_NULL).
 *
 *          For the purposes of a 1x1x1 Cube, the singular piece has no colorless faces.
 *
 *          The ordering of the colors uses Color on z-axis, Color on y-axis, then Color on x-axis, and then followed by
 *          their opposites in the same order
 **/

typedef enum COLORS
{
    GREEN,
    WHITE,
    RED,
    BLUE,
    YELLOW,
    ORANGE,
    COLORS_NULL
} Colors;

typedef enum FACES
{
    FACE_FRONT,
    FACE_UP,
    FACE_RIGHT,
    FACE_BACK,
    FACE_DOWN,
    FACE_LEFT
} Faces;

typedef struct VEC3 {
    int x;
    int y;
    int z;
} Vec3;

/**
 * @struct CUBIE
 *          Represents a 1x1x1 Cube, which can be used to represent pieces of any larger sized Rubik's Cube.
 *
 *          The coordinate system follows the axes of rotation from standard WCA moves:
 *              -CORE: The colorless origin of the Cube (0, 0, 0)
 *              -+x : direction from the core towards the R face
 *              -+y : direction from the core towards the U face
 *              -+z : direction from the core towards the F face
 *
 *          When making structures to represent larger-sized cubes with cubies, you must make position (0, 0, 0) the center/core of the cube for any rotations to work properly.
 *          As an example, a 3x3x3 Cube would have coordinates ranging from (-1, -1, -1) to (1, 1, 1), symmetrical about (0, 0, 0).
 *
 *          It is recommended to use createCubie() for heap-allocated Cubies. Cubies allocated that way will be owned by the caller of createCubie().
 *
 *          To check the face color via indexing into FACE_COLORS, using enum FACES is preferred whenever possible.
 *
 *          Example:
 *          @code
 *              Cubie* tiny_cube = create_cubie(1, 1, 1);
 *              Colors FRONT_COLOR = tiny_cube -> FACE_COLORS[FACE_FRONT];
 *          @endcode
 **/

typedef struct CUBIE
{
    Vec3 position;
    Colors FACE_COLORS[6];
} Cubie;

/**
 * @struct MOVESPEC
 *         Represents a specification for a move that can be applied to a Cube. Applying one to a Cubie results in a pure rotation.
 *
 *         The MOVE member indicates which layer to apply the move to.
 *
 *         The count member indicates the amount of times to consecutively apply the move. On a cube/cubie, applying a move 4 consecutive times results in
 *         a net 0 change. A count of 0 is functionally the same as a count of 4; both result in no net change to the state of the cube/cubie.
 *
 *         The clockwise member indicates the direction to apply the move when facing the face being rotated. A value of 1 indicates clockwise, while a value of 0
 *         indicates counterclockwise. Any other values are considered invalid
 *
 *         These moves are equivalent to standard WCA move notation as shown below:
 *         @code
 *              MoveSpec MOVE_SPEC;
 *              MOVE_SPEC.MOVE = MOVE_RIGHT;
 *              MOVE_SPEC.count = 3;
 *              MOVE_SPEC.clockwise = 1;
 *
 *              //applying MOVE_SPEC on a cube results in the expected result of applying R3 on a Cube.
 *         @endcode
 *         Although applying a single move 3 times results in the same as applying it once in the reverse direction, those are both treated as separate move specifications.
 *         R3 does convey something different to R' even though they result in the same final cube state.
 *
 **/
typedef struct MOVESPEC
{
    Moves MOVE;
    unsigned int count;
    int clockwise;
} MoveSpec;

/**
 *  Creates a Cubie at the provided integer coordinates.
 *  @param x x position of the desired Cubie
 *  @param y y position of the desired Cubie
 *  @param z z position of the desired Cubie
 *
 *  Using (x, y, z) = (0, 0, 0) indicates that the cubie to be created is the core of a cube.
 *
 *  @return Heap-allocated Cubie* owned by the caller unless otherwise stated.
 **/
Cubie* createCubie(int x, int y, int z);
void freeCubie(Cubie* source); //Do not call this if the Cubie is owned by a Cube

const Vec3* getCubiePosition(const Cubie* source);
void setCubiePosition(Cubie* source, int new_x, int new_y, int new_z);

Colors getOppositeColor(Colors COLOR); //The opposite color is defined to be the color on the opposite face on the same axis. Passing COLORS_NULL results in COLORS_NULL.

/**
 * Applies a rotation onto the source Cubie given the specified MoveSpec. The MOVE member of the MoveSpec* provided corresponds to the following Cubie rotations:
 *  - MOVE_FRONT: Rotation about the Z-axis (default: clockwise when facing the FRONT face)
 *  - MOVE_UP: Rotation about the Y-axis (default: clockwise when facing the UP face)
 *  - MOVE_RIGHT: Rotation about the X-axis (default: clockwise when facing the RIGHT face)
 *  - MOVE_BACK: Rotation about the Z-axis (default: counterclockwise when facing the FRONT face)
 *  - MOVE_DOWN: Rotation about the Y-axis (default: counterclockwise when facing the UP face)
 *  - MOVE_LEFT: Rotation about the X-axis (default: counterclockwise when facing the RIGHT face)
 *
 *  Although these moves typically result in different layers being turned on a larger cube, they result in the same rotation on a 1x1x1 Cubie.
 *  Thus, 1 application of MOVE_FRONT clockwise yields the same result as 1 application of MOVE_BACK counterclockwise as they both are fundamentally
 *  1 clockwise rotation of the Z-axis
 *
 *  @param source The Cubie to apply the rotation to
 *  @param MOVE_SPEC The move specification (MOVE dictates which rotation to do, count is the amount of times to apply the rotation, and clockwise indicates the direction to
 *  apply the rotation)
 *
 *  Only call this function when working on the Cubie level. If working with a larger sized cube, then make functions to handle moves on that cube for minimal confusion.
 *
 *  @return Error code: 0 on success and 1 on fail.
 **/
int applyRotation(Cubie* source, const MoveSpec* MOVE_SPEC);

/**
 * Converts a MoveSpec to an equivalent WCA move notation as a string.
 *
 * Some things to note about this conversion:
 *      Although the count member of MOVE_SPEC can go up to the integer limit, the converted string will not use that count in its representation and instead use the net rotations
 *      applied on a layer after a full cycle.
 *
 *      as an example:
 *      @code
 *          MoveSpec MOVE_SPEC;
 *          MOVE_SPEC.MOVE = MOVE_RIGHT;
 *          MOVE_SPEC.count = 15;
 *          MOVE_SPEC.clockwise = 0;
 *
 *          char* formatted_move = MoveSpec_to_str(MOVE_SPEC);
 *          //formatted_move stores "R7", and not "R3" nor "R15".
 *      @endcode
 * @param MOVE_SPEC the move specification to convert to standard notation.
 * @return Heap-allocated, null-terminated, char* holding the move formatted by WCA notation. The caller is responsible for freeing.
 **/
char* MoveSpec_to_str(const MoveSpec* MOVE_SPEC);

/**
 * Determines if a given Move Specification is a pure rotation about some axis of rotation when applied to a larger Cube.
 * This is useful for determining if the inspection step of a solve has ended as the inspection step only allows rotations before actual moves.
 * MOVE_NULL counts as a rotation, since it is the same as doing nothing to the Cube (which is the same as a rotation applied 4 times).
 *
 * @param MOVE_SPEC The move specification to check
 * @return Boolean: 1 if the move results in a pure rotation, 0 if otherwise (including NULL pointer)
 */
int isMoveSpecRotation(const MoveSpec* MOVE_SPEC);

/**
 * Determines if two Cubies convey the same information determined by their members.
 * They must have the same position and same face color sequence for a return value of 1.
 *
 * @param source The cubie being compared against
 * @param other The cubie to compare with source
 *
 * @return 1 if both convey the same information, 0 otherwise. If either is a null pointer, 0 is returned.
 **/
int isSameCubie(const Cubie* source, const Cubie* other);

void printCubie(const Cubie* source); //debug utility to print the cubie state to stdout

#endif // CUBIE_H