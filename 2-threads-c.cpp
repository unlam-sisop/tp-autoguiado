#include <iostream>
#include <thread>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

mutex mtx;

void threadWork(int id)
{
    for (int i = 0; i < 30; i++)
    {
        mtx.lock();
        cout << "[Thread " << id << "] => generó el número " << rand() % 100 << endl;
        mtx.unlock();
    }
}

int main()
{
    thread th1(threadWork, 1);
    thread th2(threadWork, 2);
    thread th3(threadWork, 3);

    th1.join();
    th2.join();
    th3.join();

    return EXIT_SUCCESS;
}
