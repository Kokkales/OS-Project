#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void childJob();
int main(int argc,char** argv){

  
  //1. Parent process creates multiple child process
  int numOfChildren=10;
  int pids[numOfChildren];
  printf("I am the parent process with PID: %d\n",getpid());
  for(int i=0;i<numOfChildren;i++){  //create n child processes
    pids[i]=fork();
    if(pids[i]==-1){ //FORK ERROR
      perror("FORK ERROR!!\n");
      exit(EXIT_FAILURE);
    }
    else if (pids[i]==0){
      childJob();
      sleep(5);  //wait for 5 seconds before parent process continues
      exit(EXIT_SUCCESS);
    }
    // printf("OK\n");
  }


  int status;
  int cpid;
  while (numOfChildren > 0) {
    cpid = waitpid(-1, &status, 0);
    printf("Child with PID %ld exited with status 0x%x.\n", (long)cpid, status);
    numOfChildren--;
  }
  printf("Process with PID: %d executed succesfully!\n",getpid());
  return 0;
}


void childJob(){
  printf("I am child with PID: %d and PPID: %d\n",getpid(),getppid());
}
