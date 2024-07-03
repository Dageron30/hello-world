#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10 // number of threads
#define M 3  // maximum number of concurrent threads in the m-section(critical)

// declare the condition variable and mutex
pthread_mutex_t mutex;
pthread_cond_t cond;
int threads = 0; // counter for m-section threads

// function prototypes
void enter(pthread_mutex_t *mutex, pthread_cond_t *cond, int *current_count);
void leave(pthread_mutex_t *mutex, pthread_cond_t *cond, int *current_count);
void* doWork(void *arg);
void doCriticalWork();

void enter(pthread_mutex_t *mutex, pthread_cond_t *cond, int *current_count) {
    pthread_mutex_lock(mutex);
    while (*current_count >= M) { // limits to 3 locks given out
        pthread_cond_wait(cond, mutex);
    }
    (*current_count)++;  //increments count of used locks
    pthread_mutex_unlock(mutex);
}

void leave(pthread_mutex_t *mutex, pthread_cond_t *cond, int *current_count) {
    pthread_mutex_lock(mutex);
    (*current_count)--;  //decrements count of used locks
    pthread_cond_signal(cond);
    pthread_mutex_unlock(mutex);
}

void* doWork(void *arg) {
    while (1) {
        enter(&mutex, &cond, &threads); // limit access to m threads
        doCriticalWork(); // max m threads running
        leave(&mutex, &cond, &threads); // leave m-section
        sleep(1); //delay for work
    }
    return NULL;
}

void doCriticalWork() {
    static pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&print_mutex); //locks, prints, then unlocks
    printf("Thread ID: %ld, Threads in m-section: %d\n", pthread_self(), threads);
    pthread_mutex_unlock(&print_mutex);
    sleep(1); // delay(work)
}

int main() {
    pthread_t threads[N];
    // initialize the mutex and condition variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

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

    // destroy the mutex and condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
