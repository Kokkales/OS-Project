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

  printf("Number of inputs: %d\n",argc);
  printf("Number of Processes: %s\n",argv[1]);
  printf("File to be saved: %s\n",argv[2]);
  //Open the file the user gave
  int fd = open(argv[2], O_RDWR | O_CREAT);
  if (fd ==-1)
  {
      // print which type of error have in a code
      printf("Error Number % d\n", errno);

      // print program detail "Success or failure"
      perror("Program\n");
  }
  else{
    printf("File opened succesfyully!!\n");
  }


  //1. Parent process creates multiple child process
  int numOfChildren=atoi(argv[1]);
  int pids[numOfChildren];
  printf("I am the parent process with PID: %d\n",getpid());
  for(int i=0;i<numOfChildren;i++){  //create n child processes
    pids[i]=fork();
    if(pids[i]==-1){ //FORK ERROR
      perror("FORK ERROR!!\n");
      exit(EXIT_FAILURE);
    }
    else if (pids[i]==0){
      childJob(fd);
      sleep(2);  //wait for 5 seconds before parent process continues
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
  char buf[30];
  sprintf(buf, "Parent process:%d\n", getpid());
  int sz = write(fd, buf, strlen(buf));
  printf("Process with PID: %d executed succesfully!\n",getpid());
  close(fd);
  return 0;
}


void childJob(int fd){

  //save the pid of the child to the file the user gave as parameter
  char buf[30];
  sprintf(buf, "Child process:%d\n", getpid());
  int sz = write(fd, buf, strlen(buf));
  printf("I am child with PID: %s and PPID: %d\n",buf,getppid());
}
