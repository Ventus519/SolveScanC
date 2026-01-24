//
// Created by Ven519 on 2026/01/22.
//

#include "BeginnersTracker.h"

#include <stdlib.h>

#include "CFOPTracker.h"

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

int is_first_layer_corners_solved(const BeginnersTracker* tracker)
{
    if (!tracker)
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
    const Cubie* CORNER_DBR = get_cubie_at_position(&tracker -> CUBE, 1, -1, -1);

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
    if (CORNER_DBL -> FACE_COLORS[FACE_LEFT] != CENTER_L_COLOR)
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

int is_second_layer_edges_solved(const BeginnersTracker* tracker)
{
    if (!tracker)
    {
        return 0;
    }
    const Colors CENTER_COLOR_F = get_face_center_color(&tracker -> CUBE, FACE_FRONT);
    const Colors CENTER_COLOR_R = get_face_center_color(&tracker -> CUBE, FACE_RIGHT);
    const Colors CENTER_COLOR_B = get_face_center_color(&tracker -> CUBE, FACE_BACK);
    const Colors CENTER_COLOR_L = get_face_center_color(&tracker -> CUBE, FACE_LEFT);

    const Cubie* EDGE_EFR = get_cubie_at_position(&tracker -> CUBE, 1, 0, 1);
    const Cubie* EDGE_EBR = get_cubie_at_position(&tracker -> CUBE, 1, 0, -1);
    const Cubie* EDGE_EBL = get_cubie_at_position(&tracker -> CUBE, -1, 0, -1);
    const Cubie* EDGE_EFL = get_cubie_at_position(&tracker -> CUBE, -1, 0, 1);

    if (EDGE_EFR -> FACE_COLORS[FACE_FRONT] != CENTER_COLOR_F)
    {
        return 0;
    }
    if (EDGE_EFR -> FACE_COLORS[FACE_RIGHT] != CENTER_COLOR_R)
    {
        return 0;
    }

    if (EDGE_EBR -> FACE_COLORS[FACE_BACK] != CENTER_COLOR_B)
    {
        return 0;
    }
    if (EDGE_EBR -> FACE_COLORS[FACE_RIGHT] != CENTER_COLOR_R)
    {
        return 0;
    }

    if (EDGE_EBL -> FACE_COLORS[FACE_BACK] != CENTER_COLOR_B)
    {
        return 0;
    }
    if (EDGE_EBL -> FACE_COLORS[FACE_LEFT] != CENTER_COLOR_L)
    {
        return 0;
    }

    if (EDGE_EFL -> FACE_COLORS[FACE_FRONT] != CENTER_COLOR_F)
    {
        return 0;
    }
    if (EDGE_EFL -> FACE_COLORS[FACE_LEFT] != CENTER_COLOR_L)
    {
        return 0;
    }


    return 1;
}

int is_opposite_cross_oriented(const BeginnersTracker* tracker)
{
    if (!tracker)
    {
        return 0;
    }

    const Colors OPPOSITE_CROSS_COLOR = get_face_center_color(&tracker -> CUBE, FACE_UP);

    const Cubie* OPPOSITE_CROSS_EDGE_F = get_cubie_at_position(&tracker -> CUBE, 0, 1, 1);
    const Cubie* OPPOSITE_CROSS_EDGE_R = get_cubie_at_position(&tracker -> CUBE, 1, 1, 0);
    const Cubie* OPPOSITE_CROSS_EDGE_B = get_cubie_at_position(&tracker -> CUBE, 0, 1, -1);
    const Cubie* OPPOSITE_CROSS_EDGE_L = get_cubie_at_position(&tracker -> CUBE, -1, 1, 0);

    if (OPPOSITE_CROSS_EDGE_F -> FACE_COLORS[FACE_UP] != OPPOSITE_CROSS_COLOR)
    {
        return 0;
    }
    if (OPPOSITE_CROSS_EDGE_R -> FACE_COLORS[FACE_UP] != OPPOSITE_CROSS_COLOR)
    {
        return 0;
    }
    if (OPPOSITE_CROSS_EDGE_B -> FACE_COLORS[FACE_UP] != OPPOSITE_CROSS_COLOR)
    {
        return 0;
    }
    if (OPPOSITE_CROSS_EDGE_L -> FACE_COLORS[FACE_UP] != OPPOSITE_CROSS_COLOR)
    {
        return 0;
    }

    return 1;
}

int is_opposite_cross_solved(const BeginnersTracker* tracker)
{
    if (!tracker)
    {
        return 0;
    }

    const Cubie* OPPOSITE_CROSS_EDGE_F = get_cubie_at_position(&tracker -> CUBE, 0, 1, 1);
    const Cubie* OPPOSITE_CROSS_EDGE_R = get_cubie_at_position(&tracker -> CUBE, 1, 1, 0);
    const Cubie* OPPOSITE_CROSS_EDGE_B = get_cubie_at_position(&tracker -> CUBE, 0, 1, -1);
    const Cubie* OPPOSITE_CROSS_EDGE_L = get_cubie_at_position(&tracker -> CUBE, -1, 1, 0);

    const Colors COLOR_SCHEME[4] = {
        get_face_center_color(&tracker -> CUBE, FACE_FRONT),
        get_face_center_color(&tracker -> CUBE, FACE_RIGHT),
        get_face_center_color(&tracker -> CUBE, FACE_BACK),
        get_face_center_color(&tracker -> CUBE, FACE_LEFT)
    };

    for (int i = 0; i < 4; i++)
    {

        if (OPPOSITE_CROSS_EDGE_F -> FACE_COLORS[FACE_FRONT] != COLOR_SCHEME[i % 4])
        {
            continue;
        }
        if (OPPOSITE_CROSS_EDGE_R -> FACE_COLORS[FACE_RIGHT] != COLOR_SCHEME[(i+1) % 4])
        {
            return 0;
        }
        if (OPPOSITE_CROSS_EDGE_B -> FACE_COLORS[FACE_BACK] != COLOR_SCHEME[(i+2) % 4])
        {
            return 0;
        }
        if (OPPOSITE_CROSS_EDGE_L -> FACE_COLORS[FACE_LEFT] != COLOR_SCHEME[(i+3) % 4])
        {
            return 0;
        }
        return 1;

    }



    return 1;
}

int is_last_layer_corners_permuted(const BeginnersTracker* tracker)
{
    if (!tracker)
    {
        return 0;
    }

    const Cubie* OPPOSITE_CROSS_EDGE_F = get_cubie_at_position(&tracker -> CUBE, 0, 1, 1);
    const Cubie* OPPOSITE_CROSS_EDGE_R = get_cubie_at_position(&tracker -> CUBE, 1, 1, 0);
    const Cubie* OPPOSITE_CROSS_EDGE_B = get_cubie_at_position(&tracker -> CUBE, 0, 1, -1);
    const Cubie* OPPOSITE_CROSS_EDGE_L = get_cubie_at_position(&tracker -> CUBE, -1, 1, 0);

    const Colors COLOR_OPPOSITE_CROSS_F = OPPOSITE_CROSS_EDGE_F -> FACE_COLORS[FACE_FRONT];
    const Colors COLOR_CENTER_U = get_face_center_color(&tracker -> CUBE, FACE_UP);
    const Colors COLOR_OPPOSITE_CROSS_R = OPPOSITE_CROSS_EDGE_R -> FACE_COLORS[FACE_RIGHT];
    const Colors COLOR_OPPOSITE_CROSS_B = OPPOSITE_CROSS_EDGE_B -> FACE_COLORS[FACE_BACK];
    const Colors COLOR_OPPOSITE_CROSS_L = OPPOSITE_CROSS_EDGE_L -> FACE_COLORS[FACE_LEFT];

    const Cubie* CORNER_UFR = get_cubie_at_position(&tracker -> CUBE, 1, 1, 1);
    const Cubie* CORNER_UBR = get_cubie_at_position(&tracker -> CUBE, 1, 1, -1);
    const Cubie* CORNER_UBL = get_cubie_at_position(&tracker -> CUBE, -1, 1, -1);
    const Cubie* CORNER_UFL = get_cubie_at_position(&tracker -> CUBE, -1, 1, 1);

    if (!isColorOnCubie(CORNER_UFR, COLOR_CENTER_U) || !isColorOnCubie(CORNER_UFR, COLOR_OPPOSITE_CROSS_F) ||
        !isColorOnCubie(CORNER_UFR, COLOR_OPPOSITE_CROSS_R))
    {
        return 0;
    }

    if (!isColorOnCubie(CORNER_UBR, COLOR_CENTER_U) || !isColorOnCubie(CORNER_UBR, COLOR_OPPOSITE_CROSS_B) ||
        !isColorOnCubie(CORNER_UBR, COLOR_OPPOSITE_CROSS_R))
    {
        return 0;
    }

    if (!isColorOnCubie(CORNER_UBL, COLOR_CENTER_U) || !isColorOnCubie(CORNER_UBL, COLOR_OPPOSITE_CROSS_B) ||
        !isColorOnCubie(CORNER_UBL, COLOR_OPPOSITE_CROSS_L))
    {
        return 0;
    }

    if (!isColorOnCubie(CORNER_UFL, COLOR_CENTER_U) || !isColorOnCubie(CORNER_UFL, COLOR_OPPOSITE_CROSS_F) ||
        !isColorOnCubie(CORNER_UFL, COLOR_OPPOSITE_CROSS_L))
    {
        return 0;
    }

    return 1;
}

int update_current_step_BEGINNERS(BeginnersTracker* tracker, const int continue_scramble, const int continue_inspect)
{
    if (!tracker)
    {
        return 1;
    }

    const BeginnersMilestones CURRENT_STEP = tracker -> STEP;
    if (CURRENT_STEP == BEGINNERS_MILESTONE_NULL)
    {
        return 1;
    }

    if (continue_scramble)
    {
        tracker -> STEP = BEGINNERS_SCRAMBLE;
        return 0;
    }
    if (continue_inspect)
    {
        tracker -> STEP = BEGINNERS_INSPECT;
        return 0;
    }
    tracker -> STEP = BEGINNERS_CROSS;
    if (CURRENT_STEP == BEGINNERS_FIRST_LAYER || is_cross_complete_BEGINNERS(tracker))
    {
        tracker -> STEP = BEGINNERS_FIRST_LAYER;
    }

    if (CURRENT_STEP == BEGINNERS_SECOND_LAYER ||
        (tracker -> STEP == BEGINNERS_FIRST_LAYER) && is_first_layer_corners_solved(tracker))
    {
        tracker -> STEP = BEGINNERS_SECOND_LAYER;
    }

    if (CURRENT_STEP == BEGINNERS_ORIENT_OPPOSITE_CROSS ||
        (tracker -> STEP == BEGINNERS_SECOND_LAYER) && is_second_layer_edges_solved(tracker))
    {
        tracker -> STEP = BEGINNERS_ORIENT_OPPOSITE_CROSS;
    }

    if (CURRENT_STEP == BEGINNERS_PERMUTE_OPPOSITE_CROSS ||
        (tracker -> STEP == BEGINNERS_ORIENT_OPPOSITE_CROSS && is_opposite_cross_oriented(tracker)))
    {
        tracker -> STEP = BEGINNERS_PERMUTE_OPPOSITE_CROSS;
    }

    if (CURRENT_STEP == BEGINNERS_PERMUTE_OPPOSITE_CORNERS ||
        (tracker -> STEP == BEGINNERS_PERMUTE_OPPOSITE_CROSS && is_opposite_cross_solved(tracker)))
    {
        tracker -> STEP = BEGINNERS_PERMUTE_OPPOSITE_CORNERS;
    }

    if (CURRENT_STEP == BEGINNERS_ORIENT_OPPOSITE_CORNERS ||
        (tracker -> STEP == BEGINNERS_PERMUTE_OPPOSITE_CORNERS) && is_last_layer_corners_permuted(tracker))
    {
        tracker -> STEP = BEGINNERS_ORIENT_OPPOSITE_CORNERS;
    }

    if (is_solved(&tracker -> CUBE))
    {
        tracker -> STEP = BEGINNERS_SOLVED;
    }


    return 0;
}