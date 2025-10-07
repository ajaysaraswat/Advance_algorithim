#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

const int N = 5;
mutex forks[N];

void philosopher(int id) {
    while (true) {
        cout << "Philosopher " << id << " is thinking.\n";
        this_thread::sleep_for(chrono::seconds(1));

        int left = id, right = (id + 1) % N;

        if (id == N - 1) {
            forks[right].lock(); cout << "Philosopher " << id << " picked right.\n";
            forks[left].lock();  cout << "Philosopher " << id << " picked left.\n";
        } else {
            forks[left].lock();  cout << "Philosopher " << id << " picked left.\n";
            forks[right].lock(); cout << "Philosopher " << id << " picked right.\n";
        }

        cout << "Philosopher " << id << " is eating.\n";
        this_thread::sleep_for(chrono::milliseconds(1500));

        forks[right].unlock(); cout << "Philosopher " << id << " put down right.\n";
        forks[left].unlock();  cout << "Philosopher " << id << " put down left.\n";
    }
}

int main() {
    thread p[N];
    for (int i = 0; i < N; ++i) p[i] = thread(philosopher, i);
    for (int i = 0; i < N; ++i) p[i].join();
    return 0;
}