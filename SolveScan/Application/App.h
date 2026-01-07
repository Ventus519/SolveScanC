//
// Created by Ven519 on 2025/12/31.
//


#ifndef SOLVESCAN_APP_H
#define SOLVESCAN_APP_H

#include "../Cube3/Cube3.h"
#include "../CubeTracker/RouxTracker.h"
#include "gtk/gtk.h"

typedef struct WIDGET_MAP{
    GtkWidget* button_apply_default_moves;
    GtkWidget* button_apply_moves_from_text;
    GtkWidget* button_is_solved;

    GtkWidget* textField_moves_to_apply;

    GtkWidget* WIDGET_GRID;
} WidgetLayout;


void activate (GApplication *g_app);

#endif //SOLVESCAN_APP_H