#include"helperWindows.h"
//#include"addWorkerWindow.h"
void edit_button_worker(GtkWidget *button, gpointer data);
void add_men(GtkWidget *button, gpointer data);
void add_girl(GtkWidget *button, gpointer data);
void editWorkerWindow(int number,MYSQL* sql){
	GtkWidget *window;
	GtkWidget *table;
	GtkWidget *lLastname,*lFirstname,*lPatronymic,*lPassport,*lRegistration,*lEducation,*lDateOfBirth,*lGender,*lPlace;
	GtkWidget *eLastname,*eFirstname,*ePatronymic,*ePassport,*eRegistration,*eEducation,*ePlace;
	GtkButton *edit;
	GtkWidget *men,*girl;
	char* gender=(char*)calloc(10,sizeof(char));
	gender[0]='m';
	GtkCalendar *calendar;
	GtkWidget *tableGender;
  
	gtk_init(NULL, NULL);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_window_set_title(GTK_WINDOW(window), "Редактирование работника");
	gtk_window_set_default_size (GTK_WINDOW (window), 400, 150);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_icon_from_file (GTK_WINDOW(window), "img/icon.png", NULL);
	table = gtk_table_new (3, 10, FALSE);
	tableGender= gtk_table_new (2, 1, FALSE);
	lLastname=gtk_label_new("Фамилия*");
	lFirstname=gtk_label_new("Имя*");
	lPatronymic=gtk_label_new("Отчество*");
	lDateOfBirth=gtk_label_new("Дата рождения*");
	lGender=gtk_label_new("Пол*");
	lPassport=gtk_label_new("Паспорт*");
	lRegistration=gtk_label_new("Регистрация");
	lEducation=gtk_label_new("Образование");
	lPlace=gtk_label_new("Место работы*");
	
	eLastname=gtk_entry_new();
	eFirstname=gtk_entry_new();
	ePatronymic=gtk_entry_new();
	ePassport=gtk_entry_new();
	eRegistration=gtk_entry_new();
	eEducation=gtk_entry_new();
	ePlace=gtk_entry_new();
	gtk_entry_set_max_length(eLastname,25);
	gtk_entry_set_max_length(eFirstname,25);
	gtk_entry_set_max_length(ePatronymic,25);
	gtk_entry_set_max_length(ePassport,10);
	gtk_entry_set_max_length(ePassport,10);
	gtk_entry_set_max_length(eRegistration,255);
	gtk_entry_set_max_length(eEducation,255);
	gtk_entry_set_max_length(ePlace,255);
	men=gtk_radio_button_new_with_label (NULL,"Муж.");
    girl=gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (men),"Жен.");
    calendar= gtk_calendar_new();
    
	edit = gtk_button_new_with_label("Изменить");
	
	gtk_table_attach(table,lLastname,0,1,0,1,GTK_SHRINK,GTK_FILL,0,10);
	gtk_table_attach(table,lFirstname,0,1,1,2,GTK_SHRINK,GTK_EXPAND,0,10);
	gtk_table_attach(table,lPatronymic,0,1,2,3,GTK_SHRINK,GTK_EXPAND,0,10);
	gtk_table_attach(table,lDateOfBirth,0,1,3,4,GTK_SHRINK,GTK_EXPAND,0,10);
	gtk_table_attach(table,lGender,0,1,4,5,GTK_SHRINK,GTK_EXPAND,0,10);
	gtk_table_attach(table,lPassport,0,1,5,6,GTK_SHRINK,GTK_EXPAND,0,10);
	gtk_table_attach(table,lRegistration,0,1,6,7,GTK_SHRINK,GTK_EXPAND,0,10);
	gtk_table_attach(table,lEducation,0,1,7,8,GTK_SHRINK,GTK_EXPAND,0,10);
	gtk_table_attach(table,lPlace,0,1,8,9,GTK_SHRINK,GTK_EXPAND,0,10);
	
	gtk_table_attach(table,eLastname,1,3,0,1,GTK_EXPAND,GTK_EXPAND,0,10);
	gtk_table_attach(table,eFirstname,1,3,1,2,GTK_EXPAND,GTK_EXPAND,0,10);
	gtk_table_attach(table,ePatronymic,1,3,2,3,GTK_EXPAND,GTK_EXPAND,0,10);
	gtk_table_attach(table,ePassport,1,3,5,6,GTK_EXPAND,GTK_EXPAND,0,10);
	gtk_table_attach(table,eRegistration,1,3,6,7,GTK_EXPAND,GTK_EXPAND,0,10);
	gtk_table_attach(table,eEducation,1,3,7,8,GTK_EXPAND,GTK_EXPAND,0,10);
	gtk_table_attach(table,ePlace,1,3,8,9,GTK_EXPAND,GTK_EXPAND,0,10);
	gtk_table_attach(table,calendar,1,3,3,4,GTK_SHRINK,GTK_EXPAND,0,10);
	gtk_table_attach(tableGender,men,0,1,0,1,GTK_EXPAND,0,0,0);
	gtk_table_attach(tableGender,girl,1,2,0,1,GTK_EXPAND,0,0,0);
	gtk_table_attach(table,tableGender,1,3,4,5,GTK_FILL,0,0,0);
	
	gtk_table_attach(table,edit,1,2,9,10,GTK_FILL,GTK_FILL,0,10);
	
	gtk_container_add (GTK_CONTAINER (window), table);
	gtk_widget_show_all(window);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gpointer** data=(gpointer**)malloc(sizeof(GtkWidget*)*9+sizeof(char*) + sizeof(MYSQL*) +sizeof(int*));
	data[0]=eLastname;
	data[1]=eFirstname;
	data[2]=ePatronymic;
	data[3]=ePassport;
	data[4]=eRegistration;
	data[5]=calendar;
	data[6]=gender;
	data[7]=eEducation;
	data[8]=sql;
	data[9]=window;
	data[10]=ePlace;
	data[11]=&number;
	//fprintf(stderr, "%d\n", *((int*)(*data[11])));
	//g_signal_connect(GTK_BUTTON(logIn), "clicked", G_CALLBACK(logIn_button), data);
	g_signal_connect(GTK_BUTTON(edit), "clicked",G_CALLBACK(edit_button_worker), data);
	g_signal_connect(GTK_RADIO_BUTTON(men), "clicked",G_CALLBACK(add_men), gender);
	g_signal_connect(GTK_RADIO_BUTTON(girl), "clicked",G_CALLBACK(add_girl), gender);
	
	char* request=(char*)calloc(256,sizeof(char));
    sprintf(request,"SELECT * FROM users WHERE id = %d",number);
    mysql_query(sql, request);
    free(request);
    
    MYSQL_RES *result = mysql_store_result(sql);
  	if (result == NULL) 
  	{
		fprintf(stderr, "%s\n", mysql_error(sql));
      	exit(0);
  	}
	MYSQL_ROW row;
	
	while ((row = mysql_fetch_row(result))){ 
		gtk_entry_set_text(eLastname,row[1]);
		gtk_entry_set_text(eFirstname,row[2]);
		gtk_entry_set_text(ePatronymic,row[3]);
		if (strcmp(row[5],"g")==0) gtk_button_clicked(girl);
		
		//gtk_entry_set_text(eLastname,row[4]); data
		gtk_entry_set_text(ePassport,row[6]);
		gtk_entry_set_text(eRegistration,row[7]);
		gtk_entry_set_text(eEducation,row[8]);
		gtk_entry_set_text(ePlace,row[9]);
	}
	
	gtk_main();
}

void edit_button_worker(GtkWidget *button, gpointer data){

	//fprintf(stderr, "%d\n", *(((int**)data)[11]));
	char* lastName = gtk_entry_get_text(((GtkWidget**)data)[0]);
	char* firstName = gtk_entry_get_text(((GtkWidget**)data)[1]);
	char* patronymic = gtk_entry_get_text(((GtkWidget**)data)[2]);
	char* passport = gtk_entry_get_text(((GtkWidget**)data)[3]);
	char* registration = gtk_entry_get_text(((GtkWidget**)data)[4]);
	char* education = gtk_entry_get_text(((GtkWidget**)data)[7]);
	char* place=gtk_entry_get_text(((GtkWidget**)data)[10]);
	char* gender=((gpointer**)data)[6];
	char* dateOfBirth=(char*)calloc(9,sizeof(char));
	int year,day,month;
	if (strstr(lastName,"")==0||strstr(firstName,"")==0||strstr(patronymic,"")==0||strstr(passport,"")==0||strstr(place,"")==0){
		GtkWidget *dialog = gtk_message_dialog_new (
								  ((GtkWidget**)data)[2],
								  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_WARNING,
                                  GTK_BUTTONS_CLOSE,
                                  "Добавление не было произведено\nНе заполенены обязательные поля");
	  	gtk_dialog_run (GTK_DIALOG (dialog));
 	  	gtk_widget_destroy (dialog);
		return;
	}
	gtk_calendar_get_date (((GtkWidget**)data)[5],&year,&month,&day);
	if (year>2003){
		GtkWidget *dialog = gtk_message_dialog_new (
								  ((GtkWidget**)data)[2],
								  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_WARNING,
                                  GTK_BUTTONS_CLOSE,
                                  "Добавление не было произведено\nРаботнику долно быть не меньше 16");
	  	gtk_dialog_run (GTK_DIALOG (dialog));
 	  	gtk_widget_destroy (dialog);
		return;
	}
	sprintf(dateOfBirth,"%d%0.2d%0.2d",year,month+1,day);
	int size=strlen(lastName)+strlen(firstName)+strlen(patronymic)+strlen(passport)+strlen(registration)+strlen(education)+strlen(gender)+strlen(dateOfBirth)*strlen(place);
	char* request=(char*)calloc(size+512,sizeof(char));
	
	sprintf(request,"UPDATE users SET lastname ='%s',firstname = '%s',patronymic = '%s',dateOfBirth = '%s',gender = '%s',passport ='%s',registration='%s',education='%s',place='%s' WHERE id = %d",lastName,firstName,patronymic,dateOfBirth,gender,passport,registration,education,place,*(((int**)data)[11]));
	if(!mysql_query(((MYSQL**)data)[8], request)) fprintf(stderr, "%s\n", mysql_error(((MYSQL**)data)[8]));
	free(request);
	
	gtk_widget_destroy (((GtkWidget**)data)[9]);
}

