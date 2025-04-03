#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <memory>
#include <algorithm>
#include <array>
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








// static std::vector<std::string> SplitMsg(const std::string& str, const std::string& delimeter)
// {
//     std::vector<std::string> tokens;
//     size_t foundPos = 0;
//     size_t lastPos = 0;
// 
//     foundPos = str.find(delimeter, lastPos);
//     while (foundPos != std::string::npos)
//     {
//         if (foundPos != lastPos)
//         {
//             tokens.push_back(str.substr(lastPos, foundPos - lastPos));
//         }
//         lastPos = foundPos + 1;
//         foundPos = str.find(delimeter, lastPos);
//     }
//     if (lastPos != str.length())
//     {
//         tokens.push_back(str.substr(lastPos, str.length() - lastPos));
//     }
//     return tokens;
// }
// 
// Usb::Usb(std::string devName)
//     : ICommunication(128)
//     , m_DevName(devName)
//     , m_SerialPort(open(m_DevName.c_str(), O_RDWR))
// {
//     if (m_SerialPort < 0) {
//         std::cerr << "Serial port not opened." << std::endl;
//         return;
//     }
// 
//     struct termios tty;
//     if (tcgetattr(m_SerialPort, &tty) != 0) {
//         std::cerr << "Terminal parameters not provided." << std::endl;
//         return;
//     }
// 
//     tty.c_cflag &= ~PARENB;
//     tty.c_cflag &= ~CSTOPB;
//     tty.c_cflag &= ~CSIZE;
//     tty.c_cflag |= CS8;
//     tty.c_cflag &= ~CRTSCTS;
//     tty.c_cflag |= CREAD | CLOCAL;
// 
//     cfsetispeed(&tty, B9600);
//     cfsetospeed(&tty, B9600);
// 
//     tty.c_lflag &= ~ICANON;
//     tty.c_lflag &= ~ECHO;
//     tty.c_lflag &= ~ECHOE;
//     tty.c_lflag &= ~ISIG;
// 
//     tty.c_iflag &= ~(IXON | IXOFF | IXANY);
//     tty.c_iflag &= ~(ICRNL | INLCR);
// 
//     tty.c_oflag &= ~OPOST;
// 
//     // Turn on blocking mode (wait in read till the data come)
//     tty.c_cc[VMIN] = 1;
//     tty.c_cc[VTIME] = 0;
//     // Zapisz ustawienia portu
//     if (tcsetattr(m_SerialPort, TCSANOW, &tty) != 0) {
//         std::cerr << "Terminal parameters not set." << std::endl;
//         return;
//     }
// }
// 
// void Usb::Send(const std::string& data)
// {
//     // Wysyłanie danych
//     int bytes_written = write(m_SerialPort, data.c_str(), data.length());
//     
//     // Sprawdź, czy dane zostały wysłane
//     if (bytes_written < 0)
//     {
//         std::cerr << "Data not send correctly." << std::endl;
//     }
//     else
//     {
//         std::cout << "Send " << bytes_written << " bytes." << std::endl;
//     }
// }
// 
// const std::string& Usb::Read()
// {
//     char buffer[512];
//     ssize_t num_bytes = read(m_SerialPort, buffer, 512);
// 
//     if (num_bytes > 0)
//     {
//         buffer[511] = '\0';
//         std::cout << "Received: ";
//         for (int i = 0; i < num_bytes; i++)
//         {
//             std::cout << static_cast<int>(buffer[i]) << "-";
//         }
//         std::cout << "\n";
//     }
//     GetBuffer() = std::string(buffer, 512);
//     return GetBuffer();
// }
// 
// const std::string& Usb::GetDevName() const
// {
//     return m_DevName;
// }
// 
// void Usb::ChangeToNonBlocking()
// {
//     // Get flags and set nonblocking mode
//     int flags = fcntl(m_SerialPort, F_GETFL, 0);
//     fcntl(m_SerialPort, F_SETFL, flags | O_NONBLOCK);
// 
//     // Get set attributes and turn on nonblocking mode
//     struct termios options;
//     tcgetattr(m_SerialPort, &options);
//     options.c_cc[VMIN] = 0;
//     options.c_cc[VTIME] = 0;
//     tcsetattr(m_SerialPort, TCSANOW, &options);
// }
// 
// bool Usb::TranslateMsgFromNetworkToUsb(std::vector<std::string>& vars)
// {
//     std::string& networkMessage = g_Msg;
//     vars = SplitMsg(networkMessage, "-");
//     std::array<std::vector<std::string>, 5> multipleMsgsTokens;
//     int index = 0;
//     std::for_each(vars.begin(), vars.end(), [&multipleMsgsTokens, &index](const std::string& token){
//         if (token != "AA")
//         {
//             multipleMsgsTokens[index].push_back(token);
//         }
//         else
//         {
//             multipleMsgsTokens[index].push_back("A");
//             multipleMsgsTokens[++index].push_back("A");
//         }
//     });
// 
//     std::cout << "Multiple Msgs------\n";
//     for (const auto& msgTokens : multipleMsgsTokens)
//     {
//         if (msgTokens.size() != 0)
//             std::cout << "\n";
//         for (const auto& tokens : msgTokens)
//         {
//             std::cout << tokens << " ";
//         }
//     }
// 
//     if ((*vars.begin() != "A") | (*(vars.end() - 1) != "A"))
//     {
//         return false;
//     }
// 
//     if (stoi(vars[1]) >= UsbMessageE::MAX)
//     {
//         return false;
//     }
// 
//     if (stoi(vars[1]) == UsbMessageE::REMOVE_SECTOR)
//     {
//         if (vars.size() != 4)
//         {
//             return false;
//         }
//         return true;
//     }
// 
//     if (stoi(vars[1]) == UsbMessageE::ADD_SECTOR)
//     {
//         if (vars.size() != 8)
//         {
//             return false;
//         }
//         return true;
//     }
// 
//     if (stoi(vars[1]) == UsbMessageE::SET_DIODE_HSV)
//     {
//         if (vars.size() != 9)
//         {
//             return false;
//         }
//         return true;
//     }
// 
//     if (stoi(vars[1]) == UsbMessageE::SET_DIODE_RGB)
//     {
//         if (vars.size() != 8)
//         {
//             return false;
//         }
//         return true;
//     }
// 
//     if (stoi(vars[1]) == UsbMessageE::SET_SECTOR_HSV)
//     {
//         if (vars.size() != 8)
//         {
//             return false;
//         }
//         return true;
//     }
// 
//     if (stoi(vars[1]) == UsbMessageE::SET_SECTOR_RGB)
//     {
//         if (vars.size() != 7)
//         {
//             return false;
//         }
//         return true;
//     }
// 
//     if (stoi(vars[1]) == UsbMessageE::SET_SECTOR_RAINBOW)
//     {
//         if (vars.size() != 9)
//         {
//             return false;
//         }
//         return true;
//     }
// 
//     if (stoi(vars[1]) == UsbMessageE::SPAWN_DIODE)
//     {
//         if (vars.size() != 7)
//         {
//             return false;
//         }
//         return true;
//     }
// 
//     if (stoi(vars[1]) == UsbMessageE::SET_ANIMATION_SPEED)
//     {
//         if (vars.size() != 8)
//         {
//             return false;
//         }
//         return true;
//     }
// 
//     if (stoi(vars[1]) == UsbMessageE::SET_ROLLING_EFFECT)
//     {
//         if (vars.size() != 4)
//         {
//             return false;
//         }
//         return true;
//     }
// 
//     if (stoi(vars[1]) == UsbMessageE::SET_DIMMING_ENTIRE_EFFECT)
//     {
//         if (vars.size() != 4)
//         {
//             return false;
//         }
//         return true;
//     }
// 
//     if (stoi(vars[1]) == UsbMessageE::LED_STRIP_REQ)
//     {
//         if (vars.size() != 3)
//         {
//             return false;
//         }
//         return true;
//     }
// 
//     return false;
// }
// 
// void Usb::ReceiverThread(Usb& obj)
// {
// 
//     //std::unique_lock<std::mutex> lock(g_Mutex);
//     while(1)
//     {
//         obj.Read();
// 
//         // if we close the port
//         if (!g_Session)
//         {
//             break;
//         }
//     }
// }
// 
// void Usb::SenderThread(Usb& obj)
// {
//     std::vector<std::string> usbVars;
//     std::vector<char> usbVarsNum;
//     bool areValuesValid = false;
//     std::unique_lock<std::mutex> lock(g_Mutex);
//     while(1)
//     {   
//         usbVars.clear();
//         usbVarsNum.clear();
//         g_Cv.wait(lock);
//         areValuesValid = obj.TranslateMsgFromNetworkToUsb(usbVars);
//         if (areValuesValid)
//         {
//             for (int i = 0; i < 6; i++)
//                 usbVarsNum.push_back(0xAA);
//             for (auto it = usbVars.begin() + 1; it != usbVars.end() - 1; it++)
//             {
//                 usbVarsNum.push_back(stoi(*it));
//             }
//             for (int i = 0; i < 6; i++)
//                 usbVarsNum.push_back(0xAA);
// 
//             std::cout << "Sending USB data...\n";
//             obj.Send(std::string(usbVarsNum.begin(), usbVarsNum.end()));
//         }
//         if (!g_Session)
//         {
//             obj.ChangeToNonBlocking();
//             break;
//         }
//     }
// }
// 
//

void UsbConfigurator::Send()
{
    int bytes_written = write(m_UsbData.serialPort,
                              GetSendBuffer().c_str(),
                              GetSendBuffer().length());
    
    if (bytes_written < 0)
    {
        std::cout << "Data not send correctly." << std::endl;
    }
    else
    {
        std::cout << "Send " << bytes_written << " bytes." << std::endl;
    }
}

void UsbConfigurator::Read()
{
    char buffer[128];
    ssize_t num_bytes = read(m_UsbData.serialPort, buffer, 128);

    if (num_bytes > 0)
    {
        buffer[127] = '\0';
        std::cout << "Received: ";
        for (int i = 0; i < num_bytes; i++)
        {
            std::cout << static_cast<int>(buffer[i]) << "-";
        }
        std::cout << "\n";
    }
    GetReceptionBuffer() = std::string(buffer, 128);
}

int UsbConfigurator::GetElapsedTimeBetweenRead() const
{
    return 0;
}

bool UsbConfigurator::IsConnected() const
{
    // TODO: proper return
    return false;
}

void UsbConfigurator::ChangeMode(UsbModeE mode)
{
    if (mode == UsbModeE::NONBLOCKING)
    {
        // Get flags and set nonblocking mode
        int flags = fcntl(m_UsbData.serialPort, F_GETFL, 0);
        fcntl(m_UsbData.serialPort, F_SETFL, flags | O_NONBLOCK);

        // Get set attributes and turn on nonblocking mode
        struct termios options;
        tcgetattr(m_UsbData.serialPort, &options);
        options.c_cc[VMIN] = 0;
        options.c_cc[VTIME] = 0;
        tcsetattr(m_UsbData.serialPort, TCSANOW, &options);
    }
}

void Usb::Send()
{
}

void Usb::Read()
{
}

bool Usb::TranslateMsgToUsb(const std::string &tcpIpMsg)
{
    return false;
}

void Usb::UsbSenderThread()
{
}

void Usb::UsbReaderThread()
{
}
