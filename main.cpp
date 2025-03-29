#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

using namespace std;

class Philosopher {
public:
    int id;
    mutex* leftFork;
    mutex* rightFork;

    Philosopher(int i, mutex* left, mutex* right) 
        : id(i), leftFork(left), rightFork(right) {}

    void eat() {
        // Release and block acces
        lock_guard<mutex> leftLock(*leftFork);
        lock_guard<mutex> rightLock(*rightFork);

        cout << "Philosopher " << id << " start : " << endl;
        this_thread::sleep_for(chrono::seconds(1)); 
        cout << "Philosopher  " << id << " end" << endl;
    }
};

// Quit program enytime
void mainThreadFunction();

int main(int argc, char* argv[]) {
    
    thread mainThread(mainThreadFunction);

    while(true)
    {
        int philosopherCount = stoi(argv[1]);
        if (philosopherCount > 8) {
            cout << "Argumet must less than 9" << endl;
            return 1;
        }

        vector<mutex> forks(philosopherCount); 
        vector<Philosopher> philosophers;
        vector<thread> threads;

        // Create Philosophers
        for (int i = 0; i < philosopherCount; i++) {
            philosophers.emplace_back(i, &forks[i], &forks[(i + 1) % philosopherCount]);
        }

        // Start eating
        for (int i = 0; i < philosopherCount; i++) {
            threads.emplace_back(&Philosopher::eat, &philosophers[i]);
        }

        for (auto& t : threads) {
            t.join();
        }
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
