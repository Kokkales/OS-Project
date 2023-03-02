#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>


//I create a struct that saves the PID,the status and the function that executes so now i can handle which child will be executed
struct Child{
  pid_t childPID;
  int isAvailable; //1 means available , 0 means not available
  void (*func)(int *pipefds1,int *pipefds2,int fd); // a function that makes the child to do the work
};

void childJob(int *pipefds1,int *pipefds2,int fd);
void onSIGTERM(int sig);
void killChild(int sig);

int main(int argc,char** argv){
  //create a struct to save children process situation
  struct Child *children;
  if (children == NULL) {
        perror("Memory allocation failed");
        exit(1);
  }

  int numOfChildren=atoi(argv[1]) , sz , status , i , fd;
  pid_t cpid , pids[numOfChildren];
  char buf[30];
   //pipes
  int pipefds1[2], pipefds2[2];
  int returnstatus1, returnstatus2;
  int pid;
  char readmessage[70];
  char pipe1writemessage[70];

  children = (struct Child *)malloc(numOfChildren * sizeof(struct Child));

  // set allchildren xtatus that they are available for a job
  for(i=0;i<numOfChildren;i++){
    children[i].isAvailable=1;
  }

  fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC);
  if (fd ==-1)
  {
      printf("Error Number % d\n", errno);
      perror("Program\n");
  }

  //Parent process creates multiple child process
  for(i=0;i<numOfChildren;i++){
    //create communication pipes for each child
    returnstatus1 = pipe(pipefds1);
    if (returnstatus1 == -1) {
      printf("Unable to create pipe 1 \n");
      return 1;
    }
    returnstatus2 = pipe(pipefds2);
    if (returnstatus2 == -1) {
      printf("Unable to create pipe 2 \n");
      return 1;
    }

    //create children
    pids[i]=fork();
    if(pids[i]==-1){ //FORK ERROR
      perror("FORK ERROR!!\n");
      exit(EXIT_FAILURE);
    }
    else if (pids[i]==0){ //Child process
      signal(SIGINT,(void(*)(int))onSIGTERM);

      //when i set the struct: i will be able to choose  and handle if a process is working to give the job to another process
      //get children pid and save it to the struct , set also the new status to 0 unavailable, and set te function that it has to execute
      children[i].childPID=getpid();
      children[i].isAvailable=0;
      children[i].func=&childJob;
      children[i].func(pipefds1,pipefds2,fd);
      children[i].isAvailable=1;
      }
    else if(pids[i]!=0){ //parent process
      signal(SIGINT,(void(*)(int))killChild);

      close(pipefds1[0]); // Close the unwanted pipe1 read side
      close(pipefds2[1]); // Close the unwanted pipe2 write side
      sprintf(pipe1writemessage,"Hello child, I am your father and I call you: <kostas%d>/%d",i,i*2);
      printf("Process: %d -> In Parent: Writing to child – Message is %s\n", getpid() ,pipe1writemessage);
      write(pipefds1[1], pipe1writemessage, sizeof(pipe1writemessage));
      read(pipefds2[0], readmessage, sizeof(readmessage));
      printf("Process: %d -> In Parent: Reading from child – Message is %s\n",getpid() , readmessage);
      close(pipefds1[1]); // Close the unwanted pipe1 write side
      close(pipefds2[0]); // Close the unwanted pipe2 read side

    }
  }

  //parent waits the n childs to end
  for(i=0;i<numOfChildren;i++){
    cpid = waitpid(-1, &status, 0);
    if (WIFEXITED(status)){ //child ended properly
        printf("Child ended normally. Exit code is %d\n",WEXITSTATUS(status));
    }else if(WIFSIGNALED(status)){ //child ended because a signal
        printf("Child ended because of an uncaught signal, signal = %d\n",WTERMSIG(status));
    }else if (WIFSTOPPED(status)){ //child stopped
        printf("Child process has stopped, signal code = %d\n",WSTOPSIG(status));
    }

  }
  // exit(EXIT_SUCCESS);
  close(fd);
  return 0;
}

void killChild(int sig){
  kill(-1,SIGINT);
}

void onSIGTERM(int sig){
  printf("\nI am a child and my father just terminated me!\n");
  exit(0);
}


void childJob(int *pipefds1,int *pipefds2,int fd){
  char pipe2writemessage[70] = "Done";
  char readmessage[70];
  close(pipefds1[1]); // Close the unwanted pipe1 write side
  close(pipefds2[0]); // Close the unwanted pipe2 read side
  //reading from father
  read(pipefds1[0], readmessage, sizeof(readmessage));
  printf("Process: %d -> In Child: Reading from parent – Message is %s\n",getpid() , readmessage);
  //writing to file
  char * token = strtok(readmessage, " ");
  // printf( " %s\n", token[10] ); //printing the token
  char childName[30];
  char childSleep[30];
  int i;
  while( token != NULL ) {
      sprintf(childName,"%s",token);
      token = strtok(NULL, " ");
      i++;
  }
  char buf[50];
  printf("Child name: %s\n",childName);

  char * tokenTwo = strtok(childName, "/");
  char s[2]="/";
  i=0;
  while( tokenTwo != NULL ) {
      sprintf(childSleep,"%s",tokenTwo);
      // printf("My sleep token: %s\n",tokenTwo);
      tokenTwo = strtok(NULL, s);
      i++;
  }

  int sleepTime=atoi(childSleep);

  sprintf(buf, "<%d> -> %s\n", getpid(),childName);
  int sz = write(fd, buf, strlen(buf));
  //writing back to father
  printf("Process: %d -> In Child: Writing to parent – Message is %s\n", getpid() ,pipe2writemessage);
  write(pipefds2[1], pipe2writemessage, sizeof(pipe2writemessage));
  // printf("My sleep time: %d\n", sleepTime);
  sleep(sleepTime);
  exit(EXIT_SUCCESS);
}
