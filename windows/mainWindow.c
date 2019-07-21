#include"helperWindows.h"
void worker_button(GtkWidget *button, gpointer data);
void statement_button(GtkWidget *button, gpointer data);
void graphic_button(GtkWidget *button, gpointer data);
void order_button(GtkWidget *button, gpointer data);
void mainWindow(MYSQL *sql){
	GtkWidget *window;
	GtkWidget *users;
	GtkWidget *graphic;
	GtkWidget *order;
	GtkWidget *statement;
	GtkWidget *vbox;
	GtkTreeIter iter;
	gtk_init(NULL, NULL);
	vbox = gtk_vbox_new (TRUE, 5);
	users = gtk_button_new_with_label("Записи о работниках");
	order = gtk_button_new_with_label("Создание приказа");
	graphic = gtk_button_new_with_label("Создание графика отпуска");
	statement = gtk_button_new_with_label("Формирование ведомости");
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	
	
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_window_set_title(GTK_WINDOW(window), "Главное меню");
	gtk_window_set_default_size (GTK_WINDOW (window), 300, 150);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_icon_from_file (GTK_WINDOW(window), "img/icon.png", NULL);

	gtk_box_pack_start (GTK_BOX (vbox), users, TRUE, 0, 5);
	gtk_box_pack_start (GTK_BOX (vbox), graphic, TRUE, 0, 5);
	gtk_box_pack_start (GTK_BOX (vbox), order, TRUE, 0, 5);
	gtk_box_pack_start (GTK_BOX (vbox), statement, TRUE, 0, 5);
	
	GtkWidget **data=(GtkWidget**)malloc(sizeof(GtkWidget*)+sizeof(MYSQL*));
	data[0]=window;
	data[1]=sql;
	g_signal_connect(GTK_BUTTON(users), "clicked", G_CALLBACK(worker_button), data);
	g_signal_connect(GTK_BUTTON(statement), "clicked", G_CALLBACK(statement_button), data);
	g_signal_connect(GTK_BUTTON(graphic), "clicked", G_CALLBACK(graphic_button), data);
	g_signal_connect(GTK_BUTTON(order), "clicked", G_CALLBACK(order_button), data);
	gtk_container_add (GTK_CONTAINER (window), vbox);
	gtk_widget_show_all(window);
	g_signal_connect(GTK_WINDOW(window), "delete_event",G_CALLBACK(close_window), sql);
	gtk_main();
}

void worker_button(GtkWidget *button, gpointer data)
{
	gtk_widget_hide(((GtkWidget**)data)[0]);
	workerWindow(((GtkWidget**)data)[1]);
	gtk_widget_show(((GtkWidget**)data)[0]);
}

void statement_button(GtkWidget *button, gpointer data)
{
	gtk_widget_hide(((GtkWidget**)data)[0]);
	statementWindow(((GtkWidget**)data)[1]);
	gtk_widget_show(((GtkWidget**)data)[0]);
}

void graphic_button(GtkWidget *button, gpointer data)
{
	gtk_widget_hide(((GtkWidget**)data)[0]);
	graphicWindow(((GtkWidget**)data)[1]);
	gtk_widget_show(((GtkWidget**)data)[0]);
}

void order_button(GtkWidget *button, gpointer data)
{
	gtk_widget_hide(((GtkWidget**)data)[0]);
	orderWindow();
	gtk_widget_show(((GtkWidget**)data)[0]);
}

