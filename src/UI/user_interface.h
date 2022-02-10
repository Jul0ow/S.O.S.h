#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <gtk/gtk.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#include "prefix_graph.h"
#include "my_parse.h"

#define SIZE 264

typedef struct Loader
{
    GtkWindow* w;
    GtkImage* anim;
}Loader;

typedef struct UserInterface
{
    GtkWindow *window;
    GtkGrid *grid;
    size_t nb_row;
    GtkTextView *text_view;
    GtkTextBuffer *buffer;
    GtkLabel *completion;
    Pgraphs *Gs;
    //GtkButton *b1;
}UI;

#endif
