#include"helperWindows.h"
void click_dismissal(GtkWidget *button, gpointer data){
	char *text="Наименование организации\nПРИКАЗ\n________________ N _______________\nМесто издания\nОб увольнении с работы\nУВОЛИТЬ (ОСВОБОДИТЬ ОТ ЗАНИМАЕМОЙ ДОЛЖНОСТИ):\n _______________________________________________________________\n                  ФАМИЛИЯ, имя, отчество работника\n____________________________________________________________________\n                           дата увольнения\nОснование:\nНаименование должности\n\nруководителя организации        Подпись        Расшифровка подписи\n\nС приказом ознакомлен:              Подпись        Расшифровка подписи\n                                              Дата\0";
	gtk_text_buffer_set_text(data,text,-1);
}
void click_toWork(GtkWidget *button, gpointer data){
	char *text="Наименование организации\nПРИКАЗ\n________________ N _______________\nМесто издания\nОб найме на работу\nПРИНЯТЬ (НАЗНАЧИТЬ НА ДОЛЖНОСТЬ):\n _______________________________________________________________\n                  ФАМИЛИЯ, имя, отчество работника\n____________________________________________________________________\n                           дата найма\nНаименование должности\n\nруководителя организации        Подпись        Расшифровка подписи\n\nС приказом ознакомлен:              Подпись        Расшифровка подписи\n                                              Дата\0";
	gtk_text_buffer_set_text(data,text,-1);
}
void click_toPlace(GtkWidget *button, gpointer data){
	char *text="Наименование организации\nПРИКАЗ\n________________ N _______________\nМесто издания\nОб изменении должности\nПЕРЕВЕСТИ (НАЗНАЧИТЬ НА ДОЛЖНОСТЬ):\n _______________________________________________________________\n                  ФАМИЛИЯ, имя, отчество работника\n____________________________________________________________________\n                           дата перевода\nНаименование должности\n\nруководителя организации        Подпись        Расшифровка подписи\n\nС приказом ознакомлен:              Подпись        Расшифровка подписи\n                                              Дата\0";
	gtk_text_buffer_set_text(data,text,-1);
}
void click_other(GtkWidget *button, gpointer data){
	char *text="\0";
	gtk_text_buffer_set_text(data,text,-1);
}
void save_order(GtkWidget *button, gpointer data){
	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new ("Save",
                                      ((GtkWidget**)data)[0],
                                      GTK_FILE_CHOOSER_ACTION_SAVE,
                                      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                      NULL);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		GtkTextIter start,end;
		gtk_text_buffer_get_start_iter(((GtkTextBuffer**)data)[1],&start);
		gtk_text_buffer_get_end_iter(((GtkTextBuffer**)data)[1],&end);
		char *text=gtk_text_buffer_get_text(((GtkTextBuffer**)data)[1],&start,&end,FALSE);
		char *filename;

    	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    	char *filedirectory=(char*)calloc(sizeof(char),strlen(filename)+5);
    	sprintf(filedirectory,"%s%s",filename,".txt");
    	
    	FILE *file=fopen(filedirectory,"wb+");
    	free(filedirectory);
    	fprintf(file,"%s",text);
    	fclose(file);
  	}
  	gtk_widget_destroy (dialog);
}
void orderWindow(){
	GtkWidget *table;
	GtkWidget *window;
	GtkWidget *scrolledList;
	GtkWidget *textView;
	GtkWidget *other,*toWork,*toPlace,*dismissal;
	GtkWidget *save;
	gtk_init(NULL, NULL);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_window_set_title(GTK_WINDOW(window), "Создание приказа");
	gtk_window_set_default_size (GTK_WINDOW (window), 400, 150);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_icon_from_file (GTK_WINDOW(window), "img/icon.png", NULL);
	
	other=gtk_radio_button_new_with_label (NULL,"Другое");
    toWork=gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (other),"О найме на работу");
    toPlace=gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (other),"О переводе на другую должность");
    dismissal=gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (other),"О увольнение");
    
    save = gtk_button_new_with_label("Сохранить");
    
	table = gtk_table_new (3, 5, FALSE);
	textView=gtk_text_view_new();
	
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(textView);
	//char *text="Privet\nPoka\0";
	//fprintf(stderr,"%s",text);
	//gtk_text_buffer_set_text(buffer,text,-1);
	scrolledList=gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy (scrolledList,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_min_content_width(scrolledList,500);
	gtk_scrolled_window_set_min_content_height(scrolledList,320);
	gtk_container_add (GTK_CONTAINER (scrolledList), textView);
	
	gtk_table_attach(table,scrolledList,0,3,0,1,GTK_FILL,GTK_FILL,15,0);
	
	gtk_table_attach(table,dismissal,0,1,1,2,GTK_FILL,GTK_FILL,15,0);
	gtk_table_attach(table,toWork,0,1,2,3,GTK_FILL,GTK_FILL,15,0);
	gtk_table_attach(table,toPlace,0,1,3,4,GTK_FILL,GTK_FILL,15,0);
	gtk_table_attach(table,other,0,1,4,5,GTK_FILL,GTK_FILL,15,0);
	gtk_table_attach(table,save,2,3,4,5,GTK_FILL,GTK_FILL,40,0);
	gtk_container_add (GTK_CONTAINER (window), table);
	gtk_widget_show_all(window);
	g_signal_connect(GTK_WINDOW(window), "delete_event",G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(GTK_RADIO_BUTTON(dismissal), "clicked",G_CALLBACK(click_dismissal), buffer);
	g_signal_connect(GTK_RADIO_BUTTON(toWork), "clicked",G_CALLBACK(click_toWork), buffer);
	g_signal_connect(GTK_RADIO_BUTTON(toPlace), "clicked",G_CALLBACK(click_toPlace), buffer);
	g_signal_connect(GTK_RADIO_BUTTON(other), "clicked",G_CALLBACK(click_other), buffer);
	gpointer** data=(gpointer**)malloc(sizeof(GtkWindow*)+sizeof(GtkTextBuffer*));
	data[0]=window;
	data[1]=buffer;
	g_signal_connect(GTK_BUTTON(save), "clicked",G_CALLBACK(save_order), data);
	gtk_main();
}


