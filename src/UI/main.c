#include "user_interface.h"

//==================================================================
// Utils

void file_type(char *filename)
{
    struct stat buf;
    if (stat(filename, &buf) == -1)
        errx(1, "Problem");
    if (!S_ISDIR(buf.st_mode))
        errx(1, "cd: %s: Not a directory\n", filename);
}

//===================================================================
// Functions for history

void print_hist(History* hist)
{
    Lhist* lhist = hist->lhist;
    lhist = lhist->next;
    for (; lhist; lhist = lhist->next)
	printf("%s\n", lhist->data);
}

Lhist* init_lhist(char* data)
{
    Lhist* lhist = malloc(sizeof(Lhist));
    lhist->next = NULL;
    lhist->prev = NULL;
    lhist->data = data;
    return lhist;
}

History* init_hist(int load)
{
    load = load;
    History* hist = malloc(sizeof(History));
    hist->lhist = init_lhist(NULL);
    hist->tail = hist->lhist;
    hist->size = 0;
    hist->index = -1;
    return hist;
}

void free_lhist(Lhist* lhist)
{
    free(lhist->data);
    free(lhist);
}

void free_hist(History* hist)
{
    Lhist* p;
    Lhist* lhist = hist->lhist;
    while (lhist != NULL)
    {
	p = lhist->next;
	free_lhist(lhist);
	lhist = p;
    }

    free(hist);
}

void append_hist(History* hist, char* data)
{
    hist->index = hist->size - 1;
    if (hist->tail->data && strcmp(hist->tail->data, data) == 0)
	return;

    Lhist* lhist = hist->lhist;
    Lhist* elm = init_lhist(data);
    for (; lhist->next; lhist = lhist->next)
        continue;
    elm->prev = lhist;
    elm->next = lhist->next;
    lhist->next = elm;

    hist->tail = elm;
    hist->index = hist->size;
    hist->size += 1;
}

void pop_hist(History *hist)
{
    if (hist->tail->prev)
    {
        Lhist *p = hist->tail;
        hist->tail->prev->next = NULL;
        hist->tail = p->prev;
        free_lhist(p);
	hist->size--;
    }
}

char* get_data(History* hist)
{
    Lhist* lhist = hist->lhist->next;
    for (int i = 0; i < hist->index; i++)
	lhist = lhist->next;
    return lhist->data;
}

void update_string(UI* ui)
{
    if (ui->hist->index >= 0 && ui->hist->index < ui->hist->size)
    {
	printf("ok\n");
        History* hist = ui->hist;
        char* str = get_data(hist);
        gtk_text_buffer_set_text(ui->buffer, str, strlen(str));
        gtk_text_view_set_buffer(ui->text_view, ui->buffer);
    }
}

//===================================================================
// Functions for create a new line in the shell

char* get_dir()
{
    char *s = malloc(SIZE * sizeof(char));
    while (getcwd(s, SIZE) == NULL)
        if ((s = realloc(s, SIZE + SIZE)) == NULL) errx(1, "realloc");
    return s;
}

char* set_fore_color()
{
    const char *str = "S.O.Sh@S.O.Sh-Project-S4";
    const char *format = "<b><span foreground='#FF00DC'>%s</span>"
	"<span foreground='white'>:</span><span foreground='#C7BF00'>%s</span>"
	"<span foreground='white'>$   </span></b>";
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

//===================================================================
// Functions to execute some commands

int my_parse(UI* ui, char* buf, Args* args)
{
    ui = ui;
    size_t i = 0;
    size_t len = 0;

    char *ptr;
    while ((ptr = my_str_split(buf, ' ', &i)) != NULL && len < args->len)
    {
        args->args[len] = ptr;
        len++;
    }

    args->args[args->len - 1] = (char *)NULL;

    if (!valid_command(args->args[0]))
    {
	printf("%s: Invalid command\n", args->args[0]);
	return -1;
    }

    // TODO memory leak I think not sure
    char *s = malloc((strlen(args->args[0]) + 9) * sizeof(char));
    strcpy(s, "command/");
    strcat(s, args->args[0]);
    args->command = s;

    //printf("%s\n", args->args[0]);
    
    return 0;
}

void execute_command(UI *ui, char* text)
{
    //printf("%s\n", text);
    Args* args = init_args(text);
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
	
	if (my_parse(ui, text, args) == -1)
	{
	    exit(EXIT_SUCCESS);
	}
	else if (strcmp(args->args[0], "firefox") == 0)
	{
            execl("/usr/bin/firefox", "firefox", args->args[1], NULL);
	}
        else if (strcmp(args->args[0], "lexeur") == 0)
        {
            /*printf("ok\n");
            printf("%ld\n", args->len);
            int len = 0;
            for (size_t i = 1; i < args->len; i++)
                len += strlen(args->args[i]);
            printf("%d\n", len);
            char* s = malloc((len + 1) * sizeof(char));
            for (size_t i = 1; i < args->len; i++)
                strcat(s, args->args[i]);
            printf("OK\n");
            printf("%s\n", s);*/
            execl("../../soutenance/lexing", "lexing", args->args, NULL);
            //execl("command/ls", "ls", NULL);
        }
	else if (execv(args->command, args->args) == -1)
	    printf("%s: An error occurs\n", args->args[0]);

	exit(EXIT_SUCCESS);

	//char* args[] = {"command/ls", NULL};
        /*char **args = malloc(2 * sizeof(char*));
	char *s = malloc(SIZE * sizeof(char));
	strcpy(s, "command/ls");
	*args = s;
	*(args + 1) = (char*) NULL;*/
        //execv("command/ls", args);
    }
    else
    {
        /*if (wait(NULL) == -1)
            errx(1, "The wait function failed");*/
        close(p[1]);

	//free_args(args);

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
	     {
		 if (tmp[0] == ' ')
		     strcat(s, "   ");
		 else if (tmp[0] == '\t')
		     strcat(s, "                        ");
		 else
                     strcat(s, tmp);
	     }
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
    //new_line(ui, text, text);

    if (text[0] != '\0')
    {
        execute_command(ui, text);
	append_hist(ui->hist, text);
	//print_hist(ui->hist);
    }
    else
    {
	new_line(ui, text, "");
        free(text);
    }

}

//===================================================================
// Auto-completion functions

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
    //printf("append -> OK\n");
    vector_push(ui->Gs->v, ui->Gs->G->last_pos);
    free_Pgraph(ui->Gs->G);
    //printf("free -> OK\n");
    ui->Gs->G = create_Pgraph_with_dir("./");
    //printf("create -> OK\n");
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
	//printf("detected -> OK\n");
	new_graph_space(ui);
    }
    else if (ui->Gs->G->word->w[ui->Gs->G->word->index - 1] == '/')
    {
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

//===================================================================
// Function which creates a new shell

void new_window()
{
    pid_t pid = fork();
    if (pid == -1)
    {
        errx(1, "Could not fork!");
    }
    else if (pid == 0)
    {
	char* args[] = {"./main", NULL};
        if (execv("./main", args) == -1)
            errx(1, "error open new shell");
        exit(EXIT_SUCCESS);
    }
    else
    {
	// TODO zombie if not wait
	return;
    }
}

//===================================================================
// Key management

// gets the key press
gboolean on_key_press(GtkWidget *widget, GdkEventKey *evt, gpointer user_data)
{
    widget = widget;
    UI *ui = user_data;

    if (evt->keyval == GDK_KEY_Alt_L)
    {
	printf("Nothing Loooooooooooooooooooooooooooooooool\n");
    }
    else if (evt->keyval == GDK_KEY_Return)
    {
	evaluate_string(ui);
	free_Pgraphs(ui->Gs);
        Pgraph *G = create_Pgraph_with_dir("./");
        ui->Gs = init_Pgraphs(G);
	return TRUE;
    }
    else if (evt->keyval == GDK_KEY_Up)
    {
	printf("UP\n");
        printf("index = %d, size = %d\n", ui->hist->index, ui->hist->size);
	update_string(ui);
	if (ui->hist->index > 0)
	    ui->hist->index--;
	return TRUE;
    }
    else if (evt->keyval == GDK_KEY_Down)
    {
	printf("DOWN\n");
	if (ui->hist->index < ui->hist->size)
	    ui->hist->index++;
	update_string(ui);
	return TRUE;
    }
    else if (evt->keyval == GDK_KEY_Tab)
    {
	auto_completion(ui);
    }
    else if (evt->keyval == GDK_KEY_Control_L)
    {
	new_window();
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
	        new_graph_dir(ui);
	    else if ((char)evt->keyval == ' ' && w[0] == 0)
		new_graph_space(ui);
            free(w);
        }
    }

    return FALSE;
}

//===================================================================
// Main functions (init and display the shell)

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
        errx(EXIT_FAILURE, "Load fail");
    }

    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "Main_window"));
    GtkTextBuffer* buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "Text_buffer"));
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
	.hist = NULL,
    };

    ui.hist = init_hist(0);
    Pgraph *G = create_Pgraph_with_dir("./");
    ui.Gs = init_Pgraphs(G);

    add_line(&ui, 0, "");
   
    signal(SIGCHLD, SIG_IGN); 
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "key_press_event", G_CALLBACK(on_key_press), &ui);

    gtk_widget_show_all(GTK_WIDGET(ui.window));

    gtk_main();
    free_hist(ui.hist);
    free_Pgraphs(ui.Gs);

    return 0;
}

// Test for thread (does not work)
/*
void* __sleep(void* arg)
{
    sleep(1);
    Loader* load = arg;
    GdkPixbufAnimation *pixbuf =
	    gdk_pixbuf_animation_new_from_file("animation.gif", NULL);
    printf("OK\n");
    gtk_image_set_from_animation(load->anim, pixbuf);
    gtk_widget_show(GTK_WIDGET(load->w));
    sleep(5);
    gtk_window_close(load->w);
    return NULL;
}

int main(void)
{
    gtk_init(NULL, NULL);
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "data/loader.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }
   
    printf("OK\n"); 
    GtkWindow* w = GTK_WINDOW(gtk_builder_get_object(builder, "Loader"));
    GtkImage *anim = GTK_IMAGE(gtk_builder_get_object(builder, "Animation"));
   
    Loader load;
    load.w = w;
    load.anim = anim;

    printf("OK1\n");
    pthread_t thr;
    int e = pthread_create(&thr, NULL, __sleep, &load);
    if (e != 0)
	errx(1, "pthread fail to create");

    pthread_detach(thr);

    printf("OK2\n");
    g_signal_connect(w, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_main();

    printf("finishi\n");
    return 0;
}
*/
