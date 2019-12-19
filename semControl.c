#include "semaphore.h"

int main(int argc, char* argv[]){

  char fName[32] = "story";
  int shmid;
  int semid;
  int fd;

  union semun su;

  su.val = 1;

  if(argc < 2){
    printf("Put a flag!\n");
    return 1;
  }

  if(!strcmp(argv[1],"-c")){

    semid = semget(IPC_KEY, 1, IPC_CREAT | 0644);

    if(semid < 0){
      printf("ERROR! Couldn't open semaphore: %s\n", strerror(errno));
      return 1;
    } else {
      printf("Semaphore created!\n");
      if(semctl(semid, 0, SETVAL,su) < 0){
        printf("ERROR! Couldn't set semaphore: %s\n", strerror(errno));
        return 1;
      }
    }

    shmid = shmget(IPC_KEY, 256, IPC_CREAT | 0644);

    if(shmid < 0){
      printf("ERROR! Couldn't open shared memory: %s\n", strerror(errno));
      return 1;
    } else {
      printf("Shared memory created!\n");
    }

    fd = creat(fName, )
  }
  else if(!strcmp(argv[1],"-r")){

  }
  else if(!strcmp(argv[1],"-c")){

  }
  else {
    printf("ERROR! Unknown flag.\n");
    return 1;
  }

  return 0;
}
