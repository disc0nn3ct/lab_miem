#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// так как тут дейтаграммы без соединения, надо запускать сначала server, а потом клиент. ( Клиент отправит в никуда просто )

char msg1[] = "Hello there!\n";

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

    strcat(cmd, " | sed -e \"s/[[:space:]]\\+/ /g\" |  sort -nr " );

    printf("LOL %s\n",cmd );

    // char cmd[2300] = " | sed 1d | grep -lr main . | awk -F'/' '{print $NF}' | ls -l | sort -k6M -k7n | awk '{print $9}'";
    FILE *f= popen (cmd, "r");
    if (f == NULL) perror ("Ошибка открытия файла");
    else
    {
    while ( !feof(f) )                              // пока не конец файла                           
    {
        if ( fgets(buffer, 1024, f) != NULL)       // считать символы из файла 
         ;        
            
    }
    fclose (f);                                     // закрыть файл                         
    }

    printf("LOL %s\n",buffer );

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
    // printf("After server work%s\n", buffer);

////////////////////////////////////////////////
    close(sock);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    sendto(sock, msg1, sizeof(msg1), 0,
           (struct sockaddr *)&addr, sizeof(addr));



    
    return 0;
}