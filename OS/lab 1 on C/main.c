// Написать программу сравнения и вывода результата двух введенных по запросу файлов.
// Информацию переслать через межпроцессный канал в параллельный процесс, где определить число блоков, занимаемых каждым из введенных файлов.
// Предусмотреть возможность прерывания по сигналу <CTRL>+<C>. Если поступило более 3-х сигналов прерывания, то создать новый каталог в текущем и записать туда сравниваемые файлы.



#include <stdio.h>  // scanf и тд 
#include <string.h> // для memset
#include <fcntl.h> // для open
#include <unistd.h> // для read и pipe 
#include <sys/stat.h> // stat 
// #include	<direct.h> // для mkdir 
#include <stdlib.h> // для exit 
#include <signal.h> 

#include <sys/types.h> // для wait
#include <sys/wait.h> // для wait



char name1[252], name2[252];
int i = 0;



void obrabotk()
{
	i+=1;
	printf("------------------------%d \n",i);
	// printf("=-=-=-=1=====%s,2=========%s",name1,name2);
	// if (i%3==0)
	// *i= *i+1;
	// printf("------------------------%d \n",*i);
	// if (*i%3==0)
	if (i%3==0)		
	{
		mkdir("test", 0777);
		if (fork() == 0)       // execute action in parallel process
	    {
			write(STDOUT_FILENO, "I made up new folter that named \"test\"! starting copping file:\n", 62);   // мб проверку найденых файлов ? 

			if (access(name1, F_OK) == 0)
		    {
		    	if(access(name2, F_OK) == 0)
		    	{
		    		write(STDOUT_FILENO, "all ok\n", 7);
					execl("/bin/cp", "cp",name1, name2, "test/", NULL);    
		    	}
		    	else
		    	{
		    		write(STDOUT_FILENO, " Sorry but i can't make up copy because problem with 2 file:\n", 59);
		    		return;
		    	}
		    } 
		    else
		    {
		    	write(STDOUT_FILENO, "Sorry but i can't make up copy because problem with 1 file:\n", 60);
		        return;
		    }
            return;
		}
	  	else
	    {	
	        wait(0);
	    }
	}

}



int open_myFILE(char *name)
{
    int fd;
    fd = open(name, O_RDONLY);
    if (fd != -1)
    {
        return fd;
    }




	scanf("%s", name);
	fd = open(name, O_RDONLY);
	while(fd == -1)
    {
        printf("Ow, problem, try find new one: ");
        scanf("%s", name);
        fd = open(name, O_RDONLY);
    }
    printf("fd = %d \n", fd);

	return fd;
}



int main(int argc, char* argv[])
{
	struct sigaction s;
	struct stat st1,st2; 

	char buf,buf1[1],buf2[1],vivod[100];
    int p[2],fd,fd1,counter,len1,len2;
    // char name1[252], name2[252],buf,buf1[1],buf2[1],vivod[100];
    // int p[2],fd,fd1,counter,len1,len2,i;
    i = 0;
    memset(name1,0,252);
    memset(name2,0,252);
    memset(vivod,0,100);
    memset(buf1,0,1);
    memset(buf2,0,1);
    // memset(&s, 0, sizeof(s));


    s.sa_handler = obrabotk;
    sigemptyset(&s.sa_mask);
    // sigaddset(&s.sa_mask, SIGINT); 
   
    // sigprocmask(0,0,&s.sa_mask);
    // s.sa_flags = 0;
    sigaction(SIGINT,&s,0);
    

    if (argc > 2)
    {
        strcpy(name1, argv[1]);
        strcpy(name2, argv[2]);
        printf("===============%s================%s\n",name1,name2 );
        
    }


    printf("Hello user! Please give files that can be read. \nInput 1-st file to compare: ");
    fd = open_myFILE(&name1[0]);
  
    // ----------
    printf("\nINPUT the 2-cond one: ");

    fd1 = open_myFILE(&name2[0]);




    // printf("%s xxx %s", buf1, buf2);
    

    // printf("%s, ---- %s\n", name1,name2);
    // начнем сравннеине

	stat(name1,&st1);
    // printf(" ================== %ld",st.st_size);
    stat(name2,&st2);
    len1 = st1.st_size;
    len2 = st2.st_size;

	counter = 0;
     while (read(fd, buf1, 1) && read(fd1, buf2, 1))
    // while (read(fd, &buf1, 1))
    {
     	putchar(buf1[0]); 
     	putchar(buf2[0]);
     	putchar(' ');
 		if (buf1[0] != buf2[0])
 		{
 			break;
 		}
 		else
 		{
 			counter+=1;
     	}
    }





    if (len1 < len2)
    {
    	snprintf(vivod, sizeof vivod, "\now the 1-st file is bigger and %d bytes are the same!\n",counter );
    	// printf("%s",vivod);
    	// printf("\nOw 2-nd file is bigger and %d bytes are the same!\n",counter);
    }
    else 
    {
    	if (len1 > len2)
    	{
    	snprintf(vivod, sizeof vivod, "\now the 1-st file is bigger and %d bytes are the same!\n",counter);		
    	// printf("%s",vivod);	
    	// printf("\now the 1-st file is bigger and %d bytes are the same!\n",counter); 
    	
		}
		else
		{
		if (len1 == counter)
			{
				strcpy(vivod, "\nFiles are the same!!!\n" );
    			// printf("%s",vivod);	
				// printf("\nFiles are the same!!!\n");
			}
			else
			{
			snprintf(vivod, sizeof vivod, "\nFiles has the same length and %d bytes are same!\n",counter); 
	    	// printf("%s",vivod);		
			// printf("\nFiles has the same length and %d bytes are same!\n",counter); 
			}
		}
    }
	

	pipe(p);

    if (fork() == 0) 
	{   
		write(STDOUT_FILENO, "PARALLELKA:", 10);
        close(p[1]);      
        while (read(p[0], &buf, 1) > 0)
        	write(STDOUT_FILENO, &buf, 1);

        write(STDOUT_FILENO, "\n", 1);
        close(p[0]);
        printf("File 1 blocks = %ld, File 2 blocks = %ld \n", st1.st_blocks, st2.st_blocks);
        exit(0);
    }
    else 
    {            // i'm baty
        close(p[0]);
        write(p[1], vivod, strlen(vivod));
        close(p[1]); 
        // sleep(10);         
       	// wait(0);

    }




 	
    // while ( (read(fd, buf1, 1) == read(fd1, buf2, 1))) 
    // 	{

    // 		printf("buf1=%d, buf2 = %d \n",buf1,buf2);
    // 		lseek(fd,1,1);
    // 		lseek(fd1,1,1);

    // 	}

    // sleep(8);

    // obrabotk(&i, &name1[0], &name2[0]);
    // obrabotk(&i, &name1[0], &name2[0]);
    // obrabotk(&i, &name1[0], &name2[0]);
       while(getchar() != 'q')
        {
        }

       printf("FINISHED\n");


    return 0;
}





// int main()
// {
// ...
//     struct sigaction act;
//     memset(&act, 0, sizeof(act));
//     act.sa_handler = hdl;

//     sigemptyset(&act.sa_mask);                                                             
//     sigaddset(&act.sa_mask, SIGINT); 
//     sigaction(SIGINT, &act, 0);
    
// ...
//     pthread_kill(th1, SIGINT);
// ...
// }



// int main(int argc, char* argv[])
// {
//        if (argc > 1)// если передаем аргументы, то argc будет больше 1(в зависимости от кол-ва аргументов)
//        {
//              cout << argv[1]<<endl;// вывод второй строки из массива указателей на строки(нумерация в строках начинается с 0 )
//        } else
//                {
//                  cout << "Not arguments" << endl;
//                }
//        system("pause");
//        return 0;

// #include <stdio.h>
// int main(int argc, char *argv[])
// {
//   int i=0;
//   printf("%s\n",argv[0]);
//   printf("Программа запущена и получила строку : ");
//   while(argv[++i] != NULL)
//   printf("%s ",argv[i]);
//   return 0;
// }
// https://www.opennet.ru/docs/RUS/linux_parallel/node8.html



// https://habr.com/en/post/141206/
// whereis - где находится файл 