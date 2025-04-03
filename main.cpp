#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <atomic>

using namespace std;

atomic<bool> running(true);
mutex priorityMutex;

class Philosopher {
public:
    int id;
    mutex* leftFork;
    mutex* rightFork;
    atomic<int>* lastEat;

    Philosopher(int i, mutex* left, mutex* right, atomic<int>* lastEatTime)
        : id(i), leftFork(left), rightFork(right), lastEat(lastEatTime) {}

    void dine(vector<atomic<int>>* allLastEats) {
        while (running.load()) {
            // Chceck priority
            priorityMutex.lock();
            bool canEat = true;
            int myHunger = lastEat->load();

            for (size_t i = 0; i < allLastEats->size(); ++i) {
                if (i != id && (*allLastEats)[i].load() > myHunger) {
                    canEat = false;
                    break;
                }
            }
            priorityMutex.unlock();

            if (!canEat) {
                (*lastEat)++;
                this_thread::sleep_for(chrono::milliseconds(500));
                continue;
            }

            // Picking Forks
            if (leftFork->try_lock()) {
                if (rightFork->try_lock()) {
                    cout << "Philosopher " << id << " starts eating" << endl;
                    this_thread::sleep_for(chrono::seconds(1));
                    cout << "Philosopher " << id << " finished eating" << endl;

                    *lastEat = 0;  // Reset Starvation

                    rightFork->unlock();
                }
                leftFork->unlock();
            }

            (*lastEat)++;
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    }
};

// Quit program enytime
void mainThreadFunction();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <number_of_philosophers>" << endl;
        return 1;
    }

    int philosopherCount = stoi(argv[1]);
    if (philosopherCount > 8) {
        cout << "Argument must be less than 9" << endl;
        return 1;
    }

    thread mainThread(mainThreadFunction);

    vector<mutex> forks(philosopherCount);
    vector<atomic<int>> lastEatTimes(philosopherCount);
    vector<Philosopher> philosophers;
    vector<thread> threads;

    //Creating Pilosophers and assigment forks
    for (int i = 0; i < philosopherCount; i++) {
        lastEatTimes[i] = 0;
        philosophers.emplace_back(i, &forks[i], &forks[(i + 1) % philosopherCount], &lastEatTimes[i]);
    }

    //Creating Threads 
    for (int i = 0; i < philosopherCount; i++) {
        threads.emplace_back(&Philosopher::dine, &philosophers[i], &lastEatTimes);
    }

    for (auto& t : threads) {
        t.join();
    }

    mainThread.join();

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
