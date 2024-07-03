#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>
// barrier class declaration
class Barrier {
public:  // only constructor, destructor, and wait function are public
    Barrier(int n);
    ~Barrier();
    void wait();

private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int num_threads;
};
// constructor for barrier using n as number of threads
Barrier::Barrier(int n) : num_threads(n), count(0) {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);
}

Barrier::~Barrier() { // destructor destroys mutex and cond variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}
// causes all threads to wait till the count reaches total threads
void Barrier::wait() {  
    pthread_mutex_lock(&mutex); //locks
    count++;                    //increments count
    if (count < num_threads) {  //causes thread to wait till we reach the last thread
    	std::cout << "Thread " << count << " is waiting!" << std::endl;
        pthread_cond_wait(&cond, &mutex);
    } else {
        sleep(1); //delay for work
        std::cout << "Thread " << count << " released the other threads!" << std::endl;
        count = 0; // seset count for reuse
        pthread_cond_broadcast(&cond);  //starts all threads back up
    }
    pthread_mutex_unlock(&mutex); // unlocks
}

// thread function
void* thread_fun(void* param) {
    Barrier* barrier = static_cast<Barrier*>(param);
    while (true) {  //infinite loop of waiting and all releasing at same time
        std::cout << "Thread " << pthread_self() << " is doing work before the barrier." << std::endl;
        sleep(1); //delay for work
        barrier->wait();  //each thread waits till broadcast is sent
        std::cout << "Thread " << pthread_self() << " continued work." << std::endl;
        sleep(1); //delay for work
    }
    return nullptr;
}

int main() {
    const int N = 5; // using 5 threads total 
    Barrier barrier(N);

    // create and start N threads with a thread vector
    std::vector<pthread_t> threads(N);
    for (int i = 0; i < N; ++i) {
        pthread_create(&threads[i], nullptr, thread_fun, &barrier);
    }

    // join threads
    for (int i = 0; i < N; ++i) {
        pthread_join(threads[i], nullptr);
    }

    return 0;
}
