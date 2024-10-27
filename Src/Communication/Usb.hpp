#pragma once
#include <string>
#include "Communication.hpp"
#include <vector>

class Usb : public ICommunication
{
public:
    Usb() = delete;
    Usb(std::string devName);
    ~Usb()
    {
        std::cout << "Closing the serial port\n";
        close(m_SerialPort);
    }

    void Send(const std::string& data) override;
    const std::string& Read() override;

    const std::string& GetDevName() const;
    bool TranslateMsgFromNetworkToUsb(std::vector<std::string>& vars);


    static void SenderThread(Usb& obj);


private:
    std::string m_DevName;
    int m_SerialPort;

};

