//
// Created by Ven519 on 2026/01/22.
//

#include "BeginnersTracker.h"

#include <stdlib.h>

int initialize_BeginnersTracker(BeginnersTracker* tracker)
{
    if (!tracker)
    {
        return 1;
    }
    if (initialize_cube(&tracker -> CUBE))
    {
        return 1;
    }
    tracker -> STEP = BEGINNERS_SCRAMBLE;

    return 0;
}

void free_BeginnersTracker(BeginnersTracker* tracker)
{
    if (!tracker)
    {
        return;
    }
    free(tracker);
}

int is_cross_complete_BEGINNERS(const BeginnersTracker* tracker)
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

    if (CROSS_EDGE_F -> FACE_COLORS[FACE_DOWN] != CROSS_CENTER_COLOR)
    {
        return 0;
    }
    if (CROSS_EDGE_F -> FACE_COLORS[FACE_FRONT] != get_face_center_color(&tracker -> CUBE, FACE_FRONT))
    {
        return 0;
    }
    if (CROSS_EDGE_R -> FACE_COLORS[FACE_DOWN] != CROSS_CENTER_COLOR)
    {
        return 0;
    }
    if (CROSS_EDGE_R -> FACE_COLORS[FACE_RIGHT] != get_face_center_color(&tracker -> CUBE, FACE_RIGHT))
    {
        return 0;
    }

    if (CROSS_EDGE_B -> FACE_COLORS[FACE_DOWN] != CROSS_CENTER_COLOR)
    {
        return 0;
    }
    if (CROSS_EDGE_B -> FACE_COLORS[FACE_BACK] != get_face_center_color(&tracker -> CUBE, FACE_BACK))
    {
        return 0;
    }
    if (CROSS_EDGE_L -> FACE_COLORS[FACE_DOWN] != CROSS_CENTER_COLOR)
    {
        return 0;
    }
    if (CROSS_EDGE_L -> FACE_COLORS[FACE_LEFT] != get_face_center_color(&tracker -> CUBE, FACE_LEFT))
    {
        return 0;
    }

    return 1;
}

int is_first_layer_complete(const BeginnersTracker* tracker)
{
    if (!tracker)
    {
        return 0;
    }
    if (!is_cross_complete_BEGINNERS(tracker))
    {
        return 0;
    }

    const Colors CROSS_COLOR = get_face_center_color(&tracker -> CUBE, FACE_DOWN);
    const Colors CENTER_F_COLOR = get_face_center_color(&tracker -> CUBE, FACE_FRONT);
    const Colors CENTER_R_COLOR = get_face_center_color(&tracker -> CUBE, FACE_RIGHT);
    const Colors CENTER_B_COLOR = get_face_center_color(&tracker -> CUBE, FACE_BACK);
    const Colors CENTER_L_COLOR = get_face_center_color(&tracker -> CUBE, FACE_LEFT);

    const Cubie* CORNER_DFR = get_cubie_at_position(&tracker -> CUBE, 1, -1, 1);
    const Cubie* CORNER_DFL = get_cubie_at_position(&tracker -> CUBE, -1, -1, 1);
    const Cubie* CORNER_DBL = get_cubie_at_position(&tracker -> CUBE, -1, -1, -1);
    const Cubie* CORNER_DBR = get_cubie_at_position(&tracker -> CUBE, -1, -1, -1);

    if (CORNER_DFR -> FACE_COLORS[FACE_DOWN] != CROSS_COLOR)
    {
        return 0;
    }
    if (CORNER_DFR -> FACE_COLORS[FACE_FRONT] != CENTER_F_COLOR)
    {
        return 0;
    }
    if (CORNER_DFR -> FACE_COLORS[FACE_RIGHT] != CENTER_R_COLOR)
    {
        return 0;
    }

    if (CORNER_DBR -> FACE_COLORS[FACE_DOWN] != CROSS_COLOR)
    {
        return 0;
    }
    if (CORNER_DBR -> FACE_COLORS[FACE_BACK] != CENTER_B_COLOR)
    {
        return 0;
    }
    if (CORNER_DBR -> FACE_COLORS[FACE_RIGHT] != CENTER_R_COLOR)
    {
        return 0;
    }

    if (CORNER_DBL -> FACE_COLORS[FACE_DOWN] != CROSS_COLOR)
    {
        return 0;
    }
    if (CORNER_DBL -> FACE_COLORS[FACE_BACK] != CENTER_B_COLOR)
    {
        return 0;
    }
    if (CORNER_DBL -> FACE_COLORS[FACE_RIGHT] != CENTER_L_COLOR)
    {
        return 0;
    }

    if (CORNER_DFL -> FACE_COLORS[FACE_DOWN] != CROSS_COLOR)
    {
        return 0;
    }
    if (CORNER_DFL -> FACE_COLORS[FACE_FRONT] != CENTER_F_COLOR)
    {
        return 0;
    }
    if (CORNER_DFL -> FACE_COLORS[FACE_LEFT] != CENTER_L_COLOR)
    {
        return 0;
    }


    return 1;
}

