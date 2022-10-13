#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

class Task
{
public:
    void addNumber(std::vector<int>& intVector)
    {
        m.lock();
        for (int i = 0; i < 100; i++)
        {
        intVector.push_back(i);
        }
        m.unlock();
        
    }
    void removeNumber(std::vector<int>& intVector)
    {
        m.lock();
        intVector.clear();
        m.unlock();
    }

    std::mutex m;
};

int main()
{
    Task* taskPtr = new Task();
    
    std::vector<int> intVector;

    std::thread t1(&Task::addNumber, taskPtr, ref(intVector));
    std::thread t2(&Task::removeNumber, taskPtr, ref(intVector));

    t1.join();
    t2.join();

    for (int i = 0; i < intVector.size(); i++) 
    {
        std::cout << intVector[i] << "\n";
    }

    return 0;
}