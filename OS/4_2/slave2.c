#include <stdio.h>
#include <sys/ipc.h> // Разделяемая область память (РОП)
#include <sys/shm.h> 
#include <unistd.h>   //Для _exit 
#include <sys/sem.h> // для семофор 	

// struct sembuf nbuf[3] = {{0, -1, 0}, {0, -3, IPC_NOWAIT}, {0, 2, 0}};
struct sembuf nbuf1[2] = {{0, -1, IPC_NOWAIT}, {1, -1, IPC_NOWAIT}};
struct sembuf nbuf2[2] = {{0, 2, IPC_NOWAIT}, {1, 2, IPC_NOWAIT}};
struct sembuf zeoro[1] = {{2, -1, IPC_NOWAIT}};

union semun 
{ int val;
   struct semid_ds  *sbuf;
   ushort  *array;
} arg;


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

	while(semop(fd_sem, zeoro, 1 ) == -1)
	{
		if (semop(fd_sem, nbuf1, 2) != -1)
		{
		// semop(fd_sem, nbuf1, 2);
		printf("%s", addr);
		semop(fd_sem, nbuf2, 2);
		}
	}


	printf("=====================================================\n");

	if (semctl( fd_sem, 0, IPC_STAT, arg) == 0)
	{
		printf("Rоличество семафоров в наборе: %ld\n", arg.sbuf -> sem_nsems );
	}


	// printf("%s\n", addr);


	if ( shmdt( addr ) == -1 ) perror("shmdt");
	

	return 0;
}




