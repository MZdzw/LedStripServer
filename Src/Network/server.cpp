#include <iostream>
#include <thread>
#include <cstring>
#include <fstream>
#include "server.hpp"
#include "Network.hpp"
#include "Usb.hpp"

std::mutex Mutex;
std::condition_variable Cv;

std::string g_MsgToClose;
int g_PortNumber {};
std::string g_SerialName;


void ServerTCP::ListenConnections()
{
    listen(m_Socketfd, 5);
    m_ClientSocketfd = accept(m_Socketfd, nullptr, nullptr);
    m_IsConnected = true;
}


void ServerTCP::TcpHandlerThread()
{
    ReadConfContent();
    ServerTCP server(g_PortNumber);
    for (;;)
    {
        server.ListenConnections();
        Network obj(server.GetSocket());
        Usb usbObj(g_SerialName);
        std::thread receiveTcpThread(&Network::ReceiverThread, std::ref(obj));
        std::thread senderUsbThread(&Usb::SenderThread, std::ref(usbObj));
        std::thread receiverUsbThread(&Usb::ReceiverThread, std::ref(usbObj));

        receiveTcpThread.join();
        senderUsbThread.join();
        receiverUsbThread.join();
        std::cout << "End of process\n";
        break;
    }
}

void ServerTCP::ReadConfContent()
{
    std::ifstream file("conf.txt");
    if (!file.is_open())
    {
        std::cout << "Error while opening the file\n";
    }
    std::string tmp;
    getline(file, g_SerialName);
    getline(file, tmp);
    g_PortNumber = stoi(tmp);
    tmp.clear();
    getline(file, g_MsgToClose);
    file.close();
}

