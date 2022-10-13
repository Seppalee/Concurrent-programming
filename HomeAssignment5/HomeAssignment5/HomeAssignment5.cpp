#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <chrono>

std::condition_variable cv1, cv2, cv3, cv4, cv5;

void counter()
{
    for (int i = 0; i <= 100; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        switch (i)
        {
        case 20:
            cv1.notify_one();
        case 40:
            cv2.notify_one();
        case 60:
            cv3.notify_one();
        case 80:
            cv4.notify_one();
        case 100:
            cv5.notify_one();
        }
    }
}

int main()
{
    using namespace std;
    mutex m;

    thread t1(counter);

    unique_lock<mutex> lock(m);
    cv1.wait(lock);
    cout << "20 %" << "\n";
    cv2.wait(lock);
    cout << "40 %" << "\n";
    cv3.wait(lock);
    cout << "60 %" << "\n";
    cv4.wait(lock);
    cout << "80 %" << "\n";
    cv5.wait(lock);
    cout << "100 %" << "\n";

    t1.join();

    return 0;
}