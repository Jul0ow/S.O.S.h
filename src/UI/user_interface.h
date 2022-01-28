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

#define SIZE 264

typedef struct UserInterface
{
    GtkWindow *window;
    GtkGrid *grid;
    size_t nb_row;
    GtkTextView *text_view;
    GtkTextBuffer *buffer;
    //GtkButton *b1;
}UI;

#endif
