#include <thread>
#include "Peripherals/Gpio.hpp"
#include "Network/server.hpp"

#define LED_PATH "/sys/class/gpio/gpio60"


int main(void)
{
    // std::thread gpioThread(&BlinkingLEDThread);
    std::thread tcpThreadsHandler(&ServerTCP::TcpHandlerThread);

    // gpioThread.join();
    tcpThreadsHandler.join();

    return 0;
}
