#include <iostream>
#include <random>
#include <chrono>
#include <future>

class RandomNumberGenerator
{
public:
    enum TaskType
    {
        LIGHT,
        HEAVY
    };

    RandomNumberGenerator(TaskType type) : m_type(type) {}

    void generateLight()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);

        for (int i = 0; i < 100; i++)
        {
            auto random_number = dis(gen);
        }
    }

    void generateHeavy()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);

        for (int i = 0; i < 10000000; i++)
        {
            auto random_number = dis(gen);
        }
    }

    TaskType type() const { return m_type; }

private:
    TaskType m_type;
};

int main()
{
    std::vector<RandomNumberGenerator> generators;
    for (int i = 0; i < 100; i++)
    {
        RandomNumberGenerator::TaskType type = (std::rand() % 2 == 0) ? RandomNumberGenerator::LIGHT : RandomNumberGenerator::HEAVY;
        generators.emplace_back(type);
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::future<void>> tasks;
    for (auto& generator : generators)
    {
        /*
        if (generator.type() == RandomNumberGenerator::LIGHT)
        {
            tasks.emplace_back(std::async(std::launch::deferred, &RandomNumberGenerator::generateLight, generator));
        }
        else
        {
            tasks.emplace_back(std::async(std::launch::async, &RandomNumberGenerator::generateHeavy, generator));
        }
    */
        if (generator.type() == RandomNumberGenerator::LIGHT)
        {
            tasks.emplace_back(std::async(std::launch::async, &RandomNumberGenerator::generateLight, generator));
        }
        else
        {
            tasks.emplace_back(std::async(std::launch::async, &RandomNumberGenerator::generateHeavy, generator));
        }
    }

    for (auto& task : tasks)
    {
        task.wait();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Elapsed time: " << elapsed.count() << " ms" << std::endl;

    // I did 10 test runs with both options. 
    // All using async was average 6050ms and median was 6191ms
    // Deferred and async together was average 6168ms and median was 6259ms
    // I believe all asynced is faster, because it allows all tasks to run concurrently on separate threads
    // whereas deferred causes the task to be deferred until result is needed. Meaning the task will not be
    // executed until the call to wait on the resulting future object.

    return 0;
}