//
// Created by Ven on 2025/12/20.
//

#include "Cube.h"
#include "Cubie.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>


Cube* create_cube()
{
    Cube* cube = malloc(sizeof(Cube));
    if (!cube)
    {
        return NULL;
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


    return cube;

    ERROR_FAIL:
        free_cube(cube);
        return NULL;
}

void free_cube(Cube* source)
{
    if (source != NULL)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    free(source -> GRAND_CUBE[i][j][k]);
                }
            }
        }
    }
    free(source);
}

int set_face_colors(Cube* source, const Faces FACE, const Colors COLOR)
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

int move_f(Cube* source, const int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }

    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            Cubie* modifying = get_cubie_at_position(source, x, y, 1);
            if (!modifying || applyRotation(modifying, MOVE_FRONT, count, clockwise))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);

}

int move_u(Cube* source, const int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }

    for (int x = -1; x < 2; x++)
    {
        for (int z = -1; z < 2; z++)
        {
            Cubie* modifying = get_cubie_at_position(source, x, 1, z);
            if (!modifying || applyRotation(modifying, MOVE_UP, count, clockwise))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);
}

int move_r(Cube* source, const int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }
    for (int y = -1; y < 2; y++)
    {
        for (int z = -1; z < 2; z++)
        {
            Cubie* modifying = get_cubie_at_position(source, 1, y, z);
            if (!modifying || applyRotation(modifying, MOVE_RIGHT, count, clockwise))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);
}

int move_b(Cube* source, const int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }
    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            Cubie* modifying = get_cubie_at_position(source, x, y, -1);
            if (!modifying || applyRotation(modifying, MOVE_BACK, count, clockwise))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);
}

int move_d(Cube* source, const int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }
    for (int x = -1; x < 2; x++)
    {
        for (int z = -1; z < 2; z++)
        {
            Cubie* modifying = get_cubie_at_position(source, x, -1, z);
            if (!modifying || applyRotation(modifying, MOVE_DOWN, count, clockwise))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);
}

int move_l(Cube* source, const int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }
    for (int y = -1; y < 2; y++)
    {
        for (int z = -1; z < 2; z++)
        {
            Cubie* modifying = get_cubie_at_position(source, -1, y, z);
            if (!modifying || applyRotation(modifying, MOVE_LEFT, count, clockwise))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);
}

int slice_s(Cube* source, const int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }
    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            Cubie* modifying = get_cubie_at_position(source, x, y, 0);
            if (!modifying || applyRotation(modifying, MOVE_FRONT, count, clockwise))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);
}

int slice_e(Cube* source, const int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }
    for (int x = -1; x < 2; x++)
    {
        for (int z = -1; z < 2; z++)
        {
            Cubie* modifying = get_cubie_at_position(source, x, 0, z);
            if (!modifying || applyRotation(modifying, MOVE_DOWN, count, clockwise))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);
}

int slice_m(Cube* source, const int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }
    for (int y = -1; y < 2; y++)
    {
        for (int z = -1; z < 2; z++)
        {
            Cubie* modifying = get_cubie_at_position(source, 0, y, z);
            if (!modifying || applyRotation(modifying, MOVE_LEFT, count, clockwise))
            {
                return 1;
            }
        }
    }
    return sync_cubie_positions(source);
}

int rotate_about_axis(Cube* source, const Axis AXIS_OF_ROTATION, const int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }
    Moves MOVE_APPLIED;
    switch (AXIS_OF_ROTATION)
    {
        case AXIS_Z: MOVE_APPLIED = MOVE_FRONT; break;
        case AXIS_Y: MOVE_APPLIED = MOVE_UP; break;
        case AXIS_X: MOVE_APPLIED = MOVE_RIGHT; break;
        default: return 1;
    }
    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            for (int z = -1; z < 2; z++)
            {
                Cubie* modifying = get_cubie_at_position(source, x, y, z);
                if (!modifying || applyRotation(modifying, MOVE_APPLIED, count, clockwise))
                {
                    return 1;
                }
            }
        }
    }
    return sync_cubie_positions(source);
}

int apply_move(Cube* source, const Moves MOVE, const int count, const int clockwise)
{
    if (!source)
    {
        return 1;
    }
    switch (MOVE)
    {
        case MOVE_FRONT: return move_f(source, count, clockwise);
        case MOVE_UP: return move_u(source, count, clockwise);
        case MOVE_RIGHT: return move_r(source, count, clockwise);
        case MOVE_BACK: return move_b(source, count, clockwise);
        case MOVE_DOWN: return move_d(source, count, clockwise);
        case MOVE_LEFT: return move_l(source, count, clockwise);

        case SLICE_SIDE: return slice_s(source, count, clockwise);
        case SLICE_EQUATOR: return slice_e(source, count, clockwise);
        case SLICE_MIDDLE: return slice_m(source, count, clockwise);

        case WIDE_FRONT: return (move_f(source, count, clockwise) || slice_s(source, count, clockwise));
        case WIDE_UP: return (move_u(source, count, clockwise) || slice_e(source, count, !clockwise));
        case WIDE_RIGHT: return (move_r(source, count, clockwise) || slice_m(source, count, !clockwise));
        case WIDE_BACK: return (move_b(source, count, clockwise) || slice_s(source, count, !clockwise));
        case WIDE_DOWN: return (move_d(source, count, clockwise) || slice_e(source, count, clockwise));
        case WIDE_LEFT: return (move_l(source, count, clockwise) || slice_m(source, count, clockwise));

        case ROT_Z: return rotate_about_axis(source, AXIS_Z, count, clockwise);
        case ROT_Y: return rotate_about_axis(source, AXIS_Y, count, clockwise);
        case ROT_X: return rotate_about_axis(source, AXIS_X, count, clockwise);

        default: return 1;
    }
}

int sync_cubie_positions(Cube* source)
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

int parse_move(char** p_MOVE_SPEC, Moves* p_MOVE, int* p_count, int* p_clockwise)
{
    if (!p_MOVE_SPEC || !p_MOVE || !p_count || !p_clockwise )
    {
        return 1;
    }


    switch (*p_MOVE_SPEC[0])
    {
        case 'F': *p_MOVE = MOVE_FRONT; break;
        case 'U': *p_MOVE = MOVE_UP; break;
        case 'R': *p_MOVE = MOVE_RIGHT; break;
        case 'B': *p_MOVE = MOVE_BACK; break;
        case 'D': *p_MOVE = MOVE_DOWN; break;
        case 'L': *p_MOVE = MOVE_LEFT; break;

        case 'f': *p_MOVE = WIDE_FRONT; break;
        case 'u': *p_MOVE = WIDE_UP; break;
        case 'r': *p_MOVE = WIDE_RIGHT; break;
        case 'b': *p_MOVE = WIDE_BACK; break;
        case 'd': *p_MOVE = WIDE_DOWN; break;
        case 'l': *p_MOVE = WIDE_LEFT; break;

        case 'S': *p_MOVE = SLICE_SIDE; break;
        case 'E': *p_MOVE = SLICE_EQUATOR; break;
        case 'M': *p_MOVE = SLICE_MIDDLE; break;

        case 'z': *p_MOVE = ROT_Z; break;
        case 'y': *p_MOVE = ROT_Y; break;
        case 'x': *p_MOVE = ROT_X; break;

        default: return 1;
    }

    errno = 0;
    const char query = '\'';
    char* end;
    const char* p_prime = strchr(*p_MOVE_SPEC, query);


    if (p_prime == NULL)
    {
        *p_clockwise = 1;
    }
    else
    {
        if (p_prime != strrchr(*p_MOVE_SPEC, query))
        {
            return 1;
        }
        *p_clockwise = 0;
    }
    const int test = strtol(*p_MOVE_SPEC+1, &end, 10);
    if (errno == ERANGE)
    {
        return 1;
    }
    if (test == 0)
    {
        *p_count = 1;
    }
    else
    {
        *p_count = test;
    }


    return 0;
}

int apply_move_sequence(Cube* source, char** p_MOVE_SEQ)
{
    if (!p_MOVE_SEQ)
    {
        return 1;
    }
    if (!source)
    {
        return 1;
    }
    if (strlen(*p_MOVE_SEQ) == 0)
    {
        return 0;
    }
    char* NORMALIZED_SEQ = malloc(sizeof(char)* (strlen(*p_MOVE_SEQ) + 1));
    if (!NORMALIZED_SEQ)
    {
        return 1;
    }

    NORMALIZED_SEQ[0] = '\0';
    strcat(NORMALIZED_SEQ, *p_MOVE_SEQ);

    if (NORMALIZED_SEQ[strlen(NORMALIZED_SEQ) - 1] != ' ')
    {
        char* p = malloc(sizeof(char)* strlen(NORMALIZED_SEQ) + 2);
        if (!p)
        {
            goto NORMALZED_SEQ_FAIL;
        }
        memcpy(p, NORMALIZED_SEQ, strlen(NORMALIZED_SEQ));
        p[strlen(NORMALIZED_SEQ)] = '\0';
        free(NORMALIZED_SEQ);
        NORMALIZED_SEQ = p;
        strcat(NORMALIZED_SEQ, " ");
    }
    int begin_move_index = 0;
    int end_move_index = 0;

    const size_t len = strlen(NORMALIZED_SEQ);
    for (int i = 0; i < len; i++)
    {
        if (NORMALIZED_SEQ[i] == ' ')
        {
            end_move_index = i;
            Moves MOVE;
            int count;
            int clockwise;

            char saved = NORMALIZED_SEQ[end_move_index];
            char* MOVE_SPEC = NORMALIZED_SEQ + begin_move_index;
            NORMALIZED_SEQ[end_move_index] = '\0';
            int errors = parse_move(&MOVE_SPEC, &MOVE, &count, &clockwise);
            if (errors)
            {
                goto MOVE_SPEC_FAIL;
            }

            errors = apply_move(source, MOVE, count, clockwise);
            if (errors)
            {
                goto MOVE_SPEC_FAIL;
            }
            NORMALIZED_SEQ[end_move_index] = saved;
            begin_move_index = end_move_index + 1;
        }
    }

    free(NORMALIZED_SEQ);
    return 0;

    MOVE_SPEC_FAIL:
    NORMALZED_SEQ_FAIL:
        free(NORMALIZED_SEQ);
        return 1;
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

