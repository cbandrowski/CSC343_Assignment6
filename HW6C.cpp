#include <iostream>
#include <pthread.h>
#include <unistd.h>  // for sleep

pthread_mutex_t lock;
std::string shared_data = "Initial Data";

// Writer Thread Function
void* writer_thread(void* arg) {
    pthread_mutex_lock(&lock);
    std::cout << "[Writer] Acquired lock. Writing to shared_data...\n";
    shared_data = "Hello from Thread 1!";
    sleep(1);  // Simulate delay
    std::cout << "[Writer] Done writing.\n";
    pthread_mutex_unlock(&lock);
    return nullptr;
}

// Reader Thread Function
void* reader_thread(void* arg) {
    sleep(2);  // Wait to make sure writer writes first
    pthread_mutex_lock(&lock);
    std::cout << "[Reader] Acquired lock. Reading shared_data...\n";
    std::cout << "[Reader] Read: " << shared_data << std::endl;
    pthread_mutex_unlock(&lock);
    return nullptr;
}

int main() {
    pthread_t thread1, thread2;

    // Initialize the mutex
    if (pthread_mutex_init(&lock, nullptr) != 0) {
        std::cerr << "Mutex init failed\n";
        return 1;
    }

    // Create threads
    pthread_create(&thread1, nullptr, writer_thread, nullptr);
    pthread_create(&thread2, nullptr, reader_thread, nullptr);

    // Wait for threads to finish
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    // Destroy mutex
    pthread_mutex_destroy(&lock);

    std::cout << "Main thread done.\n";
    return 0;
}
