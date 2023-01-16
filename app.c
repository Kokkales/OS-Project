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

  int numOfChildren=atoi(argv[1]);
  int pids[numOfChildren];
  int status;
  int i;
  int cpid;
  int sz;
  char buf[30];

// parent and child communication messages
int pipefds1[2], pipefds2[2];
int returnstatus1, returnstatus2;
char readmessage[20];
  char pipe1writemessage[20] = "Hello Child!";
   char pipe2writemessage[20] = "Hello Father!";


  int fd = open(argv[2], O_RDWR | O_CREAT);
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
    if (returnstatus1 == -1) {
      printf("Unable to create pipe 1 \n");
      return 1;
   }
   returnstatus2 = pipe(pipefds2);

   if (returnstatus2 == -1) {
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
      read(pipefds1[0], readmessage, sizeof(readmessage));
      printf("In Child: Reading from pipe 1 – Message is %s\n", readmessage);
      printf("In Child: Writing to pipe 2 – Message is %s\n", pipe2writemessage);
      write(pipefds2[1], pipe2writemessage, sizeof(pipe2writemessage));
      exit(EXIT_SUCCESS);
    }
    else{ //Parent process
      cpid = waitpid(-1, &status, 0);
      close(pipefds1[0]); // Close the unwanted pipe1 read side
      close(pipefds2[1]); // Close the unwanted pipe2 write side
      printf("In Parent: Writing to pipe 1 – Message is %s\n", pipe1writemessage);
      write(pipefds1[1], pipe1writemessage, sizeof(pipe1writemessage));
      read(pipefds2[0], readmessage, sizeof(readmessage));
      printf("In Parent: Reading from pipe 2 – Message is %s\n", readmessage);
      if (WIFEXITED(status)){ //child ended properly
            printf("Child ended normally. Exit code is %d\n",WEXITSTATUS(status));
        }else if(WIFSIGNALED(status)){ //child ended because a signal
            printf("Child ended because of an uncaught signal, signal = %d\n",WTERMSIG(status));
        }else if (WIFSTOPPED(status)){ //child stopped
            printf("Child process has stopped, signal code = %d\n",WSTOPSIG(status));
            exit(EXIT_SUCCESS);
        }
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
