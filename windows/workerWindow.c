#include"helperWindows.h"
#include <stdbool.h>
enum {
  COLUMN_ID,
  COLUMN_LASTNAME,
  COLUMN_FIRSTNAME,
  COLUMN_PATRONYMIC,
  COLUMN_PASSPORT,
  N_COLUMNS
};

void update(GtkTreeView *tree,MYSQL* sql,char* search);
void add_button(GtkWidget *button, gpointer data);
void remove_button(GtkWidget *button, gpointer data);
void edit_button(GtkWidget *button, gpointer data);
void search_write(GtkWidget *enrery, gpointer data)
{
	update(((gpointer**) data)[0],((gpointer**) data)[1],gtk_entry_get_text(((GtkWidget**) data)[3]));
}

gboolean get_selected_iter(GtkTreeView *tree_view,GtkTreeIter *iter)
{
 GtkTreeSelection *selection = gtk_tree_view_get_selection(tree_view);
 if (gtk_tree_selection_get_selected (selection,NULL,iter))
    return TRUE;
 return FALSE;
}

void workerWindow(MYSQL* sql){
	GtkWidget *table;
	GtkWidget *window;
	GtkTreeView *tree;
	GtkWidget *add,*edit,*remove;
	GtkWidget *search;
	GtkWidget *lSearch;  
	GtkWidget *scrolledList;
	gtk_init(NULL, NULL);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 5);
	gtk_window_set_title(GTK_WINDOW(window), "Работники");
	gtk_window_set_default_size (GTK_WINDOW (window), 400, 150);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_icon_from_file (GTK_WINDOW(window), "img/icon.png", NULL);
	scrolledList=gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy (scrolledList,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_min_content_width(scrolledList,380);
	gtk_scrolled_window_set_min_content_height(scrolledList,250);
	table = gtk_table_new (4, 3, FALSE);
	add = gtk_button_new_with_label   ("      Добавить     ");
	edit = gtk_button_new_with_label  (" Редактировать ");
	remove = gtk_button_new_with_label("       Удалить       ");
	lSearch=gtk_label_new("Поиск:");
	search=gtk_entry_new();
	
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
	
	
	
	
	gtk_table_attach(table,scrolledList,0,4,1,2,GTK_FILL,GTK_FILL,15,0);

	gtk_table_attach(table,add,0,1,2,3,GTK_EXPAND,GTK_EXPAND,15,10);
	gtk_table_attach(table,remove,1,3,2,3,GTK_FILL,GTK_EXPAND,10,10);
	gtk_table_attach(table,edit,3,4,2,3,GTK_FILL,GTK_EXPAND,15,10);
	gtk_table_attach(table,lSearch,2,3,0,1,GTK_FILL,GTK_EXPAND,0,5);
	gtk_table_attach(table,search,3,4,0,1,GTK_EXPAND,GTK_EXPAND,15,10);


	gtk_container_add (GTK_CONTAINER (window), table);
	gtk_widget_show_all(window);
	
	gpointer** data_add=(gpointer**)malloc(sizeof(GtkTreeView*)*3+sizeof(MYSQL*));
	data_add[0]=tree;
	data_add[1]=sql;
	data_add[2]=window;
	data_add[3]=search;
	g_signal_connect(GTK_BUTTON(add), "clicked", G_CALLBACK(add_button), data_add);
	gpointer** data_remove=(gpointer**)malloc(sizeof(GtkWidget*)*3+sizeof(MYSQL*));
	data_remove[0]=tree;
	data_remove[1]=sql;
	data_remove[2]=window;
	data_remove[3]=search;
	g_signal_connect(GTK_BUTTON(remove), "clicked", G_CALLBACK(remove_button), data_remove);
	g_signal_connect(GTK_WINDOW(window), "delete_event",G_CALLBACK(gtk_main_quit), sql);
	g_signal_connect(GTK_BUTTON(edit), "clicked", G_CALLBACK(edit_button), data_remove);
	
	g_signal_connect(GTK_ENTRY(search), "activate", G_CALLBACK(search_write), data_add);

	update(tree,sql,"");
	gtk_main();
}


void update(GtkTreeView *tree,MYSQL* sql,char* search){
	GtkListStore* list_store = gtk_list_store_new (N_COLUMNS,
                                   G_TYPE_INT,
                                   G_TYPE_STRING,
								   G_TYPE_STRING,
								   G_TYPE_STRING,
                                   G_TYPE_STRING);
	GtkTreeIter iter;
	
	mysql_query(sql, "SELECT * FROM users");
	MYSQL_RES *result = mysql_store_result(sql);
  	if (result == NULL) 
  	{
		fprintf(stderr, "%s\n", mysql_error(sql));
      	exit(0);
  	}
	MYSQL_ROW row;
	
	while ((row = mysql_fetch_row(result))){ 
		
		//fprintf(stderr,"%d\n",atoi(row[0]));
	
		char *strForSearch=(char*)calloc(sizeof(char),strlen(row[1])+strlen(row[2])+strlen(row[3])+strlen(row[6])+10);
		sprintf(strForSearch,"%s %s %s %s\0",row[1],row[2],row[3],row[6]);
	
		char *temp_search=search;
		
		
		char *strSearch=(char*)calloc(sizeof(char*),strlen(search)+1);
		strcpy(strSearch,search);
		char *word = strtok(strSearch," ");
		
		bool sucssefful=TRUE;
		
		while (word != NULL)                     
  		{
      		if (strstr(strForSearch,word)==NULL)
      		{
      			sucssefful=FALSE;
      			break;
      			
			}
      		word = strtok (NULL, " ");
 		}
 		
 		if (sucssefful)
 		{
 			gtk_list_store_append (list_store, &iter);
			gtk_list_store_set (list_store, &iter,
                          		COLUMN_ID, atoi(row[0]),
                          		COLUMN_LASTNAME, row[1],
                          		COLUMN_FIRSTNAME,  row[2],
						  		COLUMN_PATRONYMIC,  row[3],
						  		COLUMN_PASSPORT,  row[6],
                          		-1);
 		}
		free(strSearch);
		free(strForSearch);
	}

	gtk_tree_view_set_model (tree,list_store);
}

void add_button(GtkWidget *button, gpointer data){
	gtk_widget_hide(((GtkWidget**)data)[2]);
	addWorkerWindow(((gpointer**) data)[1]);
	update(((gpointer**) data)[0],((gpointer**) data)[1],gtk_entry_get_text(((GtkWidget**) data)[3]));
	gtk_widget_show(((GtkWidget**)data)[2]);
}

void remove_button(GtkWidget *button, gpointer data){
	GtkTreeIter iter;
	GtkListStore* list_store;
	list_store=gtk_tree_view_get_model(((GtkWidget**)data)[0]);
	if (!gtk_tree_model_get_iter_first(list_store,&iter)) {
		GtkWidget *dialog = gtk_message_dialog_new (
								   ((GtkWidget**)data)[2],
								  GTK_DIALOG_MODAL,
                                  GTK_MESSAGE_WARNING,
                                  GTK_BUTTONS_CLOSE,
                                  "Не удалось удалить элемент\nПустой список сотрудников");
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
                                  "Не удалось удалить элемент\nСотрудник не выбран");
        gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		return;
	}
	GValue val = { 0, };
	gint num_columb;
	gtk_tree_model_get_value(list_store,&iter,0,&val);
	num_columb = g_value_get_int (&val);
	g_value_unset(&val);
	GtkWidget *dialog = gtk_message_dialog_new (
								  ((GtkWidget**)data)[2],
								  GTK_DIALOG_MODAL,
                                  GTK_MESSAGE_QUESTION,
                                  GTK_BUTTONS_YES_NO,
                                  "Вы действительно хотите удалить\nвыбранного сотрудника?");
	int result=gtk_dialog_run (GTK_DIALOG (dialog));
	if (result==-8){
		char* request=(char*)calloc(sizeof(char),50);
		sprintf(request,"DELETE FROM users WHERE id = %d",num_columb);
		fprintf(stderr,"%s\n",request);
		mysql_query( ((GtkWidget**)data)[1], request);
		free(request);
		update(((gpointer**) data)[0],((gpointer**) data)[1],gtk_entry_get_text(((GtkWidget**) data)[3]));
	}
	gtk_widget_destroy (dialog);
}

void edit_button(GtkWidget *button, gpointer data){
	GtkTreeIter iter;
	GtkListStore* list_store;
	list_store=gtk_tree_view_get_model(((GtkWidget**)data)[0]);
	if (!gtk_tree_model_get_iter_first(list_store,&iter)) {
		GtkWidget *dialog = gtk_message_dialog_new (
								   ((GtkWidget**)data)[2],
								  GTK_DIALOG_MODAL,
                                  GTK_MESSAGE_WARNING,
                                  GTK_BUTTONS_CLOSE,
                                  "Не удалось отредактировать элемент\nПустой список сотрудников");
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
                                  "Не удалось отредактировать элемент\nСотрудник не выбран");
        gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		return;
	}
	GValue val = { 0, };
	gint num_columb;
	gtk_tree_model_get_value(list_store,&iter,0,&val);
	num_columb = g_value_get_int (&val);
	g_value_unset(&val);
	editWorkerWindow(num_columb,((GtkWidget**)data)[1]);
	update(((gpointer**) data)[0],((gpointer**) data)[1],gtk_entry_get_text(((GtkWidget**) data)[3]));
}

