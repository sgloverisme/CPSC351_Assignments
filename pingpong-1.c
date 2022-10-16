#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
//Sarah Glover
sem_t semPing, semPong;               // Linux


//=======================================================================
// READ THIS CAREFULLY...
// 
// PUT THE OUTPUT OF YOUR PROGRAM AS A COMMENT IN THE BOTTOM 
//   OF YOUR SUBMITTED SOURCE CODE (this file)
// 
// COMPILE and RUN this code -- you will see ponging and pinging in the wrong order
// 
// IF when developing your solution...
// you see a blank screen...
// it means your code is waiting for a semaphore you have never signaled
//    (or possibly even created)
// semaphore programming is difficult, but very rewarding
//    when done correctly
// 
//
// in the thread runners ping and pong...
// use...
// sem_wait(ptr to semaphore) for Linux, 
// use...
// sem_post(ptr to semaphore) for Linux, 
// in main...
// To create or iniitalize the semaphores, 
// use...
// sem_init(ptr to semaphore, 0, init_semaphore_value) for Linux
//
// and to destroy or release the semaphores 
// use...
// semaphore_destroy(pointer to semaphore) for Linux 
//
//=======================================================================

#define PINGS  5
#define PONGS PINGS


void* ping(void* x) {
    int pings = PINGS;
    while (pings-- > 0) {
        // TODO: need semaphores here... DONE
        //sem_wait(ptr to semaphore) for Linux
        sem_wait(&semPing);  //locking ping 
        
        printf("ping");

        // TODO: and need semaphores here... DONE
        //sem_post(ptr to semaphore) for Linux
        sem_post(&semPong);//unlocking pong

    }
    return NULL;
}

void* pong(void* x) {
    int pongs = PONGS;
    while (pongs-- > 0) {
        // TODO: need semaphores here too... DONE
        sem_wait(&semPong); //locking pong
        
        printf("  pong\n"); 
        
        // TODO: and need semaphores here... DONE
        sem_post(&semPing);//unlcking ping

    }
    return NULL;
}

int main(int argc, const char * argv[]) {
  pthread_t pinger, ponger;
  pthread_attr_t attr;

  // TODO: need to iinitialize (Linux) semaphores semPing and semPong DONE
    // sem_init(ptr to semaphore, 0, init_semaphore_value) for Linux

    sem_init(&semPing, 0,1); //this will start 
    sem_init(&semPong, 0,0 ); 

  pthread_attr_init(&attr);                      
  pthread_create(&ponger, &attr, pong, NULL);    // can create ponger first -- it will wait
  pthread_create(&pinger, &attr, ping, NULL);

  // wait for the pinger and ponger threads to join the main thread
  pthread_join(pinger, NULL);
  pthread_join(ponger, NULL);
 
  // TODO: Need to destroy (Linux) or release (MacOS) the semaphores semPing and semPong DONE
   // semaphore_destroy(pointer to semaphore) for Linux 
    sem_destroy(&semPing);
    sem_destroy(&semPong);

  // TODO: remove the next line DONE
  //printf("   <----- Note: this is not what we want !\n");

  // TODO: uncomment the next line DONE 
   printf("        success!\n"); 
  printf("\tdone...\n");

  return 0;
}

// osc@ubuntu:~/final-src-osc10e/ch5$ ./pingpong-1
// ping  pong
// ping  pong
// ping  pong
// ping  pong
// ping  pong
//         success!
//         done...