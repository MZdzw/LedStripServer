#include <thread>
#include "Peripherals/Gpio.hpp"
#include "Server/server.hpp"

#define LED_PATH "/sys/class/gpio/gpio60"


int main(void)
{
    // std::thread gpioThread(&BlinkingLEDThread);
    std::thread serverThread(&Server::ServerThread);

    // gpioThread.join();
    serverThread.join();

    return 0;
}
