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
#include <sys/sem.h> // для семафор 	


struct sembuf first[4] = {{0, +1, 0}, {0, -3, 0}, {0, +4, 0}, {0, -5, 0}};


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
	strcat(cmd, " | awk -v f=1 -v t=15 \'{for(i=f;i<=t;i++) printf(\"%s%s\",$i,(i==t)?\"\\n\":OFS)}\' ");  // Нужно для ограничения 

	f = popen (cmd, "r"); 



	int fd, fd_sem; char* addr;  struct shmid_ds sbuf;
	struct semid_ds buf; // для семафор 
	fd = shmget( 100, 25000, IPC_CREAT | 0600 ); // создадим РОП

	if ( fd == -1) { perror( "shmget" ); _exit(1); }
	addr = (char*)(shmat(fd, 0, 0 ));
	if ( addr == (char *)(-1)){ perror( "shmat" ); _exit ( 2 );}

	fd_sem = semget( 100, 1, IPC_CREAT | 0640 );  // создим набор семафор 
	if ( fd_sem == -1 ) { printf( "Ошибка в semget\n" ); _exit( 1 ); }



	arg.val = 0; semctl( fd_sem, 0, SETVAL, arg ); 



	while(fgets(buffer, 150, f) != NULL)
	{
		// strcat(temp, buffer);
		strcat(addr, buffer);
	}

	semop(fd_sem, &first[0], 1);
	printf("%ld\n", strlen(addr));

if ( shmctl( fd, IPC_STAT,  &sbuf ) == 0 ) 
    { printf("Размер РОП: %ld\n",  sbuf.shm_segsz );}  
    
	semop(fd_sem, &first[1], 1);
	printf("%s\n", addr);

	semop(fd_sem, &first[2], 1); // 
	semop(fd_sem, &first[3], 1);
	printf("%s\n", addr);


if ( shmdt( addr ) == -1 ) perror("shmdt");

if ( semctl( fd_sem, 0, IPC_RMID, 0 ) == -1 ) perror( "semctl" );
if ( shmctl( fd,  IPC_RMID,  0 ) == -1 ) perror("shmctl3");
//////////////////////////////////////////////////////////


	return 0;
}


// S - состояние процесса: D - ожидает завершения операции, R - запущен, S - спит, T - остановлен, t - остановлен отладчиком, Z - зомби;