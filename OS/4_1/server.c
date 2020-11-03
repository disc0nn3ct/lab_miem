#include <stdio.h>
#include <sys/msg.h> // для msgget
#include <string.h>

// #include <string.h>
// #include <sys/ipc.h>
// #include <sys/types.h>
// #include <unistd.h>


#ifndef MSGMAX
#define MSGMAX 1024
#endif 



struct msgbuf 
{
long mtype;
char text[MSGMAX];
};

int main()
{

char one[MSGMAX];
	struct msgbuf buf;
	struct msqid_ds buf1;

	char cmd[] = "wc -l ";


	int fd = msgget(111, 0);

	if(fd == -1)
		perror("Ошибка сообщения"); 

	if(msgrcv(fd, &buf, MSGMAX,16, IPC_NOWAIT) != -1){
		strcat(cmd, buf.text); 
		FILE *f= popen (cmd, "r");
		fgets (one, MSGMAX, f);
		printf("%s\n",one);
	}
		// printf("%s\n", buf.text);
	else perror("Ошибка в msgrcv()\n");


	if(msgctl(fd, IPC_STAT, &buf1)==0)
	{
		printf("Количество сообщений в очереди %ld\n",buf1.msg_qnum);
	}


	msgctl(fd, IPC_RMID, NULL);


	return 0;
}
