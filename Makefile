main: main.o windows/helperWindows.o event/event.o
	  gcc -o main main.o windows/helperWindows.o event/event.o `pkg-config --cflags --libs gtk+-3.0` `mysql_config --cflags --libs`

windows/helperWindows.o: windows/alertWindow.c windows/authorizationWindow.c windows/mainWindow.c
				 gcc -o windows/alertWindow.o -c windows/alertWindow.c `pkg-config --cflags --libs gtk+-3.0` `mysql_config --cflags --libs`
				 gcc -o windows/authorizationWindow.o -c windows/authorizationWindow.c `pkg-config --cflags --libs gtk+-3.0` `mysql_config --cflags --libs`
				 gcc -o windows/mainWindow.o -c windows/mainWindow.c `pkg-config --cflags --libs gtk+-3.0` `mysql_config --cflags --libs`
				 gcc -o windows/workerWindow.o -c windows/workerWindow.c `pkg-config --cflags --libs gtk+-3.0` `mysql_config --cflags --libs`
				 gcc -o windows/addWorkerWindow.o -c windows/addWorkerWindow.c `pkg-config --cflags --libs gtk+-3.0` `mysql_config --cflags --libs`
				 gcc -o windows/editWorkerWindow.o -c windows/editWorkerWindow.c `pkg-config --cflags --libs gtk+-3.0` `mysql_config --cflags --libs`
				 gcc -o windows/statementWindow.o -c windows/statementWindow.c `pkg-config --cflags --libs gtk+-3.0` `mysql_config --cflags --libs`
				 gcc -o windows/graphicWindow.o -c windows/graphicWindow.c `pkg-config --cflags --libs gtk+-3.0` `mysql_config --cflags --libs`
				 gcc -o windows/addGraphicWindow.o -c windows/addGraphicWindow.c `pkg-config --cflags --libs gtk+-3.0` `mysql_config --cflags --libs`
				 gcc -o windows/orderWindow.o -c windows/orderWindow.c `pkg-config --cflags --libs gtk+-3.0` `mysql_config --cflags --libs`
				 ld  -r windows/orderWindow.o windows/addGraphicWindow.o windows/graphicWindow.o windows/statementWindow.o windows/editWorkerWindow.o windows/alertWindow.o windows/authorizationWindow.o windows/mainWindow.o windows/workerWindow.o windows/addWorkerWindow.o -o windows/helperWindows.o
event/event.o: event/closeWindow.c
				gcc -c event/closeWindow.c -o event/closeWindow.o `pkg-config --cflags --libs gtk+-3.0` `mysql_config --cflags --libs`	
				ld -r event/closeWindow.o -o event/event.o
main.o: main.c
		gcc -c main.c `pkg-config --cflags --libs gtk+-3.0` `mysql_config --cflags --libs`
clean: 
	  rm -f main *.o windows/*.o event/*.o windows/a.out
runWorker:
		gcc windows/workerWindow.c event/event.o `pkg-config --cflags --libs gtk+-3.0` `mysql_config --cflags --libs`
		
