#include "semaphore.h"

int main(){

  int shmid;
  int semid;
  int fd;
  char* lastWritten;
  char buffer[SEG_SIZE];
  char fName[32] = "story";

  struct sembuf sb;

  sb.sem_num = 0;
  sb.sem_op = -1;

  semid = semget(IPC_KEY, 1, 0);
  if(semid < 0){
    printf("ERROR! Couldn't open semaphore: %s\n", strerror(errno));
    return 1;
  }

  printf("Waiting to get in...\n");
  semop(semid, &sb, 1);

  shmid = shmget(IPC_KEY, SEG_SIZE, 0);
  if(shmid < 0){
    printf("ERROR! Couldn't open shared memory segment: %s\n", strerror(errno));
    return 1;
  }

  fd = open(fName, O_WRONLY | O_APPEND);

  lastWritten = shmat(shmid, 0, 0);
  printf("Last addition:%s\n", lastWritten);

  printf("Write your addition:");
  fgets(buffer,SEG_SIZE,stdin);
  printf("\n");

  write(fd, buffer, SEG_SIZE);

  if(close(fd) < 0){
    printf("ERROR! Couldn't close file: %s\n", strerror(errno));
    return 1;
  }

  strcpy(lastWritten, buffer);

  if(shmdt(lastWritten) < 0){
    printf("ERROR! Couldn't detatch shared memory segment: %s\n", strerror(errno));
    return 1;
  }

  sb.sem_op = 1;
  semop(semid, &sb, 1);

  return 0;
}
