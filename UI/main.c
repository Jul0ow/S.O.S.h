#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <gtk/gtk.h>

typedef struct UserInterface
{
    GtkWindow *window;
    GtkGrid *grid;
    size_t nb_row;
    GtkTextView *text_view;
    GtkTextBuffer *buffer;
    GtkButton *b1;
}UI;

//=========================================================================
void my_print(UI *ui, const char* s, int len)
{
    gtk_text_buffer_set_text(ui->buffer, s, len);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(ui->text_view), ui->buffer);
}

void add_text_view(UI *ui)
{
    ui->text_view = GTK_TEXT_VIEW(gtk_text_view_new());
    gtk_grid_insert_column(ui->grid, ui->nb_row);
    ui->nb_row += 1; 
    gtk_grid_attach(ui->grid, GTK_WIDGET(ui->text_view), 0, ui->nb_row, 1, 1);
    ui->buffer = gtk_text_view_get_buffer(ui->text_view);
    my_print(ui, "S.O.Sh: ", 8);
    gtk_widget_show_all(GTK_WIDGET(ui->window));
}
//=========================================================================

char* set_fore_color()
{
    const char *str = "S.O.Sh Ryugo$   ";
    const char *format = "<span foreground='green'><b>%s</b></span>";
    char *markup;
    markup = g_markup_printf_escaped(format, str);
    return markup;
}

void add_line(UI *ui, int is_label, char *text)
{
    gtk_grid_insert_column(ui->grid, ui->nb_row);
    ui->nb_row += 1;
    
    if (is_label == 1)
    {
        GtkWidget *lab  = gtk_label_new(text);
	gtk_widget_set_halign(GTK_WIDGET(lab), GTK_ALIGN_START);
	gtk_grid_attach(ui->grid, lab, 0, ui->nb_row, 1, 1);
    }
    else if (is_label == 2)
    {
	GtkWidget *lab = gtk_label_new(NULL);
	char *tmp = set_fore_color();
        size_t len = strlen(tmp) + strlen(text) + 1;
	char *markup = calloc(len, sizeof(char));
        strcpy(markup, tmp);
	strcat(markup, text);
	gtk_label_set_markup(GTK_LABEL(lab), markup);

	gtk_widget_set_halign(GTK_WIDGET(lab), GTK_ALIGN_START);
        gtk_grid_attach(ui->grid, lab, 0, ui->nb_row, 1, 1);
	g_free(tmp);
	free(markup);
    }
    else
    {
	//new line
        GtkWidget *lab = gtk_label_new(NULL);
        GtkWidget *tv = gtk_text_view_new();
        GtkWidget *sub_grid = gtk_grid_new();

        char *markup = set_fore_color();
        gtk_label_set_markup(GTK_LABEL(lab), markup);
        g_free(markup);

	ui->text_view = GTK_TEXT_VIEW(tv);

        gtk_grid_attach(GTK_GRID(sub_grid), lab, 0, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(sub_grid), tv, 1, 0, 1, 1);

        gtk_grid_attach(ui->grid, sub_grid, 0, ui->nb_row, 1, 1);
    }
    
    gtk_widget_show_all(GTK_WIDGET(ui->window));
}

void remove_line(UI *ui)
{
    gtk_grid_remove_row(ui->grid, ui->nb_row);
    ui->nb_row -= 1;
    gtk_widget_show_all(GTK_WIDGET(ui->window));
}

void new_line(UI *ui, char *text)
{
    remove_line(ui);
    add_line(ui, 2, text);
    if (strcmp(text, "") != 0)
    	add_line(ui, 1, text);
    add_line(ui, 0, "");
}

void get_string(UI *ui)
{
    ui->buffer = gtk_text_view_get_buffer(ui->text_view);
    GtkTextIter start = {0,};
    GtkTextIter end = {0,};
    char *text = "";
    gtk_text_buffer_get_start_iter(ui->buffer, &start);
    gtk_text_buffer_get_end_iter(ui->buffer, &end);
    text = gtk_text_buffer_get_text(ui->buffer, &start, &end, FALSE);
    
    //==============Eval===============    
    //printf("%s\n", text);
    //==============Eval===============

    new_line(ui, text);
    free(text);
}

void _print(GtkButton *button, gpointer user_data)
{
    UI *ui = user_data;
    if (button == ui->b1)
    {
	get_string(ui);
    }
    else
    {
	remove_line(ui);
    }
}

gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    widget = widget;
    UI *ui = user_data;

    if (event->keyval == GDK_KEY_Return)
    {
	get_string(ui);
	return TRUE;
    }
    return FALSE;
}

int main(void)
{
    gtk_init(NULL, NULL);

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "test_interface.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "Main_window"));
    //GtkTextView *text_view = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "Text_view"));
    GtkTextBuffer* buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "Text_buffer"));
    GtkButton* b1 = GTK_BUTTON(gtk_builder_get_object(builder, "Button1"));

    GtkViewport *VP = GTK_VIEWPORT(gtk_builder_get_object(builder, "VP"));
    //GtkEventBox *eb = GTK_EVENT_BOX(gtk_builder_get_object(builder, "event"));
    GtkGrid *grid = GTK_GRID(gtk_grid_new());

    UI ui =
    {
	.window = window,
	.grid = grid,
	.nb_row = 1,
	.text_view = NULL,
	.buffer = buffer,
	.b1 = b1,
    };

    gtk_container_add(GTK_CONTAINER(VP), GTK_WIDGET(ui.grid));
    add_line(&ui, 0, "");

    gtk_widget_show_all(GTK_WIDGET(ui.window));

    //my_print(&ui, "S.O.Sh: Ryugo$ ", 15);
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(b1, "clicked", G_CALLBACK(_print), &ui);
    g_signal_connect(window, "key_press_event", G_CALLBACK(on_key_press), &ui);

    gtk_main();

    return 0;
}

