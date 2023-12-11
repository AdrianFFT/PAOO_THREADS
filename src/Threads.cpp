#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <condition_variable>

const int MIN_THREADS = 3;
const int MAX_THREADS = 30;
const int THREADS_AT_ONCE = 3;

std::mutex coutMutex;
sem_t sem;
std::mutex barrierMutex;
int barrierCount = 0;
std::condition_variable barrierCV;

void barrier() {
    std::unique_lock<std::mutex> lock(barrierMutex);
    barrierCount++;
    if (barrierCount == THREADS_AT_ONCE) {
        sem_post(&sem);
        barrierCount = 0;
        barrierCV.notify_all();
    } else {
        barrierCV.wait(lock, [barrierCount]() { return barrierCount == 0; });
    }
}

void threadFunction(int id) {
    {
        std::unique_lock<std::mutex> lock(coutMutex);
        std::cout << "Thread " << id << " started." << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    {
        std::unique_lock<std::mutex> lock(coutMutex);
        std::cout << "Thread " << id << " stopped." << std::endl;
    }

    barrier();
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    int numThreads = rand() % (MAX_THREADS - MIN_THREADS + 1) + MIN_THREADS;
    std::cout << numThreads << std::endl;

    sem_init(&sem, 0, 0);

    std::vector<std::shared_ptr<std::thread>> threads;

    for (int i = 0; i < numThreads; ++i) {
        auto threadPtr = std::make_unique<std::thread>(threadFunction, i + 1);
        threads.push_back(std::shared_ptr<std::thread>(std::move(threadPtr)));

        if ((i + 1) % THREADS_AT_ONCE == 0) {
            sem_wait(&sem);
            std::cout << "Next batch of threads will start soon..." << std::endl;
        }
    }

    for (auto& threadPtr : threads) {
        threadPtr->join();
    }

    sem_destroy(&sem);

    return 0;
}
 