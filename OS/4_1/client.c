// Вариант №18

// Клиент. Создать очередь сообщений. Передать в эту очередь имя файла текущего каталога, который был модифицирован последним.

// Сервер. Выбрать из очереди сообщений, созданной клиентом, последнее сообщение. Определить количество строк указанного файла, а также общее число сообщений в очереди. Записать в стандартный файл вывода эти данные, после чего удалить очередь сообщений.


#include <stdio.h>
#include <sys/msg.h> // для msgget
#include <string.h> // для strlen

// #include <sys/ipc.h>

#ifndef MSGMAX
#define MSGMAX 1024
#endif 

struct msgbuf {
long mtype;
char text[MSGMAX];
};


int main() 
{
	char cmd[]= "ls -lt | head -2 | tail -1 |  ls -lt | head -2 | tail -1 | awk 'NF>1{print $NF}'";
	FILE *f= popen (cmd, "r");

	struct msgbuf buf;
	buf.mtype = 16;


	fgets (buf.text, sizeof(buf.text), f);
	printf("%s",buf.text);
	pclose (f);

	int fd = msgget(111, IPC_CREAT | 0666 );
	
	if (fd == -1 || msgsnd(fd, &buf, strlen(buf.text)+1, IPC_NOWAIT) == -1 )
		perror("Ошибка сообщения"); 
	

	// msgctl(fd, IPC_RMID, 0 );
	// printf("%s",buf.text);

	return 0;
}