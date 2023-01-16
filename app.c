#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

void childJob(int fd);
int main(int argc,char** argv){

  int numOfChildren=atoi(argv[1]) , sz , status , i , fd;
  pid_t cpid , pids[numOfChildren];
  char buf[30];

  int pipefds1[2], pipefds2[2] , firstPipeStatus , secondPipeStatus;
  char readMessage[20] , firstPipeMessage[20] = "Hello Child!" , secondPipeMessage[20] = "Hello Father!";

  fd = open(argv[2], O_RDWR | O_CREAT);
  if (fd ==-1)
  {
      printf("Error Number % d\n", errno);
      perror("Program\n");
  }

  sprintf(buf, "Parent process : %d\n", getpid());
  sz = write(fd, buf, strlen(buf));

  //Parent process creates multiple child process
  for(i=0;i<numOfChildren;i++){
    // Create pipes for each child
    firstPipeStatus = pipe(pipefds1); //fixme -> pipe is not created
    if (firstPipeStatus == -1) {
      printf("Unable to create pipe 1 \n");
      return 1;
   }
   secondPipeStatus = pipe(pipefds2);

   if (secondPipeStatus == -1) {
      printf("Unable to create pipe 2 \n");
      return 1;
   }
    pids[i]=fork();
    if(pids[i]==-1){ //FORK ERROR
      perror("FORK ERROR!!\n");
      exit(EXIT_FAILURE);
    }
    else if (pids[i]==0){ //Child process
      childJob(fd);
      close(pipefds1[1]); // Close the unwanted pipe1 write side
      close(pipefds2[0]); // Close the unwanted pipe2 read side
      read(pipefds1[0], readMessage, sizeof(readMessage));
      printf("In Child: Reading from pipe 1 – Message is %s\n", readMessage);
      printf("In Child: Writing to pipe 2 – Message is %s\n", secondPipeMessage);
      write(pipefds2[1], secondPipeMessage, sizeof(secondPipeMessage));
      exit(EXIT_SUCCESS);
    }
    else{ //Parent process
      cpid = waitpid(-1, &status, 0);
      close(pipefds1[0]); // Close the unwanted pipe1 read side
      close(pipefds2[1]); // Close the unwanted pipe2 write side
      printf("In Parent: Writing to pipe 1 – Message is %s\n", firstPipeMessage);
      write(pipefds1[1], firstPipeMessage, sizeof(firstPipeMessage));
      read(pipefds2[0], readMessage, sizeof(readMessage));
      printf("In Parent: Reading from pipe 2 – Message is %s\n", readMessage);
      if (WIFEXITED(status)){ //child ended properly
            printf("Child ended normally. Exit code is %d\n",WEXITSTATUS(status));
        }else if(WIFSIGNALED(status)){ //child ended because a signal
            printf("Child ended because of an uncaught signal, signal = %d\n",WTERMSIG(status));
        }else if (WIFSTOPPED(status)){ //child stopped
            printf("Child process has stopped, signal code = %d\n",WSTOPSIG(status));
            exit(EXIT_SUCCESS);
        }
        close(pipefds2[0]); // Close the unwanted pipe1 read side
        close(pipefds1[1]); // Close the unwanted pipe2 write side
    }
  }
  close(fd);
  return 0;
}


void childJob(int fd){
  char buf[30];
  sprintf(buf, "Child process : %d\n", getpid());
  int sz = write(fd, buf, strlen(buf));
}