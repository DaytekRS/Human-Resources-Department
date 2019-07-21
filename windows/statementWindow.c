#include"helperWindows.h"
enum {
  COLUMN_LASTNAME,
  COLUMN_FIRSTNAME,
  COLUMN_PATRONYMIC,
  COLUMN_PASSPORT,
  COLUMN_PLACE,
  COLUMN_DATE,
  N_COLUMNS
};
void update_statement(GtkWindow* tree,MYSQL* sql);
void save_statement_button(GtkWidget *button, gpointer data);
void statementWindow(MYSQL* sql){
	GtkWidget *table;
	GtkWidget *window;
	GtkTreeView *tree;
	GtkWidget *save;
	GtkWidget *scrolledList;
	gtk_init(NULL, NULL);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_window_set_title(GTK_WINDOW(window), "Ведомость о перемещениях");
	gtk_window_set_default_size (GTK_WINDOW (window), 400, 150);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_icon_from_file (GTK_WINDOW(window), "img/icon.png", NULL);
	scrolledList=gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy (scrolledList,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_min_content_width(scrolledList,500);
	gtk_scrolled_window_set_min_content_height(scrolledList,320);
	table = gtk_table_new (2, 3, FALSE);
	save = gtk_button_new_with_label("Сохранить");
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
	
	column = gtk_tree_view_column_new_with_attributes ("Место работы",
                                                   renderer,
                                                   "text", COLUMN_PLACE,
                                                   NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
	
	column = gtk_tree_view_column_new_with_attributes ("Дата",
                                                   renderer,
                                                   "text", COLUMN_DATE,
                                                   NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
	
	gtk_table_attach(table,scrolledList,0,3,0,1,GTK_FILL,GTK_FILL,15,0);
	gtk_table_attach(table,save,2,3,1,2,GTK_FILL,GTK_FILL,15,0);
	
	gtk_container_add (GTK_CONTAINER (window), table);
	gtk_widget_show_all(window);
	g_signal_connect(GTK_WINDOW(window), "delete_event",G_CALLBACK(gtk_main_quit), sql);
	
	gpointer** data=(gpointer**)malloc(sizeof(GtkWidget*)+sizeof(MYSQL*));
	data[0]=window;
	data[1]=sql;
	g_signal_connect(GTK_BUTTON(save), "clicked",G_CALLBACK(save_statement_button), data);
	update_statement(tree,sql);
	
	gtk_main();
}

void save_statement_button(GtkWidget *button, gpointer data){
	GtkWidget *dialog;
	MYSQL* sql=((MYSQL**)data)[1];
	dialog = gtk_file_chooser_dialog_new ("Save",
                                      ((GtkWidget**)data)[0],
                                      GTK_FILE_CHOOSER_ACTION_SAVE,
                                      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                      NULL);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
  	{
    	char *filename;

    	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    	
    	
    	char *filedirectory=(char*)calloc(sizeof(char),strlen(filename)+5);
    	sprintf(filedirectory,"%s%s",filename,".csv");
    	FILE *file=fopen(filedirectory,"wb+");
    	free(filedirectory);

    	fprintf(file,"Фамилия,Имя,Отчество,Паспорт,Место работы,Дата\n");
    	
    	
    	mysql_query(sql, "SELECT * FROM statement");
		MYSQL_RES *result = mysql_store_result(sql);
  		if (result == NULL) 
  		{
			fprintf(stderr, "%s\n", mysql_error(sql));
      		exit(0);
  		}
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(result)))
		{ 
			char* request=(char*)calloc(sizeof(char),256);
			sprintf(request,"SELECT * FROM users WHERE id = %s",row[0]);
			mysql_query(sql, request);
			free(request);
			MYSQL_RES *result_user = mysql_store_result(sql);
  			if (result_user == NULL) 
  			{
				fprintf(stderr, "%s\n", mysql_error(sql));
      			exit(0);
  			}
			MYSQL_ROW row_user= mysql_fetch_row(result_user);
			fprintf(file,"%s,%s,%s,%s,%s,%s\n",row_user[1],row_user[2],row_user[3],row_user[6],row[1],row[2]);
		}
		fclose(file);
		
		
  	}

	gtk_widget_destroy (dialog);
	
}


void update_statement(GtkWindow* tree,MYSQL* sql){
	GtkListStore* list_store = gtk_list_store_new (N_COLUMNS,
                                   G_TYPE_STRING,
                                   G_TYPE_STRING,
								   G_TYPE_STRING,
								   G_TYPE_STRING,
                                   G_TYPE_STRING,
                                   G_TYPE_STRING);
	GtkTreeIter iter;
	
	mysql_query(sql, "SELECT * FROM statement");
	MYSQL_RES *result = mysql_store_result(sql);
  	if (result == NULL) 
  	{
		fprintf(stderr, "%s\n", mysql_error(sql));
      	exit(0);
  	}
	MYSQL_ROW row;
	
	while ((row = mysql_fetch_row(result))){ 
		gtk_list_store_append (list_store, &iter);
		
		char* request=(char*)calloc(sizeof(char),256);
		sprintf(request,"SELECT * FROM users WHERE id = %s",row[0]);
		mysql_query(sql, request);
		free(request);
		MYSQL_RES *result_user = mysql_store_result(sql);
  		if (result_user == NULL) 
  		{
			fprintf(stderr, "%s\n", mysql_error(sql));
      		exit(0);
  		}
		MYSQL_ROW row_user= mysql_fetch_row(result_user);
		gtk_list_store_set (list_store, &iter,
                          	COLUMN_LASTNAME, row_user[1],
                          	COLUMN_FIRSTNAME, row_user[2],
                          	COLUMN_PATRONYMIC,  row_user[3],
						  	COLUMN_PASSPORT,  row_user[6],
						  	COLUMN_PLACE,  row[1],
						  	COLUMN_DATE,  row[2],
                          	-1);
	}

	gtk_tree_view_set_model (tree,list_store);
}
