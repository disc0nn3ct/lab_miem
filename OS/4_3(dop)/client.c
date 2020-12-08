#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

char msg1[] = "Hello there!\n";

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




    sendto(sock, msg1, sizeof(msg1), 0,
           (struct sockaddr *)&addr, sizeof(addr));




    // connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    // send(sock, msg2, sizeof(msg2), 0);

    // close(sock);

////////////////////////////////////////////////


    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7500);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    while(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
                printf("OW");

        // perror("bind");
        // exit(2);
    }

    while(1)
    {
        bytes_read = recvfrom(sock, buf, 1024, 0, NULL, NULL);
        buf[bytes_read] = '\0';
        printf(buf);
        if(bytes_read > 0)
        {
            break;
        }
    }



    return 0;
}