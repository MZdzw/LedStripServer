#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <memory>
#include "Usb.hpp"
#include "Common.hpp"

enum UsbMessageE
{
    REMOVE_SECTOR, ADD_SECTOR, SET_DIODE_HSV,
    SET_DIODE_RGB, SET_SECTOR_HSV, SET_SECTOR_RGB,
    SET_SECTOR_RAINBOW, SPAWN_DIODE, SET_ANIMATION_SPEED,
    SET_ROLLING_EFFECT, SET_DIMMING_ENTIRE_EFFECT,
    SET_NO_ANIMATION, LED_STRIP_REQ, MAX
};

Usb::Usb(std::string devName)
    : ICommunication(128)
    , m_DevName(devName)
    , m_SerialPort(open(m_DevName.c_str(), O_RDWR))
{
    if (m_SerialPort < 0) {
        std::cerr << "Serial port not opened." << std::endl;
        return;
    }

    struct termios tty;
    if (tcgetattr(m_SerialPort, &tty) != 0) {
        std::cerr << "Terminal parameters not provided." << std::endl;
        return;
    }

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;

    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ISIG;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(ICRNL | INLCR);

    tty.c_oflag &= ~OPOST;

    // Turn on blocking mode (wait in read till the data come)
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;
    // Zapisz ustawienia portu
    if (tcsetattr(m_SerialPort, TCSANOW, &tty) != 0) {
        std::cerr << "Terminal parameters not set." << std::endl;
        return;
    }
}

void Usb::Send(const std::string& data)
{
    // Wysyłanie danych
    int bytes_written = write(m_SerialPort, data.c_str(), data.length());
    
    // Sprawdź, czy dane zostały wysłane
    if (bytes_written < 0)
    {
        std::cerr << "Data not send correctly." << std::endl;
    }
    else
    {
        std::cout << "Send " << bytes_written << " bytes." << std::endl;
    }
}

const std::string& Usb::Read()
{
    char buffer[512];
    ssize_t num_bytes = read(m_SerialPort, buffer, 512);

    if (num_bytes > 0)
    {
        buffer[511] = '\0';
        std::cout << "Received: ";
        for (int i = 0; i < num_bytes; i++)
        {
            std::cout << static_cast<int>(buffer[i]) << "-";
        }
        std::cout << "\n";
    }
    GetBuffer() = std::string(buffer, 512);
    return GetBuffer();
}

const std::string& Usb::GetDevName() const
{
    return m_DevName;
}

void Usb::ChangeToNonBlocking()
{
    // Get flags and set nonblocking mode
    int flags = fcntl(m_SerialPort, F_GETFL, 0);
    fcntl(m_SerialPort, F_SETFL, flags | O_NONBLOCK);

    // Get set attributes and turn on nonblocking mode
    struct termios options;
    tcgetattr(m_SerialPort, &options);
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 0;
    tcsetattr(m_SerialPort, TCSANOW, &options);
}

bool Usb::TranslateMsgFromNetworkToUsb(std::vector<std::string> &vars)
{
    std::string& networkMessage = g_Msg;

    if ((*networkMessage.begin() != 'A') | (*(networkMessage.end() - 1) != 'A'))
    {
        return false;
    }

    if ((*(networkMessage.begin() + 1) != '-') | (*(networkMessage.end() - 2) != '-'))
    {
        return false;
    }

    std::vector<std::string> numbers;
    std::string number;
    for (auto it = (networkMessage.begin() + 1); it != (networkMessage.end() - 1); it++)
    {
        if (*it == '-')
        {
            if (!number.empty())
            {
                numbers.push_back(number);
                number.clear();
            }
        }
        else 
        {
            number.push_back(*it);
        }
    }
    std::cout << "Send numbers in message: \n";
    for (const auto& it : numbers)
    {
        std::cout << it << " ";
    }
    std::cout << "\n";
    vars = numbers;

    if (stoi(numbers[0]) >= UsbMessageE::MAX)
    {
        return false;
    }

    if (stoi(numbers[0]) == UsbMessageE::REMOVE_SECTOR)
    {
        if (numbers.size() != 2)
        {
            return false;
        }
        return true;
    }

    if (stoi(numbers[0]) == UsbMessageE::ADD_SECTOR)
    {
        if (numbers.size() != 6)
        {
            return false;
        }
        return true;
    }

    if (stoi(numbers[0]) == UsbMessageE::SET_DIODE_HSV)
    {
        if (numbers.size() != 7)
        {
            return false;
        }
        return true;
    }

    if (stoi(numbers[0]) == UsbMessageE::SET_DIODE_RGB)
    {
        if (numbers.size() != 6)
        {
            return false;
        }
        return true;
    }

    if (stoi(numbers[0]) == UsbMessageE::SET_SECTOR_HSV)
    {
        if (numbers.size() != 6)
        {
            return false;
        }
        return true;
    }

    if (stoi(numbers[0]) == UsbMessageE::SET_SECTOR_RGB)
    {
        if (numbers.size() != 5)
        {
            return false;
        }
        return true;
    }

    if (stoi(numbers[0]) == UsbMessageE::SET_SECTOR_RAINBOW)
    {
        if (numbers.size() != 2)
        {
            return false;
        }
        return true;
    }

    if (stoi(numbers[0]) == UsbMessageE::SPAWN_DIODE)
    {
        if (numbers.size() != 5)
        {
            return false;
        }
        return true;
    }

    if (stoi(numbers[0]) == UsbMessageE::SET_ANIMATION_SPEED)
    {
        if (numbers.size() != 6)
        {
            return false;
        }
        return true;
    }

    if (stoi(numbers[0]) == UsbMessageE::SET_ROLLING_EFFECT)
    {
        if (numbers.size() != 2)
        {
            return false;
        }
        return true;
    }

    if (stoi(numbers[0]) == UsbMessageE::SET_DIMMING_ENTIRE_EFFECT)
    {
        if (numbers.size() != 2)
        {
            return false;
        }
        return true;
    }

    if (stoi(numbers[0]) == UsbMessageE::LED_STRIP_REQ)
    {
        if (numbers.size() != 1)
        {
            return false;
        }
        return true;
    }

    return false;
}

void Usb::ReceiverThread(Usb& obj)
{

    //std::unique_lock<std::mutex> lock(g_Mutex);
    while(1)
    {
        obj.Read();

        // if we close the port
        if (!g_Session)
        {
            break;
        }
    }
}

void Usb::SenderThread(Usb& obj)
{
    std::vector<std::string> usbVars;
    std::vector<char> usbVarsNum;
    bool areValuesValid = false;
    std::unique_lock<std::mutex> lock(g_Mutex);
    while(1)
    {   
        usbVars.clear();
        usbVarsNum.clear();
        g_Cv.wait(lock);
        areValuesValid = obj.TranslateMsgFromNetworkToUsb(usbVars);
        if (areValuesValid)
        {
            for (int i = 0; i < 6; i++)
                usbVarsNum.push_back(0xAA);
            for (auto& var : usbVars)
            {
                usbVarsNum.push_back(stoi(var));
            }
            for (int i = 0; i < 6; i++)
                usbVarsNum.push_back(0xAA);

            std::cout << "Sending USB data...\n";
            obj.Send(std::string(usbVarsNum.begin(), usbVarsNum.end()));
        }
        if (!g_Session)
        {
            obj.ChangeToNonBlocking();
            break;
        }
    }
}

