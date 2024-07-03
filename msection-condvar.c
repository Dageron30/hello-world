#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10 // Number of threads
#define M 3  // Maximum number of concurrent threads in the m-section

// Declare the condition variable and mutex
pthread_mutex_t mutex;
pthread_cond_t cond;
int current_threads_in_msection = 0; // Counter for the number of threads in the m-section

// Function prototypes
void enter(pthread_mutex_t *mutex, pthread_cond_t *cond, int *current_count);
void leave(pthread_mutex_t *mutex, pthread_cond_t *cond, int *current_count);
void* doWork(void *arg);
void doCriticalWork();

void enter(pthread_mutex_t *mutex, pthread_cond_t *cond, int *current_count) {
    pthread_mutex_lock(mutex);
    while (*current_count >= M) {
        pthread_cond_wait(cond, mutex);
    }
    (*current_count)++;
    pthread_mutex_unlock(mutex);
}

void leave(pthread_mutex_t *mutex, pthread_cond_t *cond, int *current_count) {
    pthread_mutex_lock(mutex);
    (*current_count)--;
    pthread_cond_signal(cond);
    pthread_mutex_unlock(mutex);
}

void* doWork(void *arg) {
    while (1) {
        enter(&mutex, &cond, &current_threads_in_msection); // Limit access to m threads
        // Execute m-section
        doCriticalWork(); // Run by max. m threads
        leave(&mutex, &cond, &current_threads_in_msection); // Leave m-section
        // Do more work (simulate with sleep)
        usleep(rand() % 100000);
    }
    return NULL;
}

void doCriticalWork() {
    static pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&print_mutex);
    printf("Thread ID: %ld, Threads in m-section: %d\n", pthread_self(), current_threads_in_msection);
    pthread_mutex_unlock(&print_mutex);
    usleep(rand() % 100000); // Simulate some work
}

int main() {
    pthread_t threads[N];
    // Initialize the mutex and condition variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // Seed the random number generator
    srand(time(NULL));

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

    // Destroy the mutex and condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}