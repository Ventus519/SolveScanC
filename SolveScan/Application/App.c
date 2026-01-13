//
// Created by Ven519 on 2025/12/31.
//

#include "App.h"
#include "../Cube3/Cube3.h"
#include "../CubeTracker/CubeTracker.h"

static int WINDOW_WIDTH = 800;
static int WINDOW_HEIGHT = 600;


CubeTracker TRACKER;
char* MOVES_DEFAULT = "R";

WidgetLayout layout;

static void on_button_apply_default_moves (gpointer user_data)
{
    if (is_invalid_CubeTracker(&TRACKER))
    {
        g_print("SRM SOURCE FOUND (INVALID CUBE TRACKER :: APPLY DEFAULT MOVES)\n");
        initialize_CubeTracker(&TRACKER, "../data.txt", ROUX);
        return;
    }
    MoveSpec MOVE_TO_APPLY;
    if (parse_move(&MOVES_DEFAULT, &MOVE_TO_APPLY))
    {
        g_print("COULD NOT APPLY MOVES: R\n");
        return;
    }
    track_move_from_spec(&TRACKER, &MOVE_TO_APPLY);
    g_print("Applied moves: %s\n", MOVES_DEFAULT);
    g_print("Current reconstruction: %s\n", MoveStack_to_str(TRACKER.p_MOVES_APPLIED));
}

static void on_button_is_solved (gpointer user_data)
{
    if (is_invalid_CubeTracker(&TRACKER))
    {
        g_print("SRM SOURCE FOUND (INVALID CUBE TRACKER :: IS SOLVED)\n");
        return;
    }
    if (is_solved(&TRACKER.tracker_ROUX -> CUBE))
    {
        g_print("THE CUBE IS SOLVED\n");
    }
    else
    {
        g_print("THE CUBE IS NOT SOLVED\n");
    }
}

static void on_button_apply_moves_from_text (gpointer user_data)
{
    if (is_invalid_CubeTracker(&TRACKER) || !layout.textField_moves_to_apply)
    {
        g_print("SRM SOURCE FOUND (INVALID CUBE TRACKER :: APPLY MOVES TEXT)\n");
        return;
    }

    GtkEntryBuffer* MOVES_TO_APPLY = gtk_entry_get_buffer(GTK_ENTRY (layout.textField_moves_to_apply));
    if (!MOVES_TO_APPLY)
    {
        g_print("SRM SOURCE FOUND (INVALID MOVE STRING :: APPLY_MOVES TEXT)\n");
        return;
    }
    char* moves_to_apply = gtk_entry_buffer_get_text(MOVES_TO_APPLY);
    MoveSpec MOVE_TO_APPLY;
    if (parse_move(&moves_to_apply, &MOVE_TO_APPLY))
    {
        g_print("COULD NOT PARSE MOVE (PROBABLY TOO LONG)");
        return;
    }
    track_move_from_spec(&TRACKER, &MOVE_TO_APPLY);


    g_print("APPLIED MOVES: %s\n", moves_to_apply);
    g_print("Current reconstruction: %s\n", MoveStack_to_str(TRACKER.p_MOVES_APPLIED));




}


static void link_button_handlers(WidgetLayout* p_layout)
{
    g_signal_connect(p_layout -> button_apply_default_moves, "clicked", G_CALLBACK (on_button_apply_default_moves), NULL);
    g_signal_connect(p_layout -> button_is_solved, "clicked", G_CALLBACK (on_button_is_solved), NULL);
    g_signal_connect(p_layout -> button_apply_moves_from_text, "clicked", G_CALLBACK (on_button_apply_moves_from_text), NULL);
}

static void create_button_layout_controls(GtkWindow* APP_WINDOW, WidgetLayout* p_layout)
{
    p_layout -> button_apply_default_moves = gtk_button_new_with_label("APPLY: R U R' U'");
    p_layout -> button_is_solved = gtk_button_new_with_label("IS SOLVED?");
    p_layout -> button_apply_moves_from_text = gtk_button_new_with_label("APPLY MOVES");

    p_layout -> textField_moves_to_apply = gtk_entry_new();

    p_layout -> WIDGET_GRID = gtk_grid_new();

    gtk_window_set_child (APP_WINDOW, p_layout -> WIDGET_GRID);

    gtk_grid_attach(GTK_GRID(p_layout -> WIDGET_GRID), p_layout -> button_is_solved, 3, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(p_layout -> WIDGET_GRID), p_layout -> textField_moves_to_apply, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(p_layout -> WIDGET_GRID), p_layout -> button_apply_moves_from_text, 3, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(p_layout -> WIDGET_GRID), p_layout -> button_apply_default_moves, 0, 0, 1, 1);
}

void activate (GApplication *g_app)
{
    initialize_CubeTracker(&TRACKER, "../data.txt", ROUX);


    GtkApplication* app = GTK_APPLICATION (g_app);

    GtkWindow* APP_WINDOW = gtk_application_get_active_window(app);

    if (!APP_WINDOW)
    {
        APP_WINDOW = GTK_WINDOW(gtk_application_window_new (app));
        gtk_window_set_default_size(APP_WINDOW, WINDOW_WIDTH, WINDOW_HEIGHT);
        gtk_window_set_title (APP_WINDOW, "TEST");
        gtk_window_set_application(APP_WINDOW, app);
    }

    create_button_layout_controls(APP_WINDOW, &layout);
    link_button_handlers(&layout);


    gtk_window_present (APP_WINDOW);

}