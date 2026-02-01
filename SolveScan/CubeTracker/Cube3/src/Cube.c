//
// Created by Ven on 2025/12/20.
//

#include "Cube.h"
#include "Cubie.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int set_face_colors(Cube* source, const Faces FACE, const Colors COLOR)
{
    Axis FIXED_AXIS;
    int fixed_value;

    switch (FACE)
    {
        case FACE_FRONT: FIXED_AXIS = AXIS_Z; fixed_value = 1; break;
        case FACE_UP: FIXED_AXIS = AXIS_Y; fixed_value = 1; break;
        case FACE_RIGHT: FIXED_AXIS = AXIS_X; fixed_value = 1; break;
        case FACE_BACK: FIXED_AXIS = AXIS_Z; fixed_value = -1; break;
        case FACE_DOWN: FIXED_AXIS = AXIS_Y; fixed_value = -1; break;
        case FACE_LEFT: FIXED_AXIS = AXIS_X; fixed_value = -1; break;
        default: return 1;
    }

    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            Cubie* modifying;
            switch (FIXED_AXIS)
            {
                case AXIS_Z: modifying = get_cubie_at_position(source, i, j, fixed_value); break;
                case AXIS_Y: modifying = get_cubie_at_position(source, i, fixed_value, j); break;
                case AXIS_X: modifying = get_cubie_at_position(source, fixed_value, i, j); break;
                default: return 1;
            }
            if (!modifying)
            {
                return 1;
            }
            modifying -> FACE_COLORS[FACE] = COLOR;
        }
    }
    return 0;
}


int initialize_cube(Cube* cube)
{
    if (!cube)
    {
        return 1;
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                cube -> GRAND_CUBE[i][j][k] = createCubie(i-1, j-1, k-1);
                if (!cube -> GRAND_CUBE[i][j][k])
                {
                    goto ERROR_FAIL;
                }

            }
        }
    }

    if (set_face_colors(cube, FACE_FRONT, GREEN) ||
        set_face_colors(cube, FACE_UP, WHITE) ||
        set_face_colors(cube, FACE_RIGHT, RED) ||
        set_face_colors(cube, FACE_BACK, BLUE) ||
        set_face_colors(cube, FACE_DOWN, YELLOW) ||
        set_face_colors(cube, FACE_LEFT, ORANGE))
    {
        goto ERROR_FAIL;
    }
    return 0;

    ERROR_FAIL:
        free_cube(cube);
        return 1;

}

void free_cube(Cube* cube)
{
    if (!cube)
    {
        return;
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                freeCubie(cube -> GRAND_CUBE[i][j][k]);
            }
        }
    }
    //The cube pointer itself is never freed since it is never allocated in this file
}


Colors get_face_center_color(const Cube* source, const Faces FACE)
{
    if (!source)
    {
        return COLORS_NULL;
    }
    Axis FIXED_AXIS;
    int fixed_value;
    switch (FACE)
    {
        case FACE_FRONT: FIXED_AXIS = AXIS_Z; fixed_value = 1; break;
        case FACE_UP: FIXED_AXIS = AXIS_Y; fixed_value = 1; break;
        case FACE_RIGHT: FIXED_AXIS = AXIS_X; fixed_value = 1; break;
        case FACE_BACK: FIXED_AXIS = AXIS_Z; fixed_value = -1; break;
        case FACE_DOWN: FIXED_AXIS = AXIS_Y; fixed_value = -1; break;
        case FACE_LEFT: FIXED_AXIS = AXIS_X; fixed_value = -1; break;
        default: return COLORS_NULL;
    }
    Cubie* FACE_CENTER;
    switch (FIXED_AXIS)
    {
        case AXIS_Z: FACE_CENTER = get_cubie_at_position(source, 0, 0, fixed_value); break;
        case AXIS_Y: FACE_CENTER = get_cubie_at_position(source, 0, fixed_value, 0); break;
        case AXIS_X: FACE_CENTER = get_cubie_at_position(source, fixed_value, 0, 0); break;
        default: return COLORS_NULL;
    }
    if (!FACE_CENTER)
    {
        return COLORS_NULL;
    }
    return FACE_CENTER -> FACE_COLORS[FACE];

}

Cubie* get_cubie_at_position(const Cube* source, const int x, const int y, const int z)
{
    if (!source)
    {
        return NULL;
    }

    if (abs(x) >= 2 || abs(y) >= 2 || abs(z) >= 2)
    {
        return NULL;
    }

    return (source -> GRAND_CUBE[x+1][y+1][z+1]);
}

Cubie* get_cubie_from_faces(const Cube* source, const Faces FACE_UD, const Faces FACE_FB, const Faces FACE_RL)
{
    if (!source)
    {
        return NULL;
    }
    int x_offset, y_offset, z_offset;
    switch (FACE_RL)
    {
        case FACE_RIGHT: x_offset = 1; break;
        case FACE_LEFT: x_offset = -1; break;
        case FACES_NULL: x_offset = 0; break;
        default: return NULL;
    }
    switch (FACE_UD)
    {
        case FACE_UP: y_offset = 1; break;
        case FACE_DOWN: y_offset = -1; break;
        case FACES_NULL: y_offset = 0; break;
        default: return NULL;
    }

    switch (FACE_FB)
    {
        case FACE_FRONT: z_offset = 1; break;
        case FACE_BACK: z_offset = -1; break;
        case FACES_NULL: z_offset = 0; break;
        default: return NULL;
    }

    return (source -> GRAND_CUBE[x_offset+1][y_offset+1][z_offset+1]);
}

static int sync_cubie_positions(Cube* source)
{
    if (!source)
    {
        return 1;
    }
    Cubie* RESYNCED_CUBIES[3][3][3];
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                Cubie* modifying = get_cubie_at_position(source, i-1, j-1, k-1);
                const Vec3* pos = getCubiePosition(modifying);
                if (!modifying || !pos)
                {
                    return 1;
                }

                //check for valid position
                if (abs(pos -> x) >= 2 || abs(pos -> y) >= 2 || abs(pos -> z) >= 2)
                {
                    return 1;
                }
                RESYNCED_CUBIES[pos->x+1][pos->y+1][pos->z+1] = modifying;
            }
        }
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                source -> GRAND_CUBE[i][j][k] = RESYNCED_CUBIES[i][j][k];
            }
        }
    }
    return 0;
}


static int move_f(Cube* source, const unsigned int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }

    MoveSpec FRONT_SPEC;
    FRONT_SPEC.MOVE = MOVE_FRONT;
    FRONT_SPEC.count = count;
    FRONT_SPEC.clockwise = clockwise;


    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            Cubie* modifying = get_cubie_at_position(source, x, y, 1);
            if (!modifying || applyRotation(modifying, &FRONT_SPEC))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);

}

static int move_u(Cube* source, const unsigned int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }

    MoveSpec UP_SPEC;
    UP_SPEC.MOVE = MOVE_UP;
    UP_SPEC.count = count;
    UP_SPEC.clockwise = clockwise;

    for (int x = -1; x < 2; x++)
    {
        for (int z = -1; z < 2; z++)
        {
            Cubie* modifying = get_cubie_at_position(source, x, 1, z);
            if (!modifying || applyRotation(modifying, &UP_SPEC))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);
}

static int move_r(Cube* source, const unsigned int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }

    MoveSpec RIGHT_SPEC;
    RIGHT_SPEC.MOVE = MOVE_RIGHT;
    RIGHT_SPEC.count = count;
    RIGHT_SPEC.clockwise = clockwise;

    for (int y = -1; y < 2; y++)
    {
        for (int z = -1; z < 2; z++)
        {
            Cubie* modifying = get_cubie_at_position(source, 1, y, z);
            if (!modifying || applyRotation(modifying, &RIGHT_SPEC))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);
}

static int move_b(Cube* source, const unsigned int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }

    MoveSpec BACK_SPEC;
    BACK_SPEC.MOVE = MOVE_BACK;
    BACK_SPEC.count = count;
    BACK_SPEC.clockwise = clockwise;

    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            Cubie* modifying = get_cubie_at_position(source, x, y, -1);
            if (!modifying || applyRotation(modifying, &BACK_SPEC))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);
}

static int move_d(Cube* source, const unsigned int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }

    MoveSpec DOWN_SPEC;
    DOWN_SPEC.MOVE = MOVE_DOWN;
    DOWN_SPEC.count = count;
    DOWN_SPEC.clockwise = clockwise;

    for (int x = -1; x < 2; x++)
    {
        for (int z = -1; z < 2; z++)
        {
            Cubie* modifying = get_cubie_at_position(source, x, -1, z);
            if (!modifying || applyRotation(modifying, &DOWN_SPEC))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);
}

static int move_l(Cube* source, const unsigned int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }

    MoveSpec LEFT_SPEC;
    LEFT_SPEC.MOVE = MOVE_LEFT;
    LEFT_SPEC.count = count;
    LEFT_SPEC.clockwise = clockwise;

    for (int y = -1; y < 2; y++)
    {
        for (int z = -1; z < 2; z++)
        {
            Cubie* modifying = get_cubie_at_position(source, -1, y, z);
            if (!modifying || applyRotation(modifying, &LEFT_SPEC))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);
}

static int slice_s(Cube* source, const unsigned int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }

    MoveSpec S_SLICE_SPEC;
    S_SLICE_SPEC.MOVE = MOVE_FRONT;
    S_SLICE_SPEC.count = count;
    S_SLICE_SPEC.clockwise = clockwise;

    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            Cubie* modifying = get_cubie_at_position(source, x, y, 0);
            if (!modifying || applyRotation(modifying, &S_SLICE_SPEC))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);
}

static int slice_e(Cube* source, const unsigned int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }

    MoveSpec E_SLICE_SPEC;
    E_SLICE_SPEC.MOVE = MOVE_DOWN;
    E_SLICE_SPEC.count = count;
    E_SLICE_SPEC.clockwise = clockwise;

    for (int x = -1; x < 2; x++)
    {
        for (int z = -1; z < 2; z++)
        {
            Cubie* modifying = get_cubie_at_position(source, x, 0, z);
            if (!modifying || applyRotation(modifying, &E_SLICE_SPEC))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);
}

static int slice_m(Cube* source, const unsigned int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }

    MoveSpec M_SLICE_SPEC;
    M_SLICE_SPEC.MOVE = MOVE_LEFT;
    M_SLICE_SPEC.count = count;
    M_SLICE_SPEC.clockwise = clockwise;

    for (int y = -1; y < 2; y++)
    {
        for (int z = -1; z < 2; z++)
        {
            Cubie* modifying = get_cubie_at_position(source, 0, y, z);
            if (!modifying || applyRotation(modifying, &M_SLICE_SPEC))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);
}

static int rotate_about_axis(Cube* source, const Axis AXIS_OF_ROTATION, const unsigned int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }
    MoveSpec MOVE_APPLIED_SPEC;
    MOVE_APPLIED_SPEC.count = count;
    MOVE_APPLIED_SPEC.clockwise = clockwise;

    switch (AXIS_OF_ROTATION)
    {
        case AXIS_Z: MOVE_APPLIED_SPEC.MOVE = MOVE_FRONT; break;
        case AXIS_Y: MOVE_APPLIED_SPEC.MOVE = MOVE_UP; break;
        case AXIS_X: MOVE_APPLIED_SPEC.MOVE = MOVE_RIGHT; break;
        default: return 1;
    }
    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            for (int z = -1; z < 2; z++)
            {
                Cubie* modifying = get_cubie_at_position(source, x, y, z);
                if (!modifying || applyRotation(modifying, &MOVE_APPLIED_SPEC))
                {
                    return 1;
                }
            }
        }
    }
    return sync_cubie_positions(source);
}

int apply_move_from_spec(Cube* source, const MoveSpec* MOVE_SPEC)
{
    if (!source || !MOVE_SPEC)
    {
        return 1;
    }
    switch (MOVE_SPEC -> MOVE)
    {
        case MOVE_FRONT: return move_f(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise);
        case MOVE_UP: return move_u(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise);
        case MOVE_RIGHT: return move_r(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise);
        case MOVE_BACK: return move_b(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise);
        case MOVE_DOWN: return move_d(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise);
        case MOVE_LEFT: return move_l(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise);

        case SLICE_SIDE: return slice_s(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise);
        case SLICE_EQUATOR: return slice_e(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise);
        case SLICE_MIDDLE: return slice_m(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise);

        case WIDE_FRONT: return (move_f(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise) || slice_s(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise));
        case WIDE_UP: return (move_u(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise) || slice_e(source, MOVE_SPEC -> count, !MOVE_SPEC -> clockwise));
        case WIDE_RIGHT: return (move_r(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise) || slice_m(source, MOVE_SPEC -> count, !MOVE_SPEC -> clockwise));
        case WIDE_BACK: return (move_b(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise) || slice_s(source, MOVE_SPEC -> count, !MOVE_SPEC -> clockwise));
        case WIDE_DOWN: return (move_d(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise) || slice_e(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise));
        case WIDE_LEFT: return (move_l(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise) || slice_m(source, MOVE_SPEC -> count, MOVE_SPEC -> clockwise));

        case ROT_Z: return rotate_about_axis(source, AXIS_Z, MOVE_SPEC -> count, MOVE_SPEC -> clockwise);
        case ROT_Y: return rotate_about_axis(source, AXIS_Y, MOVE_SPEC -> count, MOVE_SPEC -> clockwise);
        case ROT_X: return rotate_about_axis(source, AXIS_X, MOVE_SPEC -> count, MOVE_SPEC -> clockwise);

        case MOVE_NULL: return 0; //the move is still valid

        default: return 1; //if somehow an invalid move is provided
    }
}

int is_solved(const Cube* source)
{
    if (!source)
    {
        return 0;
    }
    const Cubie* CENTER_F = get_cubie_at_position(source, 0, 0, 1);
    const Cubie* CENTER_U = get_cubie_at_position(source, 0, 1, 0);
    const Cubie* CENTER_R = get_cubie_at_position(source, 1, 0, 0);
    const Cubie* CENTER_B = get_cubie_at_position(source, 0, 0, -1);
    const Cubie* CENTER_D = get_cubie_at_position(source, 0, -1, 0);
    const Cubie* CENTER_L = get_cubie_at_position(source, -1, 0, 0);

    if (!CENTER_F || !CENTER_U || !CENTER_R || !CENTER_B || !CENTER_D || !CENTER_L)
    {
        return 0;
    }

    const Colors COLOR_F = CENTER_F -> FACE_COLORS[FACE_FRONT];
    const Colors COLOR_U = CENTER_U -> FACE_COLORS[FACE_UP];
    const Colors COLOR_R = CENTER_R -> FACE_COLORS[FACE_RIGHT];
    const Colors COLOR_B = CENTER_B -> FACE_COLORS[FACE_BACK];
    const Colors COLOR_D = CENTER_D -> FACE_COLORS[FACE_DOWN];
    const Colors COLOR_L = CENTER_L -> FACE_COLORS[FACE_LEFT];

    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            const Cubie* QUERY_F = get_cubie_at_position(source, i, j, 1);
            const Cubie* QUERY_U = get_cubie_at_position(source, i, 1, j);
            const Cubie* QUERY_R = get_cubie_at_position(source, 1, i, j);
            const Cubie* QUERY_B = get_cubie_at_position(source, i, j, -1);
            const Cubie* QUERY_D = get_cubie_at_position(source, i, -1, j);
            const Cubie* QUERY_L = get_cubie_at_position(source, -1, i, j);
            if (!QUERY_F || !QUERY_U || !QUERY_R || !QUERY_B || !QUERY_D || !QUERY_L)
            {
                return 0;
            }
            if (QUERY_F -> FACE_COLORS[FACE_FRONT] != COLOR_F)
            {
                return 0;
            }
            if (QUERY_U -> FACE_COLORS[FACE_UP] != COLOR_U)
            {
                return 0;
            }
            if (QUERY_R -> FACE_COLORS[FACE_RIGHT] != COLOR_R)
            {
                return 0;
            }
            if (QUERY_B -> FACE_COLORS[FACE_BACK] != COLOR_B)
            {
                return 0;
            }
            if (QUERY_D -> FACE_COLORS[FACE_DOWN] != COLOR_D)
            {
                return 0;
            }
            if (QUERY_L -> FACE_COLORS[FACE_LEFT] != COLOR_L)
            {
                return 0;
            }
        }
    }
    return 1;
}

int parse_move(const char* MOVE_STR, MoveSpec* MOVE_SPEC_DEST)
{
    if (!MOVE_STR || !MOVE_SPEC_DEST)
    {
        return 1;
    }


    switch (MOVE_STR[0])
    {
        case 'F': MOVE_SPEC_DEST -> MOVE = MOVE_FRONT; break;
        case 'U': MOVE_SPEC_DEST -> MOVE = MOVE_UP; break;
        case 'R': MOVE_SPEC_DEST -> MOVE = MOVE_RIGHT; break;
        case 'B': MOVE_SPEC_DEST -> MOVE = MOVE_BACK; break;
        case 'D': MOVE_SPEC_DEST -> MOVE = MOVE_DOWN; break;
        case 'L': MOVE_SPEC_DEST -> MOVE = MOVE_LEFT; break;

        case 'f': MOVE_SPEC_DEST -> MOVE = WIDE_FRONT; break;
        case 'u': MOVE_SPEC_DEST -> MOVE = WIDE_UP; break;
        case 'r': MOVE_SPEC_DEST -> MOVE = WIDE_RIGHT; break;
        case 'b': MOVE_SPEC_DEST -> MOVE = WIDE_BACK; break;
        case 'd': MOVE_SPEC_DEST -> MOVE = WIDE_DOWN; break;
        case 'l': MOVE_SPEC_DEST -> MOVE = WIDE_LEFT; break;

        case 'S': MOVE_SPEC_DEST -> MOVE = SLICE_SIDE; break;
        case 'E': MOVE_SPEC_DEST -> MOVE = SLICE_EQUATOR; break;
        case 'M': MOVE_SPEC_DEST -> MOVE = SLICE_MIDDLE; break;

        case 'z': MOVE_SPEC_DEST -> MOVE = ROT_Z; break;
        case 'y': MOVE_SPEC_DEST -> MOVE = ROT_Y; break;
        case 'x': MOVE_SPEC_DEST -> MOVE = ROT_X; break;

        default: return 1;
    }

    errno = 0;
    char* end;

    const int test = strtol(MOVE_STR + 1, &end, 10);
    if (errno == ERANGE)
    {
        return 1;
    }
    if (test == 0)
    {
        MOVE_SPEC_DEST -> count = 1;
    }
    else
    {
        MOVE_SPEC_DEST -> count = test;
    }

    switch (*end)
    {
        case '\0': MOVE_SPEC_DEST -> clockwise = 1; break;
        case '\'': MOVE_SPEC_DEST -> clockwise = 0; break;
        default: return 1;
    }
    return 0;
}


int apply_move_from_formatted_str(Cube* source, const char* MOVE_SPEC)
{
    if (!source || !MOVE_SPEC)
    {
        return 1;
    }

    MoveSpec MOVE;
    if (parse_move(MOVE_SPEC, &MOVE))
    {
        return 1;
    }

    return apply_move_from_spec(source, &MOVE);
}

void print_cube(const Cube* source)
{
    if (!source)
    {
        return;
    }
    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            for (int z = -1; z < 2; z++)
            {
                const Cubie* observed = get_cubie_at_position(source, x, y, z);
                printCubie(observed);
            }
        }
    }
}

