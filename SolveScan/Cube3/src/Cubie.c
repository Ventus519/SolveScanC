#include "Cubie.h"
#include "Vec3.h"

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

int cycleFaces(Cubie* source, const Faces FACE1, const Faces FACE2, const Faces FACE3, const Faces FACE4, const int clockwise)
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

int rotateZ(Cubie* source, const int clockwise)
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

int rotateY(Cubie* source, const int clockwise)
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

int rotateX(Cubie* source, const int clockwise)
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

int applyRotation(Cubie* source, const Moves MOVE, const int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }


    for (int i = 0; i < count % 4; i++)
    {
        int error_test = 0;
        switch (MOVE)
        {
            case MOVE_FRONT:  error_test += rotateZ(source, clockwise); break;
            case MOVE_UP:  error_test += rotateY(source, clockwise); break;
            case MOVE_RIGHT:  error_test += rotateX(source, clockwise); break;
            case MOVE_BACK:  error_test += rotateZ(source, !clockwise); break;
            case MOVE_DOWN:  error_test += rotateY(source, !clockwise); break;
            case MOVE_LEFT:  error_test += rotateX(source, !clockwise); break;
            default: return 1;
        }
        if (error_test != 0)
        {
            return 1;
        }
    }
    return 0;
}

int isSame(const Cubie* source, const Cubie* other)
{
    if (!source || !other)
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