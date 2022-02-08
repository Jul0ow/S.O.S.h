#include "user_interface.h"

char* get_dir()
{
    char *s = malloc(SIZE * sizeof(char));
    while (getcwd(s, SIZE) == NULL)
        if (realloc(s, SIZE + SIZE) == NULL) errx(1, "realloc");
    return s;
}

char* set_fore_color()
{
    const char *str = "S.O.Sh@S.O.Sh-Project-S4";
    const char *format = "<b><span foreground='#FF00DC'>%s</span><span foreground='white'>:</span><span foreground='#C7BF00'>%s</span><span foreground='white'>$   </span></b>";
    char *markup;
    char* s = get_dir();
    markup = g_markup_printf_escaped(format, str, s);
    free(s);
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

	gtk_widget_set_name(tv, "Text_view");
	gtk_text_view_set_accepts_tab(GTK_TEXT_VIEW(tv), FALSE);
	ui->text_view = GTK_TEXT_VIEW(tv);

        gtk_grid_attach(GTK_GRID(sub_grid), lab, 0, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(sub_grid), tv, 1, 0, 1, 1);
        
	
	GtkWidget *completion = gtk_label_new("");
	gtk_widget_set_name(completion, "completion");
        gtk_grid_attach(GTK_GRID(sub_grid), completion, 2, 0, 1, 1);
        ui->completion = GTK_LABEL(completion);	

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

void new_line(UI *ui, char *text, char *output)
{
    remove_line(ui);
    add_line(ui, 2, text);
    if (text[0] != '\0')
    	add_line(ui, 1, output);
    add_line(ui, 0, "");
}

void execute_command(UI *ui, char* text)
{
    int p[2];
    if (pipe(p) == -1)
	errx(1, "Pipe fails");
    pid_t pid = fork();
    if (pid == -1)
    {
        errx(1, "Could not fork!");
    }
    else if (pid == 0)
    {
	//p[1] becomes the stdout
        close(STDOUT_FILENO);
        if (dup(p[1]) == -1)
	    errx(1, "Dup fails");
        close(p[1]);
        close(p[0]);
        char *const args[] = {"command/ls", NULL};
        execv("command/ls", args);
    }
    else
    {
        if (wait(NULL) == -1)
            errx(1, "The wait function failed");
        close(p[1]);

	remove_line(ui);
        add_line(ui, 2, text);

	char* s = calloc(SIZE, sizeof(char));
        char tmp[1];
        while (read(p[0], tmp, 1) > 0)
        {
	     if (tmp[0] == '\n')
	     {
		 //TODO error when len(s) > SIZE
		 add_line(ui, 1, s);
		 free(s);
		 s = calloc(SIZE, sizeof(char));
	     }
	     else
                 strcat(s, tmp);
        }

	if (*s != '\0')
	    add_line(ui, 1, s);
        add_line(ui, 0, "");

        free(s);
        close(p[0]);
    }
}

char* get_string(UI *ui)
{
    ui->buffer = gtk_text_view_get_buffer(ui->text_view);
    GtkTextIter start = {0,};
    GtkTextIter end = {0,};
    char *text;
    gtk_text_buffer_get_start_iter(ui->buffer, &start);
    gtk_text_buffer_get_end_iter(ui->buffer, &end);
    text = gtk_text_buffer_get_text(ui->buffer, &start, &end, FALSE);
    return text;
}

void evaluate_string(UI *ui)
{
    //TODO Lexeur + Parseur + Eval
    //==============Eval===============    
    //printf("%s\n", text);
    //==============Eval===============

    char *text = get_string(ui);
    new_line(ui, text, text);
    /*
    if (text[0] != '\0') 
        execute_command(ui, text);
    else
	new_line(ui, text, "");
    */
    free(text);
}

/*
//test button
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
*/

char* new_path(LW *lw)
{
    char *s;
    size_t len = 0;
    LW *p;
    for (p = lw->tail; p->prev && p->is_dir; p = p->prev)
	len += strlen(p->word);

    s = malloc((len + 1) * sizeof(char));
    strcpy(s, "./");
    p = p->next;
    for (; p; p = p->next)
	strcat(s, p->word);

    return s;
}

void new_graph_space(UI *ui)
{
    char *s = malloc(3 * sizeof(char));
    strcpy(s, "./");
    LW_append(ui->Gs->lw, s);
    vector_push(ui->Gs->v, ui->Gs->G->last_pos);
    free_Pgraph(ui->Gs->G);
    ui->Gs->G = create_Pgraph_with_dir("./");
}

void new_graph_dir(UI *ui)
{
    char *cpy = malloc((ui->Gs->G->word->index + 1) * sizeof(char));
    strcpy(cpy, ui->Gs->G->word->w);
    LW_append(ui->Gs->lw, cpy);
    ui->Gs->lw->tail->is_dir = 1;
    char* s = new_path(ui->Gs->lw);
    vector_push(ui->Gs->v, ui->Gs->G->last_pos);
    free_Pgraph(ui->Gs->G);
    ui->Gs->G = create_Pgraph_with_dir(s);
    free(s);
}

void update_auto_completion(UI *ui)
{
    ui->Gs->G->cur_pos = ui->Gs->G->last_pos;

    if (ui->Gs->G->word->w[ui->Gs->G->word->index - 1] == ' ')
    {
	/*char *s = malloc(3 * sizeof(char));
	strcpy(s, "./");
	LW_append(ui->Gs->lw, s);
	vector_push(ui->Gs->v, ui->Gs->G->last_pos);
	free_Pgraph(ui->Gs->G);
	ui->Gs->G = create_Pgraph_with_dir("./");*/
	new_graph_space(ui);
    }
    else if (ui->Gs->G->word->w[ui->Gs->G->word->index - 1] == '/')
    {
	/*char *cpy = malloc((ui->Gs->G->word->index + 1) * sizeof(char));
        strcpy(cpy, ui->Gs->G->word->w);
        LW_append(ui->Gs->lw, cpy);
	ui->Gs->lw->tail->is_dir = 1;
	char* s = new_path(ui->Gs->lw);
        vector_push(ui->Gs->v, ui->Gs->G->last_pos);
        free_Pgraph(ui->Gs->G);
        ui->Gs->G = create_Pgraph_with_dir(s);
	free(s);*/
	new_graph_dir(ui);
    }
}

void auto_completion(UI *ui)
{
    char *text = get_string(ui);
    const char *s = gtk_label_get_text(ui->completion);
    strcat(text, s);
    gtk_text_buffer_set_text(ui->buffer, text, strlen(text));
    gtk_text_view_set_buffer(ui->text_view, ui->buffer);
   
    if (s[0] != '\0')
    {
        strcat(ui->Gs->G->word->w, s);	
        ui->Gs->G->word->index += strlen(s);
    }
   
    gtk_label_set_text(ui->completion, ""); 
    update_auto_completion(ui);
    free(text);
}

gboolean on_key_press(GtkWidget *widget, GdkEventKey *evt, gpointer user_data)
{
    widget = widget;
    UI *ui = user_data;

    if (evt->keyval == GDK_KEY_Return)
    {
	evaluate_string(ui);
	free_Pgraphs(ui->Gs);
        Pgraph *G = create_Pgraph_with_dir("./");
        ui->Gs = init_Pgraphs(G);
	return TRUE;
    }
    else if (evt->keyval == GDK_KEY_Tab)
    {
	auto_completion(ui);
    }
    else if (evt->keyval == GDK_KEY_BackSpace)
    {
	if (ui->Gs->G->cur_pos == 0 
			&& ui->Gs->v->size > 0
			&& ui->Gs->G->over_write == 0)
	{
	    free_Pgraph(ui->Gs->G);
	    LW_pop(ui->Gs->lw);
	    char *s = new_path(ui->Gs->lw);
	    ui->Gs->G = create_Pgraph_with_dir(s);
	    ui->Gs->G->cur_pos = *(ui->Gs->v->data + ui->Gs->v->size - 1);
	    vector_pop(ui->Gs->v);
	    reinit_word(ui->Gs->G);
	    free(s);
	}
	
        char *w = get_word(ui->Gs->G, '\0');
        gtk_label_set_text(ui->completion, w);
        free(w);
    }
    else
    {
	if ((char)evt->keyval >= 32 && (char)evt->keyval <= 126)
        {
            char *w = get_word(ui->Gs->G, (char)evt->keyval);
            gtk_label_set_text(ui->completion, w);
            if ((char)evt->keyval == '/' && ui->Gs->G->over_write == 0)
	    {
	        new_graph_dir(ui);
	    }
	    else if ((char)evt->keyval == ' ' && w[0] == 0)
            {
		new_graph_space(ui);
	    }
            free(w);
        }
    }

    return FALSE;
}

void myCSS(void)
{
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen(screen, 
		    GTK_STYLE_PROVIDER (provider), 
		    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    const gchar *myCssFile = "data/mystyle.css";
    GError *error = 0;

    gtk_css_provider_load_from_file(provider, 
		    g_file_new_for_path(myCssFile), &error);
    g_object_unref(provider);
}

void createGrid(GtkWidget **grid, GtkWidget **contain, const gchar *name)
{
    *grid = gtk_grid_new ();
    gtk_widget_set_name(*grid, name);
    gtk_container_add (GTK_CONTAINER (*contain), *grid);
}

int main(void)
{
    gtk_init(NULL, NULL);

    myCSS();

    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "data/interface.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "Main_window"));
    GtkTextBuffer* buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "Text_buffer"));
    //GtkButton* b1 = GTK_BUTTON(gtk_builder_get_object(builder, "Button1"));
    GtkWidget *scroll = GTK_WIDGET(gtk_builder_get_object(builder, "scroll"));

    GtkWidget *grid;
    createGrid(&grid, &scroll, "myGrid");

    UI ui =
    {
	.window = window,
	.grid = GTK_GRID(grid),
	.nb_row = 1,
	.text_view = NULL,
	.buffer = buffer,
	.completion = NULL,
	.Gs = NULL,
	//.b1 = b1,
    };

    Pgraph *G = create_Pgraph_with_dir("./");
    ui.Gs = init_Pgraphs(G);

    add_line(&ui, 0, "");

    gtk_widget_show_all(GTK_WIDGET(ui.window));
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    //g_signal_connect(b1, "clicked", G_CALLBACK(_print), &ui);
    g_signal_connect(window, "key_press_event", G_CALLBACK(on_key_press), &ui);

    gtk_main();
    free_Pgraphs(ui.Gs);

    return 0;
}
