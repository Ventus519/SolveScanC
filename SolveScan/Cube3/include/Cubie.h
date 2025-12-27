#ifndef CUBIE_H
#define CUBIE_H
#include "Vec3.h"

typedef enum
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
} Moves;

typedef enum
{
    GREEN,
    WHITE,
    RED,
    BLUE,
    YELLOW,
    ORANGE,
    COLORS_NULL
} Colors;

typedef enum
{
    FACE_FRONT,
    FACE_UP,
    FACE_RIGHT,
    FACE_BACK,
    FACE_DOWN,
    FACE_LEFT
} Faces;

typedef struct
{
    Vec3 position;
    Colors FACE_COLORS[6];
} Cubie;

Cubie* createCubie(int x, int y, int z);
void freeCubie(Cubie* source);

const Vec3* getCubiePosition(const Cubie* source);
void setCubiePosition(Cubie* source, int new_x, int new_y, int new_z);

Colors getOppositeColor(Colors COLOR);

int cycleFaces(Cubie* source, Faces FACE1, Faces FACE2, Faces FACE3, Faces FACE4, int clockwise);

int rotateZ(Cubie* source, int clockwise);
int rotateY(Cubie* source, int clockwise);
int rotateX(Cubie* source, int clockwise);

int applyRotation(Cubie* source, Moves MOVE, int count, int clockwise);

int isSame(const Cubie* source, const Cubie* other);

void printCubie(const Cubie* source);

#endif // CUBIE_H