#include"helperWindows.h"
enum {
  COLUMN_LASTNAME,
  COLUMN_FIRSTNAME,
  COLUMN_PATRONYMIC,
  COLUMN_PASSPORT,
  COLUMN_DATE_BEGIN,
  COLUMN_DATE_END,
  N_COLUMNS
};
void click_add_graphic(GtkWidget *button, gpointer data);
void click_remove_graphic(GtkWidget *button, gpointer data);
void click_save_graphic(GtkWidget *button, gpointer data);
void update_graphic(GtkWindow* tree,MYSQL* sql);
void strrem(char* str,int n){
	for (int i=n;i<strlen(str);i++){
		str[i]=str[i+1];
	}
}
void graphicWindow(MYSQL* sql){
	GtkWidget *table;
	GtkWidget *window;
	GtkTreeView *tree;
	GtkWidget *scrolledList;
	GtkWidget *add,*remove,*save;
	gtk_init(NULL, NULL);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_window_set_title(GTK_WINDOW(window), "График отпуска");
	gtk_window_set_default_size (GTK_WINDOW (window), 400, 150);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_icon_from_file (GTK_WINDOW(window), "img/icon.png", NULL);
	scrolledList=gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy (scrolledList,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_min_content_width(scrolledList,600);
	gtk_scrolled_window_set_min_content_height(scrolledList,350);
	table = gtk_table_new (2, 3, FALSE);
	add=gtk_button_new_with_label("Добавить");
	remove=gtk_button_new_with_label("Удалить");
	save=gtk_button_new_with_label("Сохранить");
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
	
	column = gtk_tree_view_column_new_with_attributes ("Начало отпуска",
                                                   renderer,
                                                   "text", COLUMN_DATE_BEGIN,
                                                   NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
	
	column = gtk_tree_view_column_new_with_attributes ("Конец отпуска",
                                                   renderer,
                                                   "text", COLUMN_DATE_END,
                                                   NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
	
	gtk_table_attach(table,scrolledList,0,3,0,1,GTK_FILL,GTK_FILL,15,0);
	gtk_table_attach(table,save,2,3,1,2,GTK_FILL,GTK_FILL,15,0);
	gtk_table_attach(table,remove,1,2,1,2,GTK_FILL,GTK_FILL,15,0);
	gtk_table_attach(table,add,0,1,1,2,GTK_FILL,GTK_FILL,15,0);
	gtk_container_add (GTK_CONTAINER (window), table);
	gtk_widget_show_all(window);
	gpointer** data=(gpointer**)malloc(sizeof(GtkWidget*)*2+sizeof(MYSQL*));
	data[0]=tree;
	data[1]=sql;
	data[2]=window;
	g_signal_connect(GTK_BUTTON(add), "clicked",G_CALLBACK(click_add_graphic), data);
	g_signal_connect(GTK_BUTTON(remove), "clicked",G_CALLBACK(click_remove_graphic), data);
	g_signal_connect(GTK_BUTTON(save), "clicked",G_CALLBACK(click_save_graphic), data);
	g_signal_connect(GTK_WINDOW(window), "delete_event",G_CALLBACK(gtk_main_quit), sql);
	update_graphic(tree,sql);
	
	gtk_main();
}

void click_add_graphic(GtkWidget *button, gpointer data){
	gtk_widget_hide(((GtkWidget**)data)[2]);
	addGraphicWindow(((gpointer**) data)[1]);
	update_graphic(((gpointer**) data)[0],((gpointer**) data)[1]);
	gtk_widget_show(((GtkWidget**)data)[2]);
}

void click_remove_graphic(GtkWidget *button, gpointer data){
	MYSQL *sql=((GtkWidget**)data)[1];
	GtkTreeIter iter;
	GtkListStore* list_store;
	list_store=gtk_tree_view_get_model(((GtkWidget**)data)[0]);
	if (!gtk_tree_model_get_iter_first(list_store,&iter)) {
		GtkWidget *dialog = gtk_message_dialog_new (
								   ((GtkWidget**)data)[2],
								  GTK_DIALOG_MODAL,
                                  GTK_MESSAGE_WARNING,
                                  GTK_BUTTONS_CLOSE,
                                  "Не удалось удалить элемент\nПустой список отпусков");
        gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		return;
	}
	if(!get_selected_iter(((GtkWidget**)data)[0],&iter)){
		GtkWidget *dialog = gtk_message_dialog_new (
								   ((GtkWidget**)data)[2],
								  GTK_DIALOG_MODAL,
                                  GTK_MESSAGE_WARNING,
                                  GTK_BUTTONS_CLOSE,
                                  "Не удалось удалить элемент\nЗапись не выбрана");
        gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		return;
	}
	GtkWidget *dialog = gtk_message_dialog_new (
								  ((GtkWidget**)data)[2],
								  GTK_DIALOG_MODAL,
                                  GTK_MESSAGE_QUESTION,
                                  GTK_BUTTONS_YES_NO,
                                  "Вы действительно хотите удалить\nвыбранную запись?");
	int result=gtk_dialog_run (GTK_DIALOG (dialog));
	if (result==-8){
		GValue val = { 0, },val2 = { 0, },val3 = { 0, };
		gchar *date_begin,*date_end,*passport;
		gtk_tree_model_get_value(list_store,&iter,4,&val);
		date_begin = g_value_get_string(&val);
		strrem( date_begin,4);
		strrem( date_begin,6);
		gtk_tree_model_get_value(list_store,&iter,5,&val2);
		date_end = g_value_get_string(&val2);
		strrem(date_end,4);
		strrem(date_end,6);
		gtk_tree_model_get_value(list_store,&iter,3,&val3);
		passport = g_value_get_string(&val3);
	
		char* request=(char*)calloc(256,sizeof(char));
    	sprintf(request,"SELECT * FROM users WHERE passport = '%s'",passport);
    	mysql_query(sql, request);
    	free(request);
    
		MYSQL_RES *result = mysql_store_result(sql);
  		if (result == NULL) 
  		{
			fprintf(stderr, "%s\n", mysql_error(sql));
      		exit(0);
  		}
		MYSQL_ROW row;
		row = mysql_fetch_row(result);
		request=(char*)calloc(256,sizeof(char));
		sprintf(request,"DELETE FROM graphic WHERE id_user = %s AND date_begin = %s AND date_end= %s",row[0],date_begin,date_end);
 		mysql_query(sql, request);
		free(request);
		g_value_unset(&val);
		g_value_unset(&val2);
		update_graphic(((GtkWidget**)data)[0],sql);
	}
	gtk_widget_destroy (dialog);
}

void click_save_graphic(GtkWidget *button, gpointer data){
	GtkWidget *dialog;
	MYSQL* sql=((MYSQL**)data)[1];

	dialog = gtk_file_chooser_dialog_new ("Save",
                                      ((GtkWidget**)data)[2],
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
    	fprintf(file,"Фамилия,Имя,Отчество,Паспорт,Начало отпуска,Конец отпуска\n");
    	GtkTreeIter iter;
    	GtkListStore* list_store=gtk_tree_view_get_model(((GtkWidget**)data)[0]);
    	if (gtk_tree_model_get_iter_first(list_store,&iter))
    	{
    		do{
    			GValue val_f = { 0, },val_l = { 0, },val_p = { 0, },val_pass = { 0, },val_dataB = { 0, },val_dataE = { 0, };
				gtk_tree_model_get_value(list_store,&iter,0,&val_f);
				gtk_tree_model_get_value(list_store,&iter,1,&val_l);
				gtk_tree_model_get_value(list_store,&iter,2,&val_p);
				gtk_tree_model_get_value(list_store,&iter,3,&val_pass);
				gtk_tree_model_get_value(list_store,&iter,4,&val_dataB);
				gtk_tree_model_get_value(list_store,&iter,5,&val_dataE);
				gchar *firstname,*lastname,*patronymic,*passport,*dataB,*dataE;
				firstname = g_value_get_string(&val_f);
				lastname = g_value_get_string(&val_l);
				patronymic = g_value_get_string(&val_p);
				passport = g_value_get_string(&val_pass);
				dataB = g_value_get_string(&val_dataB);
				dataE = g_value_get_string(&val_dataE);
				fprintf(file,"%s,%s,%s,%s,%s,%s\n",firstname,lastname,patronymic,passport,dataB,dataE);
				g_value_unset(&val_f);
				g_value_unset(&val_l);
				g_value_unset(&val_p);
				g_value_unset(&val_pass);
				g_value_unset(&val_dataB);
				g_value_unset(&val_dataE);
    		}while(gtk_tree_model_iter_next(list_store,&iter));
    	}
		fclose(file);
  	}
	gtk_widget_destroy (dialog);
	
}

void update_graphic(GtkWindow* tree,MYSQL* sql){
	GtkListStore* list_store = gtk_list_store_new (N_COLUMNS,
                                   G_TYPE_STRING,
                                   G_TYPE_STRING,
								   G_TYPE_STRING,
								   G_TYPE_STRING,
                                   G_TYPE_STRING,
                                   G_TYPE_STRING);
	GtkTreeIter iter;
	
	mysql_query(sql, "SELECT * FROM graphic");
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
						  	COLUMN_DATE_BEGIN,  row[1],
						  	COLUMN_DATE_END,  row[2],
                          	-1);
	}

	gtk_tree_view_set_model (tree,list_store);
}
