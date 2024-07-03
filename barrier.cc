#include <iostream>
#include <pthread.h>
#include <unistd.h> // for sleep
#include <vector>

class Barrier {
public:
    Barrier(int n);
    ~Barrier();
    void wait();

private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int num_threads;
};

Barrier::Barrier(int n) : num_threads(n), count(0) {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);
}

Barrier::~Barrier() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

void Barrier::wait() {
    pthread_mutex_lock(&mutex);
    count++;
    if (count < num_threads) {
        pthread_cond_wait(&cond, &mutex);
    } else {
        count = 0; // Reset count for reuse
        pthread_cond_broadcast(&cond);
    }
    pthread_mutex_unlock(&mutex);
}

// Thread function
void* thread_fun(void* param) {
    Barrier* barrier = static_cast<Barrier*>(param);
    while (true) {
        // Do some work
        std::cout << "Thread " << pthread_self() << " is doing work before the barrier." << std::endl;
        sleep(rand() % 3); // Simulate work with sleep

        // Wait at the barrier
        barrier->wait();

        // Do some more work
        std::cout << "Thread " << pthread_self() << " is doing work after the barrier." << std::endl;
        sleep(rand() % 3); // Simulate work with sleep
    }
    return nullptr;
}

int main() {
    const int N = 5; // Number of threads
    Barrier barrier(N);

    // Create and start N threads
    std::vector<pthread_t> threads(N);
    for (int i = 0; i < N; ++i) {
        pthread_create(&threads[i], nullptr, thread_fun, &barrier);
    }

    // Join threads (though they run indefinitely in this example)
    for (int i = 0; i < N; ++i) {
        pthread_join(threads[i], nullptr);
    }

    return 0;
}