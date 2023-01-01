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
    pids[i]=fork();
    if(pids[i]==-1){ //FORK ERROR
      perror("FORK ERROR!!\n");
      exit(EXIT_FAILURE);
    }
    else if (pids[i]==0){ //Child process
      childJob(fd);
      exit(EXIT_SUCCESS);
    }
    else{ //Parent process
      cpid = waitpid(-1, &status, 0);
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
