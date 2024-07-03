#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10 // number of threads
#define M 3  // maximum number of concurrent threads in the m-section(critical)

// declare the semaphore
sem_t semaphore;

// function prototypes
void enter(sem_t *sem);
void leave(sem_t *sem);
void* doWork(void *arg);
void doCriticalWork();

void enter(sem_t *sem) {
    // wait until the semaphore is over 0
    sem_wait(sem);
}

void leave(sem_t *sem) {
    // increment the semaphore value
    sem_post(sem);
}

void* doWork(void *arg) {
    while (1) {
        enter(&semaphore); // restricts to number of semaphore
        doCriticalWork(); // max m threads running
        leave(&semaphore); // leave m-section
        sleep(1); //delay for work
    }
    return NULL;
}

void doCriticalWork() {
    static int count = 0;
    count++;
    printf("Thread ID: %ld, Threads in m-section: %d\n", pthread_self(), count);
    sleep(1); // delay(work) so we don't stay on thread 1
    count--;
}

int main() {
    pthread_t threads[N];
    // initialize the semaphore with M
    sem_init(&semaphore, 0, M);

    // create and start N threads with error prot
    for (int i = 0; i < N; i++) {
        if (pthread_create(&threads[i], NULL, doWork, NULL) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    // join threads
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }
    // destroy the semaphore
    sem_destroy(&semaphore);

    return 0;
}
