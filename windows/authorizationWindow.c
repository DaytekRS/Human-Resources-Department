#include"helperWindows.h"

void logIn_button(GtkWidget *button, gpointer data);

void authorizationWindow(MYSQL *sql){
/* Объявляем виджеты */
    GtkWidget *label;   // Метка
    GtkWidget *lLogin,*lPassword;  // надписи логин и пароль
	GtkWidget *eLogin,*ePassword; //поля для ввода логина и пароля
	GtkWidget *table; //таблица расположения
	GtkWidget *logIn;
	GtkWidget *window;
   //окно программы
 /* Инициализируем GTK+ */
    gtk_init(NULL, NULL);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	table = gtk_table_new (2, 5, FALSE);
    gtk_window_set_title(GTK_WINDOW(window), "Отдел кадров");
	gtk_window_set_default_size (GTK_WINDOW (window), 300, 150);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_icon_from_file (GTK_WINDOW(window), "img/icon.png", NULL);
   
	label = gtk_label_new("Авторизация");
    lLogin = gtk_label_new("Логин: ");
	lPassword = gtk_label_new("Пароль: ");
	
	eLogin = gtk_entry_new();
	ePassword = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(ePassword), FALSE);
    
	logIn = gtk_button_new_with_label("Войти");
	GtkWidget **data=(GtkWidget**)malloc(sizeof(GtkWidget*)*3+sizeof(MYSQL*));
	data[0]=eLogin;
	data[1]=ePassword;
	data[2]=window;
	data[3]=sql;
	g_signal_connect(GTK_BUTTON(logIn), "clicked", G_CALLBACK(logIn_button), data);

    gtk_table_attach(table,label,0,2,0,1,GTK_EXPAND,GTK_EXPAND,0,10);
	gtk_table_attach(table,lLogin,0,1,1,2,GTK_EXPAND,GTK_EXPAND,0,5);
	gtk_table_attach(table,lPassword,0,1,2,3,GTK_EXPAND,GTK_EXPAND,0,5);
    gtk_table_attach(table,eLogin,1,2,1,2,GTK_EXPAND,GTK_EXPAND,0,5);
	gtk_table_attach(table,ePassword,1,2,2,3,GTK_EXPAND,GTK_EXPAND,0,5);
	
	gtk_table_attach(table,logIn,1,2,3,4,GTK_FILL,GTK_FILL,50,5);
	gtk_container_add(GTK_CONTAINER(window), table);
	
    /* Показываем окно вместе с виджетами */
    gtk_widget_show_all(window);

    /* Соединяем сигнал завершения с выходом из программы */
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Приложение переходит в вечный цикл ожидания действий пользователя */
    gtk_main();
}


void logIn_button(GtkWidget *button, gpointer data)
{
	MYSQL *con=((GtkWidget**)data)[3];
	GtkWidget *login=((GtkWidget**)data)[0];
	GtkWidget *password=((GtkWidget**)data)[1];
	char *tLogin=gtk_entry_get_text(login),
		 *tPassword=gtk_entry_get_text(password);
	

	if (mysql_real_connect(con, "localhost",tLogin, tPassword, 
          "PersonnelDepartment", 0, NULL, 0) == NULL) 
  	{

	  GtkWidget *dialog = gtk_message_dialog_new (
								  ((GtkWidget**)data)[2],
								  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_WARNING,
                                  GTK_BUTTONS_CLOSE,
                                  "Вход не был выполнен\nПроверьте введенный логин и пароль");
	  gtk_dialog_run (GTK_DIALOG (dialog));
 	  gtk_widget_destroy (dialog);
      return;
  	}  
	/*if (mysql_query(con, "INSERT INTO users VALUES(NULL,'A','B',NULL,NULL,NULL,NULL,NULL,NULL)")) {
       fprintf(stderr, "%s\n", mysql_error(con));
    }*/
 	  gtk_widget_destroy (((GtkWidget**)data)[2]);
	  mainWindow(con);
}
