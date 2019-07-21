#include"helperWindows.h"
void alert(char* title,char* str){
	GtkWidget *window; //окно
	GtkWidget *imgAlert; //изображение 
	GtkWidget *label; 
	GtkWidget *table;
	gtk_init(0,NULL);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	imgAlert = gtk_image_new();
	gtk_image_set_from_file(GTK_IMAGE(imgAlert),"../img/alert.png");
	table = gtk_table_new (1, 2, FALSE);
	label = gtk_label_new(str);
    
	
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_window_set_title(GTK_WINDOW(window), title);
	gtk_window_set_default_size (GTK_WINDOW (window), 300, 115);	
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	
	gtk_table_attach(GTK_TABLE(table),imgAlert,0,1,0,1,GTK_SHRINK,GTK_SHRINK,0,10);
	gtk_table_attach(GTK_TABLE(table),label,1,2,0,1,GTK_FILL,GTK_FILL,20,0);

	gtk_container_add(GTK_CONTAINER(window), table);
		
	gtk_widget_show_all(window);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_main();
	return 0;
}
