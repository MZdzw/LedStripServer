#ifndef __SERVER_HPP__
#define __SERVER_HPP__
#include "TcpIp.hpp"
#include "Usb.hpp"

class Server
{
private:
    TcpIp& m_TcpIpConnection;
    Usb& m_UsbConnection;
    int m_Id;

public:
    Server(TcpIp& tcpIpConnection, Usb& usbConnection, int id)
    : m_TcpIpConnection(tcpIpConnection), m_UsbConnection(usbConnection), m_Id(id)
    {
    }

    ~Server()
    {
    }

    void ResetClientData();

    void ConnectionManager();
    static void ServerThread();
};

#endif
