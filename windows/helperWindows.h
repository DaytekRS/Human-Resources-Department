#include <gtk/gtk.h> 
#include <my_global.h>
#include <mysql.h> 
#include"../event/event.h"
void alert(char* title,char* str);
void authorizationWindow(MYSQL *sql);
void mainWindow(MYSQL *sql);
void workerWindow(MYSQL* sql);
void addWorkerWindow(MYSQL* sql);
void editWorkerWindow(int number,MYSQL* sql);
void statementWindow(MYSQL* sql);
void graphicWindow(MYSQL* sql);
void addGraphicWindow(MYSQL* sql);
void orderWindow();

