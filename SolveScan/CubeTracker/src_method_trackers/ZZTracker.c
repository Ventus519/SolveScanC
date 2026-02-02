//
// Created by Ven519 on 2026/01/31.
//

#include "ZZTracker.h"

#include <stdlib.h>

int initialize_ZZTracker(ZZTracker* tracker)
{
    if (!tracker)
    {
        return 1;
    }
    if (initialize_cube(&tracker -> CUBE))
    {
        return 1;
    }
    tracker -> STEP = ZZ_SCRAMBLE;
    return 0;
}

void free_ZZTracker(ZZTracker* tracker)
{
    if (!tracker)
    {
        return;
    }
    free(tracker);
}

static int is_cubie_edge(const Cubie* target)
{
    if (!target)
    {
        return 0;
    }
    int count_colored_faces = 0;
    for (Faces FACE = FACE_FRONT; FACE != FACES_NULL; FACE++)
    {
        if (target -> FACE_COLORS[FACE] != COLORS_NULL)
        {
            count_colored_faces++;
        }
    }

    return (count_colored_faces == 2);
}

static int is_edge_oriented(const Cubie* EDGE, const Colors COLOR_U, const Colors COLOR_F)
{
    if (!EDGE)
    {
        return 0;
    }
    const Colors COLOR_D = getOppositeColor(COLOR_U);
    const Colors COLOR_B = getOppositeColor(COLOR_F);

    Faces FACE_UD = FACES_NULL;
    Faces FACE_FB = FACES_NULL;
    Faces FACE_RL = FACES_NULL;

    if (cubie_to_face_notation(EDGE, &FACE_UD, &FACE_FB, &FACE_RL))
    {
        return 0;
    }
    Colors COLOR_EDGE_UD = COLORS_NULL;
    Colors COLOR_EDGE_FB = COLORS_NULL;
    Colors COLOR_EDGE_RL = COLORS_NULL;

    if (FACE_UD != FACES_NULL)
    {
        COLOR_EDGE_UD = EDGE -> FACE_COLORS[FACE_UD];
    }
    if (FACE_FB != FACES_NULL)
    {
        COLOR_EDGE_FB = EDGE -> FACE_COLORS[FACE_FB];
    }
    if (FACE_RL != FACES_NULL)
    {
        COLOR_EDGE_RL = EDGE -> FACE_COLORS[FACE_RL];
    }

    if (COLOR_EDGE_UD == COLOR_U || COLOR_EDGE_UD == COLOR_D)
    {
        return 1;
    }
    if (COLOR_EDGE_FB == COLOR_U || COLOR_EDGE_FB == COLOR_D)
    {
        return 1;
    }
    if (COLOR_EDGE_RL == COLOR_U || COLOR_EDGE_RL == COLOR_D)
    {
        return 0;
    }
    if (COLOR_EDGE_RL == COLOR_F || COLOR_EDGE_RL == COLOR_B)
    {
        return 0;
    }

    return 1;
}


int is_edge_orientation_complete(const ZZTracker* tracker)
{
    if (!tracker)
    {
        return 0;
    }

    const Cube* TRACKED_CUBE = &tracker -> CUBE;
    const Colors COLOR_F = get_face_center_color(TRACKED_CUBE, FACE_FRONT);
    const Colors COLOR_U = get_face_center_color(TRACKED_CUBE, FACE_FRONT);

    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            for (int z = -1; z < 2; z++)
            {
                const Cubie* PIECE = get_cubie_at_position(TRACKED_CUBE, x, y, z);
                if (!PIECE)
                {
                    return 0;
                }

                if (!is_cubie_edge(PIECE))
                {
                    continue;
                }
                if (!is_edge_oriented(PIECE, COLOR_U, COLOR_F))
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}
