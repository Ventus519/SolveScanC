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
    if ((COLOR_EDGE_FB == COLOR_U || COLOR_EDGE_FB == COLOR_D) && COLOR_EDGE_UD == COLORS_NULL)
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

int is_cross_complete_ZZ(const ZZTracker* tracker)
{
    if (!tracker)
    {
        return 0;
    }

    const Colors CROSS_CENTER_COLOR = get_face_center_color(&tracker -> CUBE, FACE_DOWN);

    const Cubie* CROSS_EDGE_F = get_cubie_from_faces(&tracker -> CUBE, FACE_DOWN, FACE_FRONT, FACES_NULL);
    const Cubie* CROSS_EDGE_R = get_cubie_from_faces(&tracker -> CUBE, FACE_DOWN, FACES_NULL, FACE_RIGHT);
    const Cubie* CROSS_EDGE_B = get_cubie_from_faces(&tracker -> CUBE, FACE_DOWN, FACE_BACK, FACES_NULL);
    const Cubie* CROSS_EDGE_L = get_cubie_from_faces(&tracker -> CUBE, FACE_DOWN, FACES_NULL, FACE_LEFT);

    if (CROSS_EDGE_F -> FACE_COLORS[FACE_FRONT] != get_face_center_color(&tracker -> CUBE, FACE_FRONT))
    {
        return 0;
    }
    if (CROSS_EDGE_R -> FACE_COLORS[FACE_RIGHT] != get_face_center_color(&tracker -> CUBE, FACE_RIGHT))
    {
        return 0;
    }
    if (CROSS_EDGE_B -> FACE_COLORS[FACE_BACK] != get_face_center_color(&tracker -> CUBE, FACE_BACK))
    {
        return 0;
    }
    if (CROSS_EDGE_L -> FACE_COLORS[FACE_LEFT] != get_face_center_color(&tracker -> CUBE, FACE_LEFT))
    {
        return 0;
    }

    if (CROSS_EDGE_F -> FACE_COLORS[FACE_DOWN] != CROSS_CENTER_COLOR)
    {
        return 0;
    }
    if (CROSS_EDGE_R -> FACE_COLORS[FACE_DOWN] != CROSS_CENTER_COLOR)
    {
        return 0;
    }
    if (CROSS_EDGE_B -> FACE_COLORS[FACE_DOWN] != CROSS_CENTER_COLOR)
    {
        return 0;
    }
    if (CROSS_EDGE_L -> FACE_COLORS[FACE_DOWN] != CROSS_CENTER_COLOR)
    {
        return 0;
    }

    return 1;
}

int count_complete_f2l_pairs_ZZ(const ZZTracker* tracker)
{
    if (!tracker)
    {
        return 0;
    }
    int count = 0;
    for (int i = 0; i < 4; i++)
    {
        count += (1 & tracker -> f2l_pairs_completed >> i);
    }
    return count;
}

int is_f2l_pair_complete_ZZ(const ZZTracker* tracker, const Faces FACE_FB, const Faces FACE_RL)
{
    if (!tracker)
    {
        return 0;
    }

    const Cubie* F2L_CORNER = get_cubie_from_faces(&tracker -> CUBE, FACE_DOWN, FACE_FB, FACE_RL);
    const Cubie* NEARBY_CROSS_EDGE_RL = get_cubie_from_faces(&tracker -> CUBE, FACE_DOWN, FACES_NULL, FACE_RL);
    const Cubie* NEARBY_CROSS_EDGE_FB = get_cubie_from_faces(&tracker -> CUBE, FACE_DOWN, FACE_FB, FACES_NULL);

    const Cubie* F2L_EDGE = get_cubie_from_faces(&tracker -> CUBE, FACES_NULL, FACE_FB, FACE_RL);

    if (!F2L_CORNER || !NEARBY_CROSS_EDGE_FB || !NEARBY_CROSS_EDGE_RL || !F2L_EDGE)
    {
        return 0;
    }

    if (F2L_CORNER -> FACE_COLORS[FACE_DOWN] != get_face_center_color(&tracker -> CUBE, FACE_DOWN))
    {
        return 0;
    }
    if (F2L_EDGE -> FACE_COLORS[FACE_FB] != get_face_center_color(&tracker -> CUBE, FACE_FB))
    {
        return 0;
    }
    if (F2L_EDGE -> FACE_COLORS[FACE_RL] != get_face_center_color(&tracker -> CUBE, FACE_RL))
    {
        return 0;
    }
    if (NEARBY_CROSS_EDGE_FB -> FACE_COLORS[FACE_FB] != F2L_CORNER -> FACE_COLORS[FACE_FB])
    {
        return 0;
    }
    if (NEARBY_CROSS_EDGE_RL -> FACE_COLORS[FACE_RL] != F2L_CORNER -> FACE_COLORS[FACE_RL])
    {
        return 0;
    }

    return 1;
}

int update_completed_f2l_pairs(ZZTracker* tracker)
{
    if (!tracker)
    {
        return 1;
    }
    tracker -> f2l_pairs_completed = 0;
    if (is_f2l_pair_complete_ZZ(tracker, FACE_FRONT, FACE_LEFT))
    {
        tracker -> f2l_pairs_completed |= 0x01;
    }
    if (is_f2l_pair_complete_ZZ(tracker, FACE_FRONT, FACE_RIGHT))
    {
        tracker -> f2l_pairs_completed |= 0x02;
    }
    if (is_f2l_pair_complete_ZZ(tracker, FACE_BACK, FACE_LEFT))
    {
        tracker -> f2l_pairs_completed |= 0x04;
    }
    if (is_f2l_pair_complete_ZZ(tracker, FACE_BACK, FACE_RIGHT))
    {
        tracker -> f2l_pairs_completed |= 0x08;
    }

    return 0;
}

int is_ocll_complete(const ZZTracker* tracker)
{
    if (!tracker)
    {
        return 0;
    }
    const Colors COLOR_U = get_face_center_color(&tracker -> CUBE, FACE_UP);

    const Cubie* CORNER_UFR = get_cubie_from_faces(&tracker -> CUBE, FACE_UP, FACE_FRONT, FACE_RIGHT);
    const Cubie* CORNER_UBR = get_cubie_from_faces(&tracker -> CUBE, FACE_UP, FACE_BACK, FACE_RIGHT);
    const Cubie* CORNER_UBL = get_cubie_from_faces(&tracker -> CUBE, FACE_UP, FACE_BACK, FACE_LEFT);
    const Cubie* CORNER_UFL = get_cubie_from_faces(&tracker -> CUBE, FACE_UP, FACE_FRONT, FACE_LEFT);

    if (CORNER_UFR -> FACE_COLORS[FACE_UP] != COLOR_U || CORNER_UFL -> FACE_COLORS[FACE_UP] != COLOR_U)
    {
        return 0;
    }
    if (CORNER_UBR -> FACE_COLORS[FACE_UP] != COLOR_U || CORNER_UBL -> FACE_COLORS[FACE_UP] != COLOR_U)
    {
        return 0;
    }

    return 1;
}

