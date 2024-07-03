#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10 // Number of threads
#define M 3  // Maximum number of concurrent threads in the m-section

// Declare the semaphore
sem_t semaphore;

// Function prototypes
void enter(sem_t *sem);
void leave(sem_t *sem);
void* doWork(void *arg);
void doCriticalWork();

void enter(sem_t *sem) {
    // Wait until the semaphore value is greater than 0
    sem_wait(sem);
}

void leave(sem_t *sem) {
    // Signal (increment) the semaphore value
    sem_post(sem);
}

void* doWork(void *arg) {
    while (1) {
        enter(&semaphore); // Limit access to m threads
        // Execute m-section
        doCriticalWork(); // Run by max. m threads
        leave(&semaphore); // Leave m-section
        // Do more work (simulate with sleep)
        usleep(rand() % 100000);
    }
    return NULL;
}

void doCriticalWork() {
    static int count = 0;
    static pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&count_mutex);
    count++;
    printf("Thread ID: %ld, Threads in m-section: %d\n", pthread_self(), count);
    usleep(rand() % 100000); // Simulate some work
    count--;
    pthread_mutex_unlock(&count_mutex);
}

int main() {
    pthread_t threads[N];
    // Initialize the semaphore with a value of M
    sem_init(&semaphore, 0, M);

    // Create and start N threads
    for (int i = 0; i < N; i++) {
        if (pthread_create(&threads[i], NULL, doWork, NULL) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Join threads (though they run indefinitely)
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the semaphore
    sem_destroy(&semaphore);

    return 0;
}