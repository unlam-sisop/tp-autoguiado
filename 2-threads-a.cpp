#include <iostream>
#include <thread>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

void threadWork(int id)
{
    while (true)
    {
        cout << "[Thread " << id << "] => generó el número " << rand() << endl;
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
