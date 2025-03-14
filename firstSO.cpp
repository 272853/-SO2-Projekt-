#include <iostream>
#include "firstSO.h"

using namespace std;

Philosopher::Philosopher(int getId) : Id(getId), leftFork(true), rightFork(false) {}

void Philosopher::state() {
    if (leftFork && rightFork) {
        cout << Id << " is Eating" << endl;
    } else {
        cout << Id << " isn't Eating" << endl;
    }
}
