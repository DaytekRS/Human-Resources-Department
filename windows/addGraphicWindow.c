#include"helperWindows.h"
enum {
  COLUMN_ID,
  COLUMN_LASTNAME,
  COLUMN_FIRSTNAME,
  COLUMN_PATRONYMIC,
  COLUMN_PASSPORT,
  N_COLUMNS
};
void add_button_graphic(GtkWidget *button, gpointer data);
void addGraphicWindow(MYSQL* sql){
	GtkWidget *window;
	GtkWidget *table;
	GtkTreeView *tree;
	GtkWidget *scrolledList;
	GtkButton *add;
	GtkWidget *lBegin,*lEnd;
	GtkCalendar *calendar_begin,*calendar_end;

  
	gtk_init(NULL, NULL);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_window_set_title(GTK_WINDOW(window), "Добавить");
	gtk_window_set_default_size (GTK_WINDOW (window), 400, 150);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_icon_from_file (GTK_WINDOW(window), "img/icon.png", NULL);
	table = gtk_table_new (3, 4, FALSE);
	lBegin=gtk_label_new("Дата начала отпука");
	lEnd=gtk_label_new("Дата окончания отпуска");
	
	scrolledList=gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy (scrolledList,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_min_content_width(scrolledList,380);
	gtk_scrolled_window_set_min_content_height(scrolledList,150);
	
    calendar_begin=gtk_calendar_new();
    calendar_end=gtk_calendar_new();
	add = gtk_button_new_with_label("Добавить");
	
	tree=gtk_tree_view_new();
	gtk_container_add (GTK_CONTAINER (scrolledList), tree);
	
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	renderer = gtk_cell_renderer_text_new ();
	

	column = gtk_tree_view_column_new_with_attributes ("Фамилия",
                                                   renderer,
                                                   "text", COLUMN_LASTNAME,
                                                   NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
	column = gtk_tree_view_column_new_with_attributes ("Имя",
                                                   renderer,
                                                   "text", COLUMN_FIRSTNAME,
                                                   NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
	column = gtk_tree_view_column_new_with_attributes ("Отчество",
                                                   renderer,
                                                   "text", COLUMN_PATRONYMIC,
                                                   NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
	column = gtk_tree_view_column_new_with_attributes ("Паспорт",
                                                   renderer,
                                                   "text", COLUMN_PASSPORT,
                                                   NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
	update(tree,sql,"");
	
	
	gtk_table_attach(table,scrolledList,0,2,0,1,GTK_SHRINK,GTK_FILL,0,10);
	gtk_table_attach(table,lBegin,0,1,1,2,GTK_SHRINK,GTK_EXPAND,0,10);
	gtk_table_attach(table,lEnd,0,1,2,3,GTK_SHRINK,GTK_EXPAND,0,10);
	gtk_table_attach(table,calendar_begin,1,2,1,2,GTK_SHRINK,GTK_EXPAND,0,10);
	gtk_table_attach(table,calendar_end,1,2,2,3,GTK_SHRINK,GTK_EXPAND,0,10);
	gtk_table_attach(table,add,1,2,3,4,GTK_SHRINK,GTK_EXPAND,0,10);
	
	
	gtk_container_add (GTK_CONTAINER (window), table);
	gtk_widget_show_all(window);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gpointer** data=(gpointer**)malloc(sizeof(GtkWidget*)*4+ sizeof(MYSQL*) );
	data[0]=tree;
	data[1]=calendar_begin;
	data[2]=calendar_end;
	data[3]=window;
	data[4]=sql;

	g_signal_connect(GTK_BUTTON(add), "clicked",G_CALLBACK(add_button_graphic), data);

	gtk_main();
}

void add_button_graphic(GtkWidget *button, gpointer data){
	MYSQL *sql=((MYSQL**)data)[4];
	GtkTreeIter iter;
	GtkListStore* list_store;
	list_store=gtk_tree_view_get_model(((GtkWidget**)data)[0]);
	if (!gtk_tree_model_get_iter_first(list_store,&iter)) {
		GtkWidget *dialog = gtk_message_dialog_new (
								   ((GtkWidget**)data)[3],
								  GTK_DIALOG_MODAL,
                                  GTK_MESSAGE_WARNING,
                                  GTK_BUTTONS_CLOSE,
                                  "Не удалось добавить элемент\nПустой список сотрудников");
        gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		return;
	}
	if(!get_selected_iter(((GtkWidget**)data)[0],&iter)){
		GtkWidget *dialog = gtk_message_dialog_new (
								   ((GtkWidget**)data)[3],
								  GTK_DIALOG_MODAL,
                                  GTK_MESSAGE_WARNING,
                                  GTK_BUTTONS_CLOSE,
                                  "Не удалось добавить элемент\nСотрудник не выбран");
        gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		return;
	}
	GValue val = { 0, };
	gint num_columb;
	gtk_tree_model_get_value(list_store,&iter,0,&val);
	num_columb = g_value_get_int (&val);
	g_value_unset(&val);
	char* date_begin=(char*)calloc(9,sizeof(char));
	char* date_end=(char*)calloc(9,sizeof(char));
	int year,day,month;
	gtk_calendar_get_date (((GtkWidget**)data)[1],&year,&month,&day);
	sprintf(date_begin,"%d%0.2d%0.2d",year,month+1,day);
	gtk_calendar_get_date (((GtkWidget**)data)[2],&year,&month,&day);
	sprintf(date_end,"%d%0.2d%0.2d",year,month+1,day);
	if (atoi(date_end)<=atoi(date_begin)){
		GtkWidget *dialog = gtk_message_dialog_new (
								  ((GtkWidget**)data)[3],
								  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_WARNING,
                                  GTK_BUTTONS_CLOSE,
                                  "Ошибка добавления\nДата конца отпуска не может\nбыть раньше его начала");
	  	gtk_dialog_run (GTK_DIALOG (dialog));
 	  	gtk_widget_destroy (dialog);
		return;
	}
	int size=strlen(date_begin)+strlen(date_end)+10;
	char* request=(char*)calloc(size+60,sizeof(char));
	sprintf(request,"INSERT INTO graphic VALUES ('%d','%s','%s')",num_columb,date_begin,date_end);
	mysql_query(sql, request);
	free(request);
	free(date_end);
	free(date_begin);
	gtk_widget_destroy (((GtkWidget**)data)[3]);
}



