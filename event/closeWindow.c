#include"event.h";
gint close_window(GtkWidget * widget, GdkEvent * event, gpointer data)
{
	mysql_close((MYSQL*)data);
	gtk_main_quit ();
	return (FALSE);
}

