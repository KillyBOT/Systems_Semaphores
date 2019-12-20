#include "semaphore.h"

int main(int argc, char* argv[]){

  char fName[32] = "story";
  char readBuffer[256];
  int shmid;
  int semid;
  int fd;

  union semun su;
  struct sembuf sb;

  su.val = 1;
  sb.sem_num = 0;
  sb.sem_op = -1;

  if(argc < 2){
    printf("Put a flag!\n");
    return 1;
  }

  if(!strcmp(argv[1],"-c")){

    semid = semget(IPC_KEY, 1, IPC_CREAT | 0644);

    if(semid < 0){
      printf("ERROR! Couldn't open semaphore: %s\n", strerror(errno));
      return 1;
    }
    printf("Semaphore created!\n");

    if(semctl(semid, 0, SETVAL, su) < 0){
      printf("ERROR! Couldn't set semaphore: %s\n", strerror(errno));
      return 1;
    }

    shmid = shmget(IPC_KEY, SEG_SIZE, IPC_CREAT | 0644);

    if(shmid < 0){
      printf("ERROR! Couldn't open shared memory: %s\n", strerror(errno));
      return 1;
    }
    printf("Shared memory created!\n");

    fd = open(fName, O_CREAT | O_TRUNC | O_RDWR, 0644);
    if(fd < 0){
      printf("ERROR! Couldn't open file: %s\n", strerror(errno));
      return 1;
    }
    printf("File created!\n");
  }
  else if(!strcmp(argv[1],"-r")){

    semid = semget(IPC_KEY, 1, 0);

    if(semid < 0){
      printf("ERROR! Couldn't open semaphore: %s\n", strerror(errno));
      return 1;
    }

    if(close(fd) < 0){
      printf("ERROR! Couldn't close file: %s\n", strerror(errno));
      return 1;
    }

    printf("Waiting to get in...\n");
    semop(semid, &sb, 1);

    shmid = shmget(IPC_KEY, SEG_SIZE, 0);

    if(shmid < 0){
      printf("ERROR! Couldn't open shared memory: %s\n", strerror(errno));
      return 1;
    }

    fd = open(fName, O_RDONLY);
    if(fd < 0){
      printf("ERROR! Couldn't open file: %s\n", strerror(errno));
      return 1;
    }

    printf("The current story:\n");

    while(read(fd, readBuffer, 256) >= 256){
        printf("%s", readBuffer);
    }

    semctl(semid, IPC_RMID, 0);
    printf("Semaphore removed!\n");

    shmctl(shmid, IPC_RMID, 0);
    printf("Shared memory removed!\n");

    if(remove(fName) < 0){
      printf("Error removing file: %s\n", strerror(errno));
      return 1;
    }
    printf("File removed!\n");
  }
  else if(!strcmp(argv[1],"-v")){
    fd = open(fName,O_RDONLY);
    if(fd < 0){
      printf("ERROR! Couldn't open file: %s\n", strerror(errno));
      return 1;
    }

    printf("The current story:\n");

    while(read(fd, readBuffer, 256) >= 256){
        printf("%s", readBuffer);
    }

    if(close(fd) < 0){
      printf("ERROR! Couldn't close file: %s\n", strerror(errno));
      return 1;
    }

  }
  else {
    printf("ERROR! Unknown flag.\n");
    return 1;
  }
  return 0;
}
