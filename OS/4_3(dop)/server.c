#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>


// так как тут дейтаграммы без соединения, надо запускать сначала server, а потом клиент. ( Клиент отправит в никуда просто )

char msg1[] = "Hello there!\n";

void create_mess(char *msg1 )
{
    char buffer[2048];
    char cmd[2300] = "pwd";
    






}


int main()
{
    int sock;
    struct sockaddr_in addr;
    char buf[1024];
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

    // while(bytes_read == 0)
    // {
        bytes_read = recvfrom(sock, buf, 1024, 0, NULL, NULL);
        buf[bytes_read] = '\0';
        printf(buf);
    // }


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