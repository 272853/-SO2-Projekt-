#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <atomic>

using namespace std;

class Philosopher {
public:
    int id;
    mutex* leftFork;
    mutex* rightFork;
    atomic<int>* lastEat;  // Race condition

    Philosopher(int i, mutex* left, mutex* right, atomic<int>* lastEatTime)
        : id(i), leftFork(left), rightFork(right), lastEat(lastEatTime) {}

    //Main function checking if philosopher can eat and set state of hunger
    void dine() {
        while (true) {
            // deadlock
            if (leftFork->try_lock()) {
                if (rightFork->try_lock()) {
                    
                    cout << "Philosopher " << id << " start : " << endl;
                    this_thread::sleep_for(chrono::seconds(1)); 
                    cout << "Philosopher  " << id << " end" << endl;

                    // starvation
                    *lastEat = 0;

                    // free forks
                    rightFork->unlock();
                }
                leftFork->unlock();
            }

            // more hungry
            (*lastEat)++;

            this_thread::sleep_for(chrono::milliseconds(1000));
        }
    }
};

// Quit program enytime
void mainThreadFunction();

int main(int argc, char* argv[]) {
    thread mainThread(mainThreadFunction);

    int philosopherCount = stoi(argv[1]);
    if (philosopherCount > 8) {
        cout << "Argumet must less than 9" << endl;
        return 1;
    }

    vector<mutex> forks(philosopherCount);
    vector<atomic<int>> lastEatTimes(philosopherCount);
    vector<Philosopher> philosophers;
    vector<thread> threads;

    //Creating Pilosophers and assigment forks
    for (int i = 0; i < philosopherCount; i++) {
        philosophers.emplace_back(i, &forks[i], &forks[(i + 1) % philosopherCount], &lastEatTimes[i]);
    }

    //Creating Threads 
    for (int i = 0; i < philosopherCount; i++) {
        threads.emplace_back(&Philosopher::dine, &philosophers[i]);
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}

void mainThreadFunction()
{
    string quit;
    while(true){
        getline(cin, quit);
        if(quit=="q"||quit=="Q") {
            exit(0); 
        }
    }
}
