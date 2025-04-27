#pragma once
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

struct TerminalInterfaceT
{
    termios settings;
    std::string devName;
    int serialPort;
    bool isConnected;

    TerminalInterfaceT()
    {
        serialPort = open("/dev/ttyACM0", O_RDWR);
        if (serialPort < 0)
        {
            std::cout << "Serial port not opened." << std::endl;
            return;
        }

        struct termios settings;
        if (tcgetattr(serialPort, &settings) != 0) {
            std::cout << "Terminal parameters not provided." << std::endl;
            return;
        }

        settings.c_cflag &= ~PARENB;
        settings.c_cflag &= ~CSTOPB;
        settings.c_cflag &= ~CSIZE;
        settings.c_cflag |= CS8;
        settings.c_cflag &= ~CRTSCTS;
        settings.c_cflag |= CREAD | CLOCAL;

        cfsetispeed(&settings, B9600);
        cfsetospeed(&settings, B9600);

        settings.c_lflag &= ~ICANON;
        settings.c_lflag &= ~ECHO;
        settings.c_lflag &= ~ECHOE;
        settings.c_lflag &= ~ISIG;

        settings.c_iflag &= ~(IXON | IXOFF | IXANY);
        settings.c_iflag &= ~(ICRNL | INLCR);

        settings.c_oflag &= ~OPOST;

        // Turn on blocking mode (wait in read till the data come)
        settings.c_cc[VMIN] = 1;
        settings.c_cc[VTIME] = 0;
        // Zapisz ustawienia portu
        if (tcsetattr(serialPort, TCSANOW, &settings) != 0)
        {
            std::cout << "Terminal parameters not set." << std::endl;
            return;
        }
        isConnected = true;
    }
};
