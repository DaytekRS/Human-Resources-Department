#include <gtk/gtk.h> /* подключаем GTK+ */
#include "windows/helperWindows.h"
#include <my_global.h>
#include <mysql.h> 



int main( int argc, char *argv[])
{
	MYSQL *con=mysql_init(NULL);
	if (con == NULL) 
  	{
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  	}
    authorizationWindow(con);

    return 0;
}
