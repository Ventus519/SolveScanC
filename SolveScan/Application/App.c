//
// Created by Ven519 on 2025/12/31.
//

#include "App.h"

#include <stdnoreturn.h>

#include "../Cube3/Cube3.h"
#include "../CubeTracker/CubeTracker.h"

static int WINDOW_WIDTH = 800;
static int WINDOW_HEIGHT = 600;


EnabledTrackers DEFAULT_ENABLED = ROUX;

CubeTracker TRACKER;
char* MOVES_DEFAULT = "R";

WidgetLayout layout;


static void on_button_apply_scramble_from_text (gpointer user_data)
{
    if (is_invalid_CubeTracker(&TRACKER) || !layout.textField_scramble)
    {
        g_print("SRM SOURCE FOUND (INVALID CUBE TRACKER :: APPLY SCRAMBLE TEXT)\n");
        return;
    }
    GtkEntryBuffer* SCRAMBLE_TO_APPLY = gtk_entry_get_buffer(GTK_ENTRY(layout.textField_scramble));
    if (!SCRAMBLE_TO_APPLY)
    {
        g_print("NO SCRAMBLE");
    }
    char* scramble_moves = gtk_entry_buffer_get_text(SCRAMBLE_TO_APPLY);
    MoveStack* SCRAMBLE_SPEC = move_str_to_MoveStack(scramble_moves);
    if (!SCRAMBLE_SPEC)
    {
        return;
    }
    if (track_scramble(&TRACKER, SCRAMBLE_SPEC))
    {
        g_print("COULD NOT TRACK SCRAMBLE");
        return;
    }
    free_MoveStack(SCRAMBLE_SPEC);
    g_print("TRACKED SCRAMBLE\n");
    g_print("Current reconstruction: %s\n", TRACKER.reconstruction);


    gtk_widget_set_visible (GTK_WIDGET (layout.button_apply_scramble_from_text), FALSE);
    gtk_widget_set_visible (GTK_WIDGET (layout.textField_scramble), FALSE);
}

static void on_button_get_current_step (gpointer user_data)
{
    if (is_invalid_CubeTracker(&TRACKER))
    {
        g_print("SRM SOURCE FOUND (INVALID CUBE TRACKER :: APPLY DEFAULT MOVES)\n");
        initialize_CubeTracker(&TRACKER, "../data.txt", ROUX);
        return;
    }
    switch (TRACKER.ENABLED)
    {
        case BEGINNERS: return;
        case CFOP:
        {
            const CFOPMilestones CURRENT_STEP = get_current_CFOP_step(&TRACKER);
            g_print("CURRENT STEP: ");
            switch (CURRENT_STEP)
            {
                case CFOP_SCRAMBLE: g_print("(CFOP) SCRAMBLE\n"); break;
                case CFOP_INSPECT: g_print("(CFOP) INSPECT\n"); break;
                case CFOP_CROSS: g_print("(CFOP) CROSS\n"); break;
                case CFOP_F2L_1: g_print("(CFOP F2L) Pair 1\n"); break;
                case CFOP_F2L_2: g_print("(CFOP F2L) Pair 2\n"); break;
                case CFOP_F2L_3: g_print("(CFOP F2L) Pair 3\n"); break;
                case CFOP_F2L: g_print("(CFOP F2L) Pair 4\n"); break;
                case CFOP_OLL: g_print("(CFOP) OLL\n"); break;
                case CFOP_PLL: g_print("(CFOP) PLL\n"); break;
                case CFOP_SOLVED: g_print("(CFOP) SOLVED\n"); break;
                case CFOP_MILESTONE_NULL: g_print("INVALID CFOP STEP\n"); break;
            }
            break;
        }
        case ROUX:
        {
            const RouxMilestones CURRENT_STEP = get_current_ROUX_step(&TRACKER);
            g_print("CURRENT STEP: ");
            switch (CURRENT_STEP)
            {
                case ROUX_SCRAMBLE: g_print("(ROUX) SCRAMBLE\n"); break;
                case ROUX_INSPECT: g_print("(ROUX) INSPECT\n"); break;
                case ROUX_FIRST_BLOCK: g_print("(ROUX) FB\n"); break;
                case ROUX_SECOND_BLOCK: g_print("(ROUX) SB\n"); break;
                case ROUX_LAST_LAYER_CORNERS: g_print("(ROUX) CMLL\n"); break;
                case ROUX_LAST_SIX_EDGES: g_print("(ROUX) L6E\n"); break;
                case ROUX_SOLVED: g_print("(ROUX) SOLVED\n"); break;
                case ROUX_MILESTONE_NULL: g_print("INVALID ROUX STEP\n"); break;
            }
            break;
        }
        case ZZ: return;
    }
}

static void on_button_is_solved (gpointer user_data)
{
    if (is_invalid_CubeTracker(&TRACKER))
    {
        g_print("SRM SOURCE FOUND (INVALID CUBE TRACKER :: IS SOLVED)\n");
        return;
    }
    if (is_subtracker_cube_solved(&TRACKER))
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
    MoveStack* APPLIED_MOVES = move_str_to_MoveStack(moves_to_apply);
    if (!MOVES_TO_APPLY)
    {
        g_print("NO MOVES WERE PROVIDED");
        return;
    }
    if (track_MoveStack(&TRACKER, APPLIED_MOVES))
    {
        g_print("NO MOVES WERE APPLIED");
        free_MoveStack(APPLIED_MOVES);
        return;
    }

    free_MoveStack(APPLIED_MOVES);
    g_print("APPLIED MOVES: %s\n", moves_to_apply);
    g_print("Current reconstruction: %s\n", TRACKER.reconstruction);




}


static void link_button_handlers(WidgetLayout* p_layout)
{
    g_signal_connect(p_layout -> button_get_current_step, "clicked", G_CALLBACK (on_button_get_current_step), NULL);
    g_signal_connect(p_layout -> button_is_solved, "clicked", G_CALLBACK (on_button_is_solved), NULL);
    g_signal_connect(p_layout -> button_apply_moves_from_text, "clicked", G_CALLBACK (on_button_apply_moves_from_text), NULL);
    g_signal_connect(p_layout -> button_apply_scramble_from_text, "clicked", G_CALLBACK (on_button_apply_scramble_from_text), NULL);
}

static void create_button_layout_controls(GtkWindow* APP_WINDOW, WidgetLayout* p_layout)
{
    p_layout -> button_get_current_step = gtk_button_new_with_label("GET CURRENT STEP");
    p_layout -> button_is_solved = gtk_button_new_with_label("IS SOLVED?");
    p_layout -> button_apply_moves_from_text = gtk_button_new_with_label("APPLY MOVES");
    p_layout -> button_apply_scramble_from_text = gtk_button_new_with_label("APPLY SCRAMBLE");

    p_layout -> textField_moves_to_apply = gtk_entry_new();
    p_layout -> textField_scramble = gtk_entry_new();

    p_layout -> WIDGET_GRID = gtk_grid_new();

    gtk_window_set_child (APP_WINDOW, p_layout -> WIDGET_GRID);

    gtk_grid_attach(GTK_GRID(p_layout -> WIDGET_GRID), p_layout -> button_is_solved, 3, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(p_layout -> WIDGET_GRID), p_layout -> textField_moves_to_apply, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(p_layout -> WIDGET_GRID), p_layout -> textField_scramble, 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(p_layout -> WIDGET_GRID), p_layout -> button_apply_scramble_from_text, 3, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(p_layout -> WIDGET_GRID), p_layout -> button_apply_moves_from_text, 3, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(p_layout -> WIDGET_GRID), p_layout -> button_get_current_step, 0, 0, 1, 1);
}

void activate (GApplication *g_app)
{
    initialize_CubeTracker(&TRACKER, "../data.txt", DEFAULT_ENABLED);


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