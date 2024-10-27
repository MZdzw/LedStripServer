#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <fstream>
#include "Gpio.hpp"

const std::string LedPath = "/sys/class/gpio/gpio60";

static void writeLED(State_e state)
{
    std::fstream file;
    file.open(LedPath + "/value", std::ios::out);
    file << ((state == State_e::HIGH) ? "1" : "0");
    file.flush();
    file.close();
}

void BlinkingLEDThread()
{
    using namespace std::chrono_literals;
    std::cout << "Beginning of foo thread\n";

    while (1)
    {
        writeLED(State_e::HIGH);
        std::this_thread::sleep_for(1000ms);
        writeLED(State_e::LOW);
        std::this_thread::sleep_for(1000ms);
    }
}
