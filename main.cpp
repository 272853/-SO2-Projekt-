#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstdlib>

using namespace std;

class Philosopher {
public:
    int id;
    bool leftFork;
    bool rightFork;


    

};

// Quit program enytime
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

int main(int argc, char* argv[]) {
    thread mainThread(mainThreadFunction);
    // { 
        
        while(true)
        {
            cout<<"Do..."<<endl;
            _sleep(1000);
        }


    // }
    mainThread.join();
    return 0;
}
