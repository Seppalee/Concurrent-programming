#include <iostream>
#include <thread>
#include <future>
#include <Windows.h>
#include <random>

bool space_pressed = false;

class Sensor 
{
public:
    Sensor()
    {
        promise = nullptr;
        sensor_t = std::thread([&] {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> distr(0, 4095);

            while (!space_pressed)
            {

                int reading = distr(gen);
                if (reading > 4000 && promise)
                {
                    promise->set_value(reading);
                    promise = nullptr;
                }
            }
        });
    }
    ~Sensor()
    {
        if (sensor_t.joinable()) sensor_t.join();
    }

    std::promise<int>* promise;
private:
    std::thread sensor_t;
};

class SensorReader {
public:
    SensorReader()
    {
        reader_t = std::thread([] {
            Sensor sensors[4];
            std::promise<int> promises[4];
            while (!space_pressed) 
            {
                for (size_t i = 0; i < 4; i++)
                {
                    auto& promise = promises[i];
                    sensors[i].promise = &promise;
                    auto future = promise.get_future();

                    auto res = future.wait_for(std::chrono::milliseconds(1));

                    if (res == std::future_status::ready)
                    {
                        const int value = future.get();
                        promise = std::promise<int>();
                        std::cout << "Reading from sensor " << i + 1 << ", value: " << value << "\n";
                    }
                }
            }
        });
    }
    ~SensorReader()
    {
        reader_t.join();
    }
private:
    std::thread reader_t;
};

int main()
{
    SensorReader sensorReader;

    while (!GetAsyncKeyState(VK_SPACE))
    {
    }
    space_pressed = true;
    std::cout << "Stopping sensors";
    return 0;
}
