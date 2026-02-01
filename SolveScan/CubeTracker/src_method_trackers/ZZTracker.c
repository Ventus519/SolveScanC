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

int is_edge_orientation_complete(const ZZTracker* tracker)
{
    if (!tracker)
    {
        return 1;
    }

    const Colors COLOR_U = get_face_center_color(&tracker -> CUBE, FACE_UP);
    const Colors COLOR_D = getOppositeColor(COLOR_U);
    const Colors COLOR_F = get_face_center_color(&tracker -> CUBE, FACE_FRONT);
    const Colors COLOR_B = getOppositeColor(COLOR_F);

    //implementation still needs to be done

    return 0;
}