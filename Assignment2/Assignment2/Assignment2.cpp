#include <iostream>
#include <thread>
#include <array>

using namespace std;

class Task
{
public:
    void execute(int* array)
    {
        for (int i = 0; i < sizeof(array); i++) 
        {
            array[i] = rand() % 100;
        }
    }
};

int main()
{    
    Task * taskPtr = new Task();

    int allocated;
    cout << "How many numbers to allocate: ";
    cin >> allocated;
    int *array = new int[allocated];

    thread t1(&Task::execute, taskPtr, array);
    
    t1.join();

    cout << "Your random array is \n";
    for (int i = 0; i < allocated; i++) {
        cout << array[i] << "\n";
    }

    return 0;
}