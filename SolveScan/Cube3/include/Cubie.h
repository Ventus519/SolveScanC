#ifndef CUBIE_H
#define CUBIE_H

typedef enum MOVES
{
    MOVE_FRONT,
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_BACK,
    MOVE_DOWN,
    MOVE_LEFT,

    WIDE_FRONT,
    WIDE_UP,
    WIDE_RIGHT,
    WIDE_BACK,
    WIDE_DOWN,
    WIDE_LEFT,

    SLICE_SIDE,
    SLICE_EQUATOR,
    SLICE_MIDDLE,

    ROT_Z,
    ROT_Y,
    ROT_X,

    MOVE_NULL
} Moves;

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

typedef struct CUBIE
{
    Vec3 position;
    Colors FACE_COLORS[6];
} Cubie;

typedef struct MOVESPEC
{
    Moves MOVE;
    int count;
    int clockwise;
} MoveSpec;



Cubie* createCubie(int x, int y, int z);
void freeCubie(Cubie* source);

const Vec3* getCubiePosition(const Cubie* source);
void setCubiePosition(Cubie* source, int new_x, int new_y, int new_z);

Colors getOppositeColor(Colors COLOR);

int cycleFaces(Cubie* source, Faces FACE1, Faces FACE2, Faces FACE3, Faces FACE4, int clockwise);

int rotateZ(Cubie* source, int clockwise);
int rotateY(Cubie* source, int clockwise);
int rotateX(Cubie* source, int clockwise);

int applyRotation(Cubie* source, const MoveSpec* MOVE_SPEC);
char* MoveSpec_to_str(const MoveSpec* MOVE_SPEC);

int isSame(const Cubie* source, const Cubie* other);

void printCubie(const Cubie* source);

#endif // CUBIE_H