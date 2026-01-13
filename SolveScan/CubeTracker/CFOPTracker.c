//
// Created by Ven519 on 2026/01/12.
//

#include "CFOPTracker.h"

#include "RouxTracker.h"

int initialize_CFOPTracker(CFOPTracker* tracker)
{
    if (!tracker)
    {
        return 1;
    }
    if (initialize_cube(&tracker -> CUBE))
    {
        return 1;
    }
    tracker -> STEP = CFOP_SCRAMBLE;
    return 0;
}

int is_cross_complete(const CFOPTracker* tracker)
{
    if (!tracker)
    {
        return 0;
    }
    const Colors CROSS_CENTER_COLOR = get_face_center_color(&tracker -> CUBE, FACE_DOWN);

    const Cubie* CROSS_EDGE_F = get_cubie_at_position(&tracker -> CUBE, 0, -1, 1);
    const Cubie* CROSS_EDGE_R = get_cubie_at_position(&tracker -> CUBE, 1, -1, 0);
    const Cubie* CROSS_EDGE_B = get_cubie_at_position(&tracker -> CUBE, 0, -1, -1);
    const Cubie* CROSS_EDGE_L = get_cubie_at_position(&tracker -> CUBE, -1, -1, 0);

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

int count_complete_f2l_pairs(const CFOPTracker* tracker)
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

int is_f2l_pair_complete(const CFOPTracker* tracker, const Faces FACE_FRONT_OR_FACE_BACK, const Faces FACE_RIGHT_OR_FACE_LEFT)
{
    if (!tracker)
    {
        return 0;
    }
    int z_offset = 0;
    int x_offset = 0;
    switch (FACE_FRONT_OR_FACE_BACK)
    {
        case FACE_FRONT: z_offset = 1; break;
        case FACE_BACK: z_offset = -1; break;
        default: return 0;
    }
    switch (FACE_RIGHT_OR_FACE_LEFT)
    {
        case FACE_RIGHT: x_offset = 1; break;
        case FACE_LEFT: x_offset = -1; break;
        default: return 0;
    }

    const Cubie* F2L_CORNER = get_cubie_at_position(&tracker -> CUBE, x_offset, -1, z_offset);
    const Cubie* NEARBY_CROSS_EDGE_RL = get_cubie_at_position(&tracker -> CUBE, x_offset, -1, 0);
    const Cubie* NEARBY_CROSS_EDGE_FB = get_cubie_at_position(&tracker -> CUBE, 0, -1, z_offset);

    const Cubie* F2L_EDGE = get_cubie_at_position(&tracker -> CUBE, x_offset, 0, z_offset);

    if (F2L_CORNER -> FACE_COLORS[FACE_DOWN] != get_face_center_color(&tracker -> CUBE, FACE_DOWN))
    {
        return 0;
    }
    if (F2L_EDGE -> FACE_COLORS[FACE_FRONT_OR_FACE_BACK] != get_face_center_color(&tracker -> CUBE, FACE_FRONT_OR_FACE_BACK))
    {
        return 0;
    }
    if (F2L_EDGE -> FACE_COLORS[FACE_RIGHT_OR_FACE_LEFT] != get_face_center_color(&tracker -> CUBE, FACE_RIGHT_OR_FACE_LEFT))
    {
        return 0;
    }
    if (NEARBY_CROSS_EDGE_FB -> FACE_COLORS[FACE_FRONT_OR_FACE_BACK] != F2L_CORNER -> FACE_COLORS[FACE_FRONT_OR_FACE_BACK])
    {
        return 0;
    }
    if (NEARBY_CROSS_EDGE_RL -> FACE_COLORS[FACE_RIGHT_OR_FACE_LEFT] != F2L_CORNER -> FACE_COLORS[FACE_RIGHT_OR_FACE_LEFT])
    {
        return 0;
    }

    return 1;
}
