#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void create_mess(char *msg1 )
{
    char buffer[2048];
    char cmd[2300] = "pwd";

    FILE *f= popen (cmd, "r");
    if (f == NULL) perror ("Ошибка открытия файла");
    else
    {
    while ( !feof(f) )                              // пока не конец файла                           
    {
        if ( fgets(msg1, 1024, f) != NULL)       // считать символы из файла 
         ;        
            
    }
    fclose (f);                                     // закрыть файл                         
    }

    memset(cmd, 0, sizeof(msg1));
    strcpy(cmd, "grep -lr main . | awk -F'/' '{print $NF}' | tr '\n' ' ' ");
    f= popen (cmd, "r");
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

    strcat(msg1, buffer);
}

int main()
{
    char msg1[2500];
    memset(msg1, 0, sizeof(msg1));
    create_mess(msg1);
    printf("%s\n", msg1 );
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


    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    
    while(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    { ; }

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