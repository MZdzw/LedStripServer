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
        if (m_SerialPort >= 0)
        {
            std::cout << "Closing the serial port\n";
            close(m_SerialPort);
            m_SerialPort = -1;
        }
    }

    void Send(const std::string& data) override;
    const std::string& Read() override;

    const std::string& GetDevName() const;
    void ChangeToNonBlocking();

    bool TranslateMsgFromNetworkToUsb(std::vector<std::string>& vars);

    static void ReceiverThread(Usb& obj);
    static void SenderThread(Usb& obj);


private:
    std::string m_DevName;
    int m_SerialPort;

};

