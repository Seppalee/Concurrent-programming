#include <iostream>
#include <vector>
#include <thread>

using namespace std;

void helpFn() {
    for (int i = 0; i < 1000; i++) {
        cout << i;
    }
}

int main() {
    const auto numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(std::thread(helpFn));
    }
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}