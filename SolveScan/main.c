//
// Created by Ven on 2025/12/20.
//

//#include "RouxTracker.h"

#include "Application/App.h"

#include <gtk/gtk.h>

// int CubeTest();
// int ParseMoveTest();
// int ApplyMoveSeqTest(Cube* TEST);
// int RouxTrackerTest();

extern RouxTracker* tracker;


int main (int argc, char** argv)
{
    GtkApplication* gtk_app = gtk_application_new("main.c_test", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(gtk_app, "activate", G_CALLBACK(activate), NULL);


    return g_application_run (G_APPLICATION (gtk_app), argc, argv);
}
