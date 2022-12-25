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

int signal_received =0;
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
void server_handler(){
    signal_received = 1;
    printf("Request from client received i am the server handler \n");
    char id[50];
    char num1[50];
    char mathtype[50];
    char num2[50];
    int pid;
    int fd2=open("toServer.txt",O_RDONLY);
    if(fd2<0)
    {
        perror("after open: ");
        exit(-1);
    }
    readline(id,fd2);
    int clientId = atoi(id);
    printf("Client pid is: %d\n",clientId);
    readline(num1,fd2);
    readline(mathtype,fd2);
    readline(num2,fd2);
    pid=fork();
    if(pid<0)
    {
        printf("error with fork");
        exit(-1);
    }
    if(pid==0)
    {
        char* argp[]={"rm","toServer.txt",NULL};
        execvp("rm",argp);
    }
    else {
        wait(NULL);
        char resultString[50];
        double resultDouble=0;
        if(mathtype[0]=='1')
        {
            resultDouble= (double)atoi(num1)+(double)atoi(num2);
            sprintf(resultString,"%f",resultDouble);
            strcat(resultString,"\0");
        }
        else if(mathtype[0]=='2')
        {
            resultDouble= (double)atoi(num1)-(double)atoi(num2);
            sprintf(resultString,"%f",resultDouble);
            strcat(resultString,"\0");
        }
        else if(mathtype[0]=='3')
        {
            resultDouble= (double)atoi(num1)*(double)atoi(num2);
            sprintf(resultString,"%f",resultDouble);
            strcat(resultString,"\0");
        }
        else if (mathtype[0]=='4')
        {
            if(atoi(num2)==0)  strcpy(resultString,"Cannot divide by 0\n");
            else {
                resultDouble = (double)atoi(num1)/(double)atoi(num2);
                sprintf(resultString,"%f",resultDouble);
                strcat(resultString,"\0");

            }
        }
        char nameFile[50];
        strcpy(nameFile, "toClient");
        strcat(nameFile,id);
        strcat(nameFile, ".txt");
        int fd3 = open(nameFile, O_RDONLY | O_WRONLY | O_CREAT,0777);
        write(fd3, resultString, strlen(resultString));
        close(fd3);
        printf("\nsignal sent to %d\n",atoi(id));
        kill(atoi(id),2);
    }
}
int main(int argc,char *argv[]){
    int time = 59;
    printf("Waiting for new request from client \n");
    signal(1,server_handler);
    while(time>0) {
        if (!signal_received) {
            sleep(1);
            if (time % 30 == 0)
                printf("Waiting for a new client, %d seconds remaining\n", time);
            time--;
        } else {
           pause();
            signal_received=0;
            time=59;
        }
    }
    if(time==0)
        printf("No signal recieved,Exiting \n");
    return 0;
}
