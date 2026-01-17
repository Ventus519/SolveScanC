#include "Cubie.h"

#include <stdlib.h>
#include <stdio.h>


Cubie* createCubie(const int x, const int y, const int z)
{
    Cubie* cubie = malloc(sizeof(Cubie));
    if (!cubie)
    {
        return NULL;
    }


    cubie -> position.x = x;
    cubie -> position.y = y;
    cubie -> position.z = z;


    for (int i = 0; i < 6; i++)
    {
        cubie -> FACE_COLORS[i] = COLORS_NULL;
    }

    return cubie;
}

void freeCubie(Cubie* source)
{
    free(source);
}

const Vec3* getCubiePosition(const Cubie* source)
{
    if (!source)
    {
        return NULL;
    }
    return &(source -> position);
}

void setCubiePosition(Cubie* source, const int new_x, const int new_y, const int new_z)
{
    if (!source)
    {
        return;
    }
    source -> position.x = new_x;
    source -> position.y = new_y;
    source -> position.z = new_z;
}

Colors getOppositeColor(const Colors COLOR)
{
    switch (COLOR)
    {
        case GREEN: return BLUE;
        case WHITE: return YELLOW;
        case RED: return ORANGE;
        case BLUE: return GREEN;
        case YELLOW: return WHITE;
        case ORANGE: return RED;
        default: return COLORS_NULL;
    }
}

static int cycleFaces(Cubie* source, const Faces FACE1, const Faces FACE2, const Faces FACE3, const Faces FACE4, const int clockwise)
{
    if (!source)
    {
        return 1;
    }

    if (clockwise)
    {
        const Colors COLOR4 = source -> FACE_COLORS[FACE4];
        source -> FACE_COLORS[FACE4] = source -> FACE_COLORS[FACE3];
        source -> FACE_COLORS[FACE3] = source -> FACE_COLORS[FACE2];
        source -> FACE_COLORS[FACE2] = source -> FACE_COLORS[FACE1];
        source -> FACE_COLORS[FACE1] = COLOR4;
    }
    else
    {
        const Colors COLOR1 = source -> FACE_COLORS[FACE1];
        source -> FACE_COLORS[FACE1] = source -> FACE_COLORS[FACE2];
        source -> FACE_COLORS[FACE2] = source -> FACE_COLORS[FACE3];
        source -> FACE_COLORS[FACE3] = source -> FACE_COLORS[FACE4];
        source -> FACE_COLORS[FACE4] = COLOR1;
    }
    return 0;
}

static int rotateZ(Cubie* source, const int clockwise)
{
    if (!source)
    {
        return 1;
    }

    const Vec3* pos = getCubiePosition(source);
    if (!pos)
    {
        return 1;
    }

    if (clockwise)
    {
        setCubiePosition(source, pos -> y, -pos -> x, pos -> z);
    }
    else
    {
        setCubiePosition(source, -pos -> y, pos -> x, pos -> z);
    }

    return cycleFaces(source, FACE_UP, FACE_RIGHT, FACE_DOWN, FACE_LEFT, clockwise);
}

static int rotateY(Cubie* source, const int clockwise)
{
    if (!source)
    {
        return 1;
    }

    const Vec3* pos = getCubiePosition(source);
    if (!pos)
    {
        return 1;
    }

    if (clockwise)
    {
        setCubiePosition(source, -pos -> z, pos -> y, pos -> x);
    }
    else
    {
        setCubiePosition(source, pos -> z, pos -> y, -pos -> x);
    }
    return cycleFaces(source, FACE_RIGHT, FACE_FRONT, FACE_LEFT, FACE_BACK, clockwise);
}

static int rotateX(Cubie* source, const int clockwise)
{
    if (!source)
    {
        return 1;
    }

    const Vec3* pos = getCubiePosition(source);
    if (!pos)
    {
        return 1;
    }
    if (clockwise)
    {
        setCubiePosition(source, pos -> x, pos -> z, -pos -> y);
    }
    else
    {
        setCubiePosition(source, pos -> x, -pos -> z, pos -> y);
    }
    return  cycleFaces(source, FACE_FRONT, FACE_UP, FACE_BACK, FACE_DOWN, clockwise);
}

int applyRotation(Cubie* source, const MoveSpec* MOVE_SPEC)
{
    if (!source || !MOVE_SPEC)
    {
        return 1;
    }


    for (int i = 0; i < MOVE_SPEC -> count % 4; i++)
    {
        int error_test = 0;
        switch (MOVE_SPEC -> MOVE)
        {
            case MOVE_FRONT:  error_test += rotateZ(source, MOVE_SPEC -> clockwise); break;
            case MOVE_UP:  error_test += rotateY(source, MOVE_SPEC -> clockwise); break;
            case MOVE_RIGHT:  error_test += rotateX(source, MOVE_SPEC -> clockwise); break;
            case MOVE_BACK:  error_test += rotateZ(source, !(MOVE_SPEC -> clockwise)); break;
            case MOVE_DOWN:  error_test += rotateY(source, !(MOVE_SPEC -> clockwise)); break;
            case MOVE_LEFT:  error_test += rotateX(source, !(MOVE_SPEC -> clockwise)); break;
            default: return 1;
        }
        if (error_test != 0)
        {
            return 1;
        }
    }
    return 0;
}

char* MoveSpec_to_str(const MoveSpec* MOVE_SPEC)
{
    if (!MOVE_SPEC)
    {
        return NULL;
    }
    int result_size = 3;
    if (!MOVE_SPEC -> clockwise)
    {
        result_size = 4;
    }

    char* result = malloc(sizeof(char) * result_size);
    switch (MOVE_SPEC -> MOVE)
    {
        case MOVE_FRONT: result[0] = 'F'; break;
        case MOVE_UP: result[0] = 'U'; break;
        case MOVE_RIGHT: result[0] = 'R'; break;
        case MOVE_BACK: result[0] = 'B'; break;
        case MOVE_DOWN: result[0] = 'D'; break;
        case MOVE_LEFT: result[0] = 'L'; break;

        case WIDE_FRONT: result[0] = 'f'; break;
        case WIDE_UP: result[0] = 'u'; break;
        case WIDE_RIGHT: result[0] = 'r'; break;
        case WIDE_BACK: result[0] = 'b'; break;
        case WIDE_DOWN: result[0] = 'd'; break;
        case WIDE_LEFT: result[0] = 'l'; break;

        case SLICE_SIDE: result[0] = 'S'; break;
        case SLICE_EQUATOR: result[0] = 'E'; break;
        case SLICE_MIDDLE: result[0] = 'M'; break;

        case ROT_Z: result[0] = 'z'; break;
        case ROT_Y: result[0] = 'y'; break;
        case ROT_X: result[0] = 'x'; break;

        default: return NULL;
    }

    if (!MOVE_SPEC -> clockwise)
    {
        result[1] = '\'';
        result[2] = ' ';
        result[3] = '\0';
        return result;
    }

    result[1] = ' ';
    result[2] = '\0';
    return result;
}

int isMoveSpecRotation(const MoveSpec* MOVE_SPEC)
{
    if (!MOVE_SPEC)
    {
        return 0;
    }
    if (MOVE_SPEC -> MOVE < ROT_Z || MOVE_SPEC -> MOVE > MOVE_NULL)
    {
        return 0;
    }
    return 1;
}

int isSameCubie(const Cubie* source, const Cubie* other)
{
    if (!source || !other)
    {
        return 0;
    }

    const Vec3* source_pos = getCubiePosition(source);
    const Vec3* other_pos = getCubiePosition(other);
    if (!source_pos || !other_pos)
    {
        return 0;
    }
    if (source_pos -> x != other_pos -> x)
    {
        return 0;
    }
    if (source_pos -> y != other_pos -> y)
    {
        return 0;
    }
    if (source_pos -> z != other_pos -> z)
    {
        return 0;
    }


    for (int i = 0; i < 6; i++)
    {
        if (source -> FACE_COLORS[i] != other -> FACE_COLORS[i])
        {
            return 0;
        }
    }
    return 1;
}

void printCubie(const Cubie* source)
{
    if (!source)
    {
        return;
    }
    const Vec3* source_pos = getCubiePosition(source);
    if (!source_pos)
    {
        return;
    }

    printf("CUBIE POSITION: %d, %d, %d \n", source_pos->x, source_pos->y, source_pos->z);

    for (int i = 0; i < 6; i++)
    {
        switch (i)
        {
            case 0: printf("FRONT: "); break;
            case 1: printf("UP: "); break;
            case 2: printf("RIGHT: "); break;
            case 3: printf("BACK: "); break;
            case 4: printf("DOWN: "); break;
            case 5: printf("LEFT: "); break;
            default: return;
        }
        switch (source -> FACE_COLORS[i])
        {
            case GREEN: puts("GREEN"); break;
            case WHITE: puts("WHITE"); break;
            case RED: puts("RED"); break;
            case BLUE: puts("BLUE"); break;
            case YELLOW: puts("YELLOW"); break;
            case ORANGE: puts("ORANGE"); break;
            case COLORS_NULL: puts("NO COLOR"); break;
            default: return;
        }
    }
    puts("\n");
}