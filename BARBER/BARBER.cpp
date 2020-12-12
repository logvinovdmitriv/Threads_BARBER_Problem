#include <iostream>      
#include <thread>        
#include <mutex>         
#include <condition_variable>
#include <list>
#include <semaphore>
#include  <chrono>
#define COUNT_OF_CHAIRS_ALL 10
int COUNT_OF_CHAIRS_USED = 0;


std::mutex mtx;             // mutex for critical section
std::condition_variable cv; // condition variable for critical section  
bool ready = false;         // Tell threads to run
int current = 0;            // current count
std::thread threads[15];

/* Prints the thread id / max number of threads */
void DoBarberWork(int num) {

    std::unique_lock<std::mutex> lck(mtx);
   
    while (num != current || !ready) 
    {
        if (!ready)
        {
            std::cout << "Customer NO " << num << " came, BARBERSHOP CLOSED\n";
        };
        if (num != current && ready) 
        {
            std::cout << "Customer NO " << num << " Waiting\n";
        };
        cv.wait(lck);
    }
    current++;
    std::cout << "Barber Starts\t";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand()%500 + 500));
    std::cout << "current NO of Customer is: " << current << "\t";
    COUNT_OF_CHAIRS_USED--;
    std::cout << "Barber Ends\n\n";

    /* Notify next threads to check if it is their turn */
    cv.notify_all();
}

/* Changes ready to true, and begins the threads printing */
void runBarberShop() {
    std::unique_lock<std::mutex> lck(mtx);
    ready = true;
    cv.notify_all();
}

int main() {

    int threadnum = 15;
 

    /* Создание потоков ПОСЕТИТЕОЕЙ */
    for (int id = 0; id < threadnum; id++)
    {
        std::cout << "Customer NO " << id << " CREATED\n";
        if (COUNT_OF_CHAIRS_USED >= COUNT_OF_CHAIRS_ALL)
        {
            std::cout << "Customer LEFT - NO FREE CHAIRS\n";
        }
        else
        {
            threads[id] = std::thread(DoBarberWork, id);
            COUNT_OF_CHAIRS_USED++;
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 500));
        };
    }

   
    std::cout << "BARBERSHOP OPEN\n";
    runBarberShop(); // ОТКРЫТИЕ ПАРИКМАРЕРСКОЙ

    /* Merge all threads to the main thread */
    for (int id = 0; id < COUNT_OF_CHAIRS_ALL; id++)
        threads[id].join();

    std::cout <<"DONE\tBARBERSHOP CLOSED"<< std::endl;

    return 0;
}