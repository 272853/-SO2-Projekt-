#ifndef FIRSTSO_H
#define FIRSTSO_H

class Philosopher {
public:
    int Id;
    bool leftFork, rightFork;

    Philosopher(int getId);
    void state();
};

#endif // FIRSTSO_H
