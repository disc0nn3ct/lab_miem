// Вариант №18

// Сервер. Создать разделяемую область памяти (РОП) и набор семафоров(НС). Записать в РОП идентификаторы всех процессов, 
// принадлежащих данному пользователю, приоритеты этих процессов, суммарное рабочее время, затраченное этими процессами и имена
// задействованных терминалов. Сообщения, полученные от клиентов, распечатывать. В конце сеанса удалить РОП и НС.

// Клиент 1. Выбрать из РОП идентификатор процесса – «долгожителя» и определить текущее его состояние.

// Клиент 2. Выбрать из РОП идентификатор процесса с наименьшим приоритетом, а также определить количество семафоров в наборе.


#include <stdio.h>  
#include <unistd.h>   //Для _exit 

#include <string.h>
#include <sys/ipc.h> // Разделяемая область память (РОП)
#include <sys/shm.h> 

#include <sys/sem.h> // для семофор 	



struct sembuf first1[2] = {{0, -2, 0}, {1, -2, 0}};
// struct sembuf second1[3] = {{0, 1, 0}, {0,-3, 0}};
struct sembuf first[1] = {{2, 2, 0}};



union semun 
{ int val;
   struct semid_ds  *sbuf;
   ushort  *array;
} arg;


int main()
{
	char cmd[100] = "whoami | tr '\n' ' ' ";
	FILE *f= popen (cmd, "r");
	char buffer[1024];
	fgets(buffer, 1024 , f); 
	strcpy(cmd, "ps -l -fu"); 
	strcat(cmd, buffer); 

	f = popen (cmd, "r");


	// char buffer[1024];
	// char cmd[100] = "ps -l";
	// FILE *f= popen (cmd, "r");



	int fd, fd_sem; char* addr;  struct shmid_ds sbuf;
	struct semid_ds buf; // для семафор 
	fd = shmget( 100, 1024, IPC_CREAT | 0600 ); // создадим РОП

	if ( fd == -1) { perror( "shmget" ); _exit(1); }
	addr = (char*)(shmat(fd, 0, 0 ));

	fd_sem = semget( 100, 3, IPC_CREAT | 0640 );  // создим набор семафор 
	if ( fd_sem == -1 ) { printf( "Ошибка в semget\n" ); _exit( 1 ); }


	// arg.val = 0; semctl( fd_sem, 1, SETVAL, arg );
 	// arg.val = 1; semctl( fd_sem, 0, SETVAL, arg ); 
	arg.val = 0; semctl( fd_sem, 0, SETVAL, arg );
	arg.val = 1; semctl( fd_sem, 0, SETVAL, arg ); 
	arg.val = 2; semctl( fd_sem, 0, SETVAL, arg ); 


	if (f == NULL) perror ("Ошибка открытия файла");
	else
	{
	while ( !feof(f) )                               
	{
		if ( fgets(buffer, 1024 , f) != NULL ) 
		{     
			strcpy(addr, buffer);  
		// fputs(buffer, stdout);  
			arg.val = 0; semctl( fd_sem, 1, SETVAL, arg );
			arg.val = 1; semctl( fd_sem, 0, SETVAL, arg ); 
			semop(fd_sem, first1, 2);
		}
	// if ( semop(fd_sem, nbuf, 1 ) == -1 ) perror ( "semop" );
	}
	fclose(f);                                      
	}

semop(fd_sem, first, 1);




if ( shmctl( fd, IPC_STAT,  &sbuf ) == 0 ) 
    { printf("Размер РОП: %ld\n",  sbuf.shm_segsz );}
    

if ( shmdt( addr ) == -1 ) perror("shmdt");

if ( semctl( fd_sem, 0, IPC_RMID, 0 ) == -1 ) perror( "semctl" );
if ( shmctl( fd,  IPC_RMID,  0 ) == -1 ) perror("shmctl3");


// printf("LOOOL\n");

	return 0;
}


// S - состояние процесса: D - ожидает завершения операции, R - запущен, S - спит, T - остановлен, t - остановлен отладчиком, Z - зомби;