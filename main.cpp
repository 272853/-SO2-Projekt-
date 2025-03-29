#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstdlib>

using namespace std;


// Philosopher class
class Philosopher {
public:
    int id;
    bool leftFork;
    bool rightFork;
    int lastEat;

    bool canEat()
        {
            if(leftFork==true && rightFork==true){
                return true;
             } 
             else{
                return false; 
             }   
        }
    

};

int nextPhilosopher(int philosopherId, int philosopherCount);

int beforePhilosopher(int philosopherId, int philosopherCount);

// Quit program enytime
void mainThreadFunction();

int main(int argc, char* argv[]) 
{
    thread mainThread(mainThreadFunction);
    // { 

        int philosopherCount = stoi(argv[1]);

        if(philosopherCount>8){
            cout<<"Argument is too big, must be less or equal 8"<<endl;
            cout<<"--------------------------------------------"<<endl;
            cout<<"Press q to quit"<<endl;
        }
        else{
            // Create Philosophers 
            vector<Philosopher> philosophers(philosopherCount);
            for (int i = 0; i < philosopherCount; i++) {
                philosophers[i].id = i + 1;
                philosophers[i].leftFork = false;
                philosophers[i].rightFork = false;
                philosophers[i].lastEat=1;
            }
            
            int hungriestPhilosopherId = philosophers[0].id;
            int maxLastEat = philosophers[0].lastEat;

            while(true){

                for (int i = 1; i < philosopherCount; i++) {
                    if (philosophers[i].lastEat > maxLastEat) {
                        maxLastEat = philosophers[i].lastEat;
                        hungriestPhilosopherId = philosophers[i].id;
                    }
                }


            }

        }

    // }
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

int nextPhilosopher(int philosopherId, int philosopherCount)
{
    if(philosopherId==philosopherCount){
        return 1;
    }
    else{
        return philosopherId+1;
    }
}

int beforePhilosopher(int philosopherId, int philosopherCount)
{
    if(philosopherId=1){
        return philosopherCount;
    }
    else{
        return philosopherId-1;
    }
}