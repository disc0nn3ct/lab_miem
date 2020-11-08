#include <stdio.h>
#include <sys/ipc.h> // Разделяемая область память (РОП)
#include <sys/shm.h> 
#include <unistd.h>   //Для _exit 
#include <sys/sem.h> // для семофор 	
#include <string.h>
#include <stdlib.h>


///////////////////////////////////////////////////
// для сообщений 
#include <sys/msg.h> // для msgget

#ifndef MSGMAX
#define MSGMAX 1024
#endif 

struct msgbuf {
long mtype;
char text[MSGMAX];
};
//////////////////////////////////////////////////


struct sembuf nbuf1[2] = {{0, -1, IPC_NOWAIT}, {1, -1, IPC_NOWAIT}};
struct sembuf nbuf2[2] = {{0, 2, IPC_NOWAIT}, {1, 2, IPC_NOWAIT}};
struct sembuf zeoro[1] = {{2, -1, IPC_NOWAIT}};

union semun 
{ int val;
   struct semid_ds  *sbuf;
   ushort  *array;
} arg;


int str_to_int(char* stri)
{
	int k = atoi(stri); 

	return k;
}


void Finding_liver(char* addr, int* minPrior, int* PID)
{
char buffer[100];


	char cmd[1200] = "echo \"";
	strcat(cmd, addr);
	cmd[strlen(cmd)-1]=' ';
	strcat(cmd, "\" | awk '{print $7}'");


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
	   int k = str_to_int(buffer);

	   if(k > *minPrior )
	   { 
	   	*minPrior = k;
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
	   }
}





int main()
{
	int fd, fd_sem; 
	char buffer[1024];
	fd = shmget(100, 0, 0 );
	char* addr; 
	addr = (char*)(shmat(fd, 0, 0 ));
	struct semid_ds buf;	
	arg.sbuf = &buf;


	fd_sem = semget( 100, 0, 0 );  // откроем набор семафор 
	if ( fd_sem == -1 ) { printf( "Ошибка в semget\n" ); _exit( 1 ); }


	int PID=0, minPrior = 0; 
	while(semop(fd_sem, zeoro, 1 ) == -1)
	{
		if (semop(fd_sem, nbuf1, 2) != -1)
		{
		printf("%s", addr);
		Finding_liver(addr, &minPrior, &PID);
		semop(fd_sem, nbuf2, 2);
		}
	}




	printf("=====================================================\n");

	if (semctl( fd_sem, 0, IPC_STAT, arg) == 0)
	{
		printf("Rоличество семафоров в наборе: %ld\n", arg.sbuf -> sem_nsems );
	}
	printf("Минимальный приоритет %d у PID %d \n",minPrior, PID);



	if ( shmdt( addr ) == -1 ) perror("shmdt");
////////////////////////////////////////////////////	

	int fd123 = msgget(112, IPC_CREAT | 0666 );

	struct msgbuf buf12;
	struct msqid_ds bufxx;
	buf12.mtype = 16;
	sprintf(buf12.text, "Минимальный приоритет %d у PID %d И Rоличество семафоров в наборе: %ld\n", minPrior, PID, arg.sbuf -> sem_nsems );
	if (fd123 == -1 || msgsnd(fd123, &buf12, strlen(buf12.text)+1, IPC_NOWAIT) == -1 )
		perror("Ошибка сообщения"); 



	return 0;
}




