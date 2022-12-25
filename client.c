// ilay cohen 206779514
//sean lavy   206634529


#include<stdio.h>
#include<stdlib.h>
#include<sys/fcntl.h>
#include<string.h>
#include<errno.h>
#include<signal.h>
#include<time.h>
#include<unistd.h>
void readline(char *buffer, int fd)
{
    char letter = ' ';
    int test1 = 0;
    int temp = 0;
    int len = 0;
    char buf[2];
    test1 = read(fd, buf, 1);
    buf[0];
    while (buf[0] != '\n')
    {
        if (test1 == 0)
        {
            break;
        }
        buffer[temp] = buf[0];
        test1 = read(fd, buf,1);
        temp++;
        len++;
    }
    buffer[temp] = '\0';
}
void handler() {
    printf("Client received file from the server\n");
    int idClient = getpid();
    char nameFile[50];
    char buf[50];
    char ch;
    int i = 0;
    char result[50];
    char pidbuf[50];
    sprintf(pidbuf,"%d",idClient);
    strcpy(nameFile, "toClient");
    strcat(nameFile,pidbuf);
    strcat(nameFile, ".txt");
    printf("file name is: %s\n", nameFile);
    int fd1 = open(nameFile, O_RDONLY);
    if (fd1 < 0) { perror("file didnt open: \n"); }
    readline(result,fd1);
    printf("The result is: %s\n", result);
    int pid1 = fork();
    if (pid1 < 0)
    {
        printf("error with fork");
        exit(-1);
    }
    if (pid1 == 0)
    {
        char *argp[] = {"rm", nameFile, NULL};
        execvp("rm", argp);
    }
    else {
        wait(NULL);
        exit(0);
    }
}
int main(int argc, char *argv[]) {
    int i;
    int fd;
    char id[50];
    char firstNum[50];
    char secondNum[50];
    char operetion[50];
    if(argc!=5)
    {
        printf("error in number of arguments\n");
        exit(-1);
    }
    for (i = 0; i < 10; i++) {
        fd = open("toServer.txt", O_RDONLY | O_WRONLY | O_CREAT, 0666);
        if (fd < 0) {
            printf("cannot able to open and create the file");
            printf("File failed to open in the %d time\n",i+1);
            int x = random() % 6;
            sleep(x);
        } else break;
    }
    if (i == 10) {
        printf("cannot connect to the server on the 10 try, Bye Bye");
        exit(-1);
    }
    strcpy(firstNum, argv[2]);
    strcpy(operetion, argv[3]);
    strcpy(secondNum, argv[4]);
    char all[50];
    int idclient =  getpid();
    char clientid[50];
    sprintf(clientid,"%d",idclient);
    strcpy(all,clientid);
    strcat(all ,"\n");
    write(fd,all,strlen(all));
    strcpy(all,argv[2]);
    strcat(all,"\n");
    write(fd,all,strlen(all));
    strcpy(all,argv[3]);
    strcat(all,"\n");
    write(fd,all,strlen(all));
    strcpy(all,argv[4]);
    strcat(all,"\n");
    write(fd,all,strlen(all));
    close(fd);
    signal(2,handler);
    kill(atoi(argv[1]), 1);
    time_t start_time = time(NULL);
    while (time(NULL) - start_time < 30) {
        if (time(NULL) - start_time == 29) {
            printf("Server took too long to respond,Terminating Client.\n");
            exit(-1);
        }
        sleep(1);
    }
    return 0;
}
