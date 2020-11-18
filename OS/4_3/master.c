// Вариант №18  Сервер. Создать гнездо домена UNIX типа virtual circuit. Назначить ему адрес. 
// Среди поступивших от клиентов сообщений, отфильтровать такие, которые имеются и в том и в другом списке.  
// Клиент 1. Создать гнездо домена UNIX типа virtual circuit. Послать в серверное гнездо сообщение, 
// содержащее имена всех файлов текущего каталога, размер которых не превышает 100 байтов.  
// Клиент 2. Создать гнездо домена UNIX типа virtual circuit. 
// Послать в серверное гнездо сообщение, содержащее имена всех файлов текущего каталога, созданных за последние 3 дня.


#include <stdio.h>
#include <sys/socket.h>
#include <strings.h> // для bzero
#include <netdb.h> // локальные адреса, библиотечные функции для локальных адресов.  для bzero 
#include <stdlib.h> // для exit 
#include <string.h>
#include <sys/un.h> // для sockaddr_un 
#include <unistd.h> // for close  из мана



void unique(char *arr1, char* arr2)
{
	// printf("%s\n", arr1 );
	// printf("%s\n", arr2 );
	char buffer[2048];
	char cmd[2300] = "echo ";
	strcat(cmd, arr1);
	strcat(cmd, " ");
	strcat(cmd, arr2);
	strcat(cmd, " | tr \" \" \"\n\" | sort | uniq -u | tr \"\n\" \" \" ");
	
	FILE *f= popen (cmd, "r");
	if (f == NULL) perror ("Ошибка открытия файла");
	else
	{
	while ( !feof(f) )    						    // пока не конец файла                           
	{
		if ( fgets(buffer , 1024 , f) != NULL )       // считать символы из файла 
			printf("%s\n",buffer);                       
	}
	fclose (f);             						// закрыть файл                         
	}

}






int main()
{
	struct sockaddr_un addr;	
	bzero( ( void *)&addr, sizeof( addr )); // почистили память 
	addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/tmp/omg.sock");

	int fd = socket( AF_UNIX, SOCK_STREAM, 0); // сначала указывается семейство адресов
	if (fd < 0) 
	{
		perror("ERROR opening socket");
		exit(1);
	}

	if ( bind( fd, ( struct sockaddr *)&addr, sizeof( addr ) ) != 0) printf( "ошибка bind\n" );
	if ( listen( fd, 0 ) != 0 ) printf( "ошибка listen\n" ); 
	int fd1 = accept( fd, 0, 0 ); 


	char buffer[1024];
	bzero(buffer, sizeof(buffer));
	recv( fd1, buffer, sizeof( buffer ), 0 );
	// printf("%s\n", buffer);


	char buffer1[1024];
	int fd2 = accept( fd, 0, 0 ); 
	bzero(buffer1, sizeof(buffer1));
	recv( fd2, buffer1, sizeof( buffer1 ), 0 );
	// printf("%s\n", buffer1);

	unique(buffer, buffer1);



	close( fd1 );
	close( fd );
	remove( "/tmp/omg.sock" );
	/* code */
	return 0;
}