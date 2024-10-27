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
    SET_ROLLING_EFFECT, SET_DIMMING_ENTIRE_EFFECT, MAX
};

Usb::Usb(std::string devName)
    : ICommunication(128)
    , m_DevName(devName)
    , m_SerialPort(open(m_DevName.c_str(), O_RDWR))
{
    // Sprawdź, czy otwarcie portu zakończyło się sukcesem
    if (m_SerialPort < 0) {
        std::cerr << "Serial port not opened." << std::endl;
        return;
    }

    // Konfiguracja parametrów połączenia szeregowego
    struct termios tty;
    if (tcgetattr(m_SerialPort, &tty) != 0) {
        std::cerr << "Terminal parameters not provided." << std::endl;
        return;
    }

    tty.c_cflag &= ~PARENB; // Brak parzystości
    tty.c_cflag &= ~CSTOPB; // Jeden bit stopu
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;     // 8 bitów danych
    tty.c_cflag &= ~CRTSCTS; // Wyłącz kontrolę przepływu sprzętowego
    tty.c_cflag |= CREAD | CLOCAL; // Włącz odczyt i wyłącz kontrolę terminala

    // Ustaw prędkość transmisji na 9600 baud
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    // Ustawienia bez trybu kanonicznego (natychmiastowy odczyt danych)
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;   // Wyłącz echo
    tty.c_lflag &= ~ECHOE;  // Wyłącz echo backspace
    tty.c_lflag &= ~ISIG;   // Wyłącz obsługę sygnałów (Ctrl+C)

    // Wyłącz kontrolę przepływu programowego (XON/XOFF)
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(ICRNL | INLCR); // Wyłącz zamianę znaków CR/NL

    tty.c_oflag &= ~OPOST; // Wyłącz przetwarzanie wyjścia

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

}

const std::string& Usb::GetDevName() const
{
    return m_DevName;
}

bool Usb::TranslateMsgFromNetworkToUsb(std::vector<std::string>& vars)
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
        if (numbers.size() != 10)
        {
            return false;
        }
        return true;
    }

    if (stoi(numbers[0]) == UsbMessageE::SET_DIODE_HSV)
    {
        if (numbers.size() != 9)
        {
            return false;
        }
        return true;
    }

    if (stoi(numbers[0]) == UsbMessageE::SET_DIODE_RGB)
    {
        if (numbers.size() != 8)
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

    return false;
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
            break;
        }
    }
}

