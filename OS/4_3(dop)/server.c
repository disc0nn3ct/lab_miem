#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for close

// так как тут дейтаграммы без соединения, надо запускать сначала server, а потом клиент. ( Клиент отправит в никуда просто )


void create_mess(char *msg1, char *buffer)
{
    char cmd[2300] = "echo \" ";
	strcat(cmd, msg1);
	for(int i=6; i < strlen(cmd); i++)
	{
		if(cmd[i] == '\n')
		{
			cmd[i] = ' '; 
			break;
		}
		cmd[i] = ' ';
	}

	strcat(cmd, " \"");
    strcat(cmd, " | sed -e \"s/[[:space:]]\\+/ /g\" |  sed -e \"s/[[:space:]]/\\\\\\| /g\" | cut -c 4- " );
    FILE *f= popen (cmd, "r");
    if (f == NULL) perror ("Ошибка открытия файла");
    else
    {
    while ( !feof(f) )                              // пока не конец файла                           
    {
        fgets(buffer, 1024, f);       // считать символы из файла 
    }
    fclose (f);                                     // закрыть файл                         
    }
    char cmd1[2300] = "ls -lct | grep '";           // работа со строками прелесна 
    strcat(cmd1, buffer);
    cmd1[strlen(cmd1)-1] = ' ';
    cmd1[strlen(cmd1)-3] = ' ';
    cmd1[strlen(cmd1)-4] = '\'';
    strcat(cmd1, "| awk '{ print $9 }' | tr \"\\n\" \" \" ");

    memset(buffer, 0, sizeof(buffer));
    f= popen (cmd1, "r");
    if (f == NULL) perror ("Ошибка открытия файла");
    else
    {
    while ( !feof(f) )                              // пока не конец файла                           
    {
        fgets(buffer, 1024, f);            
    }
    }
    fclose(f);                                     // закрыть файл                         
    
    printf("\nОтсортировано по времени: %s\n",buffer );
}

int main()
{

    int sock;
    struct sockaddr_in addr;
    char buf[4024];
    int bytes_read;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7500);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);


    if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }
    bytes_read = recvfrom(sock, buf, 4024, 0, NULL, NULL);
    buf[bytes_read] = '\0';
    printf(buf);

    char buffer[2500];
    create_mess(buf, buffer);

////////////////////////////////////////////////
    close(sock);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    sendto(sock, buffer, sizeof(buffer), 0,
           (struct sockaddr *)&addr, sizeof(addr));

    return 0;
}