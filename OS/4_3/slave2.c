// Клиент 2. Клиент 2. Создать гнездо домена UNIX типа virtual circuit. Послать в серверное гнездо сообщение, содержащее имена всех файлов текущего каталога, созданных за последние 3 дня.

#include <stdio.h>
#include <unistd.h> // for close  из мана
#include <sys/socket.h>
#include <sys/un.h> // для sockaddr_un 
#include <stdlib.h> // для exit 

int main()
{
	char cmd[] = "find . -mtime -3 -type f -print | cut -c 3- | tr \"\n\" \" \" "; // файлы созданные за последние 3 дня.
	// для проверки создадим файл с ложной датой:	 touch -mad "1998-07-08 12:15:25" test 
	char buffer[1024];
	FILE *f= popen (cmd, "r");
	if (f == NULL) perror ("Ошибка открытия файла");
	else
	{
	while ( !feof(f) )    						    // пока не конец файла                           
	{
		if ( fgets(buffer , 1024 , f) != NULL )       // считать символы из файла 
			printf("%s",buffer);                       
	}
	fclose (f);             						// закрыть файл                         
	}
	
///////////////////////////////////	
	
   	struct sockaddr_un serv_addr;


    	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd < 0) 
	{
		perror("ERROR opening socket");
		exit(1);
	}

    	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, "/tmp/omg.sock");	

	if (connect(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
	{
		perror("ERROR connecting");
		exit(1);
	}
	
	int n = write(fd, buffer, strlen(buffer));
	if (n < 0) 
	{
		perror("ERROR writing to socket");
		exit(1);
	}

	printf("\n");
    	close(fd);
	return 0;
}
