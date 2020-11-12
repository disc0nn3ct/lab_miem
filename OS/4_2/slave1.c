#include <stdio.h>
#include <sys/ipc.h> // Разделяемая область память (РОП)
#include <sys/shm.h> 
#include <unistd.h>   //Для _exit 
#include <sys/sem.h> // для семофор 	
#include <string.h>
#include <stdlib.h>


struct sembuf nbuf1[2] = {{0, -1, IPC_NOWAIT}, {1, 0, IPC_NOWAIT}};
struct sembuf nbuf2[2] = {{0, 1, IPC_NOWAIT}, {1, 1, IPC_NOWAIT}};
struct sembuf zeoro[1] = {2, -1, IPC_NOWAIT};
struct sembuf zeoro1[1] = {2, -1, 0}; // для посылки сообщения :) 


int str_to_int(char* stri)
{
	int k = atoi(stri); 

	return k;
}


void Finding_liver(char* addr, int* maxSec, int* PID, char* state)
{

	char buffer[100];


	char cmd[1200] = "echo \"";
	strcat(cmd, addr);
	cmd[strlen(cmd)-1]=' ';
	strcat(cmd, "\" | awk '{print $14}'");


	FILE *f= popen (cmd, "r");

	if (f == NULL) perror ("Ошибка открытия файла");
	else
	   {
	     while ( !feof(f) )    // пока не конец файла                           
	     {
	       if ( fgets(buffer , 100 , f) != NULL )     // считать символы из файла 
	        ;                      
	     }
	     fclose (f);             // закрыть файл                         
	   }

	int sec=0;
	sec+= str_to_int(&buffer[0])*3600 + str_to_int(&buffer[3])*60 + str_to_int(&buffer[6]);



	if(*maxSec < sec)
	{
		*maxSec = sec; 
		strcpy(cmd, "echo \"");
		strcat(cmd, addr);
		cmd[strlen(cmd)-1]=' ';
		strcat(cmd, "\" | awk '{print $4}'");
		f= popen (cmd, "r");
		if (f == NULL) perror ("Ошибка открытия файла");
		else
		   {
		     while ( !feof(f) )    // пока не конец файла                           
		     {
		       if ( fgets(buffer, 100 , f) != NULL )     // считать символы из файла 
		        ;                      
		     }
		     fclose (f);             // закрыть файл                         
		   }
		*PID = str_to_int(buffer);
		strcpy(cmd, "echo \"");
		strcat(cmd, addr);
		cmd[strlen(cmd)-1]=' ';
		strcat(cmd, "| grep ");
		strcat(cmd, buffer);
		strcat(cmd, "\" | awk '{print $2}' | head -1");
		f= popen (cmd, "r");
		if (f == NULL) perror ("Ошибка открытия файла");
		else
		{
		    while ( !feof(f) )    // пока не конец файла                           
		    {
		       if ( fgets(buffer, 100 , f) != NULL )     // считать символы из файла 
		        ;                      
		    }
		    fclose (f);             // закрыть файл                         
		}
	strcpy(state, buffer);	   

	}



}

int main()
{
	char* state;
	int maxSec = -1, PID = -1; 

	int fd, fd_sem; 
	char buffer[1024];
	fd = shmget(100, 0, 0 );
	char* addr; 
	addr = (char*)(shmat(fd, 0, 0 ));

	fd_sem = semget( 100, 0, 0 );  // откроем набор семафор 
	if ( fd_sem == -1 ) { printf( "Ошибка в semget\n" ); _exit( 1 ); }

	while(semop(fd_sem, zeoro, 1 )== -1)
	{
		if (semop(fd_sem, nbuf1, 2) != -1)
		{
			printf("%s", addr);
			Finding_liver(addr, &maxSec, &PID, state);
			semop(fd_sem, nbuf2, 2);
		}

	}
//////////////////////////////////////////////////////////////////////////////



	printf("=====================================================\n");
	printf("Сколько секунд жил %d PID = %d Cостояние = %s\n", maxSec, PID, state);

	if (semop(fd_sem, zeoro1, 1) == -1 )
	{
		printf("%s\n", "Error");
	}
	sprintf(addr, "Процесс долгожитель жил %d PID = %d Cостояние = %s\n", maxSec, PID, state);
	// strcpy(addr, "LOL");  
	semop(fd_sem, zeoro1, 1 );




	if ( shmdt( addr ) == -1 ) perror("shmdt");   // need 


	return 0;
}




