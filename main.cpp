#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

class Philosopher {
public:
    int id;
    mutex* leftFork;
    mutex* rightFork;
    int lastEat;  

    Philosopher(int i, mutex* left, mutex* right) 
        : id(i), leftFork(left), rightFork(right), lastEat(0) {}

    void eat() {
        // Reserving in one time 2 forks (deadlock, race condition)
        scoped_lock lock(*leftFork, *rightFork);

        cout << "Philosopher " << id << " start : " << endl;
        this_thread::sleep_for(chrono::seconds(1)); 
        cout << "Philosopher  " << id << " end" << endl;

        lastEat = 0;  //starvation
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
    vector<Philosopher> philosophers;
    vector<thread> threads;

    // Create Philosophers
    for (int i = 0; i < philosopherCount; i++) {
        philosophers.emplace_back(i, &forks[i], &forks[(i + 1) % philosopherCount]);
    }

    while (true) {  
        // Start eat from most hungries Philosopher
        auto& hungriest = *min_element(philosophers.begin(), philosophers.end(),
        [](const Philosopher& a, const Philosopher& b) {
            return a.lastEat > b.lastEat;
        });

        threads.emplace_back(&Philosopher::eat, &hungriest);

        // Other Philosopher are mor Hungry
        for (auto& phil : philosophers) {
            if (&phil != &hungriest) phil.lastEat++;
        }

        this_thread::sleep_for(chrono::milliseconds(1000)); 
        }

        for (auto& t : threads) {
            if (t.joinable()) t.join();
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
