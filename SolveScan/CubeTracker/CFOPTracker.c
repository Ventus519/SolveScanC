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

void free_CFOPTracker(CFOPTracker* tracker)
{
    if (!tracker)
    {
        return;
    }
    free(tracker);
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

int update_completed_f2l_pairs(CFOPTracker* tracker)
{
    if (!tracker)
    {
        return 1;
    }

    if (is_f2l_pair_complete(tracker, FACE_FRONT, FACE_LEFT))
    {
        tracker -> f2l_pairs_completed |= 0x01;
    }
    if (is_f2l_pair_complete(tracker, FACE_FRONT, FACE_RIGHT))
    {
        tracker -> f2l_pairs_completed |= 0x02;
    }
    if (is_f2l_pair_complete(tracker, FACE_BACK, FACE_LEFT))
    {
        tracker -> f2l_pairs_completed |= 0x04;
    }
    if (is_f2l_pair_complete(tracker, FACE_BACK, FACE_RIGHT))
    {
        tracker -> f2l_pairs_completed |= 0x08;
    }

    return 0;
}

int is_oll_complete(const CFOPTracker* tracker)
{
    if (!tracker)
    {
        return 0;
    }
    const Colors U_FACE_CENTER_COLOR = get_face_center_color(&tracker -> CUBE, FACE_UP);
    for (int x = -1; x < 2; x++)
    {
        for (int z = -1; z < 2; z++)
        {
            const Cubie* TEST = get_cubie_at_position(&tracker -> CUBE, x, 1, z);
            if (TEST -> FACE_COLORS[FACE_UP] != U_FACE_CENTER_COLOR)
            {
                return 0;
            }
        }
    }
    return 1;
}

int update_current_step_CFOP(CFOPTracker* tracker, const int continue_scramble, const int continue_inspect)
{
    if (!tracker)
    {
        return 1;
    }
    if (tracker -> STEP == CFOP_MILESTONE_NULL)
    {
        return 1;
    }
    if (continue_scramble)
    {
        tracker -> STEP = CFOP_SCRAMBLE;
        return 0;
    }
    if (continue_inspect)
    {
        tracker -> STEP = CFOP_INSPECT;
        return 0;
    }
    tracker -> STEP = CFOP_CROSS;
    if (is_cross_complete(tracker))
    {
        tracker -> STEP = CFOP_F2L_1;
    }
    if (update_completed_f2l_pairs(tracker))
    {
        return 1;
    }
    const int completed_pairs = count_complete_f2l_pairs(tracker);
    switch (completed_pairs)
    {
        case 0: tracker -> STEP = CFOP_F2L_1; break;
        case 1: tracker -> STEP = CFOP_F2L_2; break;
        case 2: tracker -> STEP = CFOP_F2L_3; break;
        case 3: tracker -> STEP = CFOP_F2L; break;
        case 4: tracker -> STEP = CFOP_OLL; break;
        default: return 1;
    }
    if (tracker -> STEP == CFOP_OLL && is_oll_complete(tracker))
    {
        tracker -> STEP = CFOP_PLL;
    }
    if (is_solved(&tracker -> CUBE))
    {
        tracker -> STEP = CFOP_SOLVED;
    }
    return 0;
}