#include <iostream>
#include <thread>
#include <functional>
#include <array>
#include <vector>
#include <cstring>
#include "server.hpp"
#include "TcpIpConfigurator.hpp"
#include "Common.hpp"

ServerAndTcpIpNotificationS serverAndTcpIpNotification[MAX_NUM_OF_CLIENTS];
std::array<in_addr_t, MAX_NUM_OF_CLIENTS> addresesOfConnectedClients;

void Server::ResetClientData()
{
    SocketInterfaceT& conf = static_cast<TcpIpConfigurator&>(this->m_TcpIpConnection.GetTcpIpConfiguration()).GetSocketInterface();
    std::memset(&conf.clientAddress, 0, sizeof(conf.clientAddress));
    conf.clientConnected = false;
    conf.clientSocketFd = 0;
}

void Server::ConnectionManager()
{
    std::unique_lock<std::mutex> lock(serverAndTcpIpNotification[m_Id].serverAndTcpIpMutex);

    std::vector<std::tuple<std::thread, std::thread,
                           std::thread, std::thread>> threads;

    SocketInterfaceT& conf = static_cast<TcpIpConfigurator&>(this->m_TcpIpConnection.GetTcpIpConfiguration()).GetSocketInterface();
    for(;;)
    {
        socklen_t clientLen = sizeof(conf.clientAddress);
        std::cout << "Waiting for connection...(" << this->m_Id << ")\n";
        std::cout << "Address of client before accept: " << conf.clientAddress.sin_addr.s_addr << std::endl;
        conf.clientSocketFd = accept(conf.socketFd, (struct sockaddr*)&conf.clientAddress, &clientLen);
        std::cout << "Address of client after accept: " << conf.clientAddress.sin_addr.s_addr << std::endl;
        auto it = std::find_if(addresesOfConnectedClients.begin(), addresesOfConnectedClients.end(),
                               [&conf](in_addr_t address){
                                  return address == conf.clientAddress.sin_addr.s_addr;
                               });
        if (it != addresesOfConnectedClients.end())
        {
            // There is already the client connected
            std::cout << "There is already that client\n";
            std::cout << "Not accepting\n";
            close(conf.clientSocketFd);
            ResetClientData();
        }
        else
        {
            addresesOfConnectedClients[m_Id] = conf.clientAddress.sin_addr.s_addr;
            conf.clientConnected = true;
            std::thread rcvThr(&TcpIp::TcpIpReaderThread, std::ref(this->m_TcpIpConnection), m_Id);
            // threads.push_back(std::make_tuple<std::thread, std::thread>(
            //           std::thread(&TcpIp::TcpIpReaderThread, std::ref(this->m_TcpIpConnection)),
            //           std::thread(&TcpIp::TcpIpSenderThread, std::ref(this->m_TcpIpConnection)),
            //           std::thread(&Usb::UsbReaderThread, std::ref(this->m_UsbConnection)),
            //           std::thread(&Usb::UsbSenderThread, std::ref(this->m_UsbConnection)))
            //          );
            std::cout << "Connection established id=" << m_Id << "\n";
            serverAndTcpIpNotification[m_Id].serverAndTcpIpCv.wait(lock);
            std::cout << "Disconnecting\n";
            close(conf.clientSocketFd);
            ResetClientData();
            addresesOfConnectedClients[m_Id] = 0;
            rcvThr.join();
        }
        
    }
    // for (auto& [tcpIpReceiverThread, tcpIpSenderThread,
    //             usbReceiverThread, usbSenderThread] : threads)
    // {
    //     if (tcpIpReceiverThread.joinable())
    //         tcpIpReceiverThread.join();
    //     if (tcpIpSenderThread.joinable())
    //         tcpIpSenderThread.join();
    //     if (usbReceiverThread.joinable())
    //         usbReceiverThread.join();
    //     if (usbSenderThread.joinable())
    //         usbSenderThread.join();
    // }
}

void Server::ServerThread()
{
    std::vector<std::tuple<std::thread, std::thread,
                           std::thread, std::thread>> threads;

    int serverSocket;
    std::array<int, MAX_NUM_OF_CLIENTS> clientsSocket = {};
    sockaddr_in serverAddress = {AF_INET, htons(54321), INADDR_ANY, {0}};
    std::array<sockaddr_in, MAX_NUM_OF_CLIENTS> clientsAddress = {};
    std::array<bool, MAX_NUM_OF_CLIENTS> isConnected = {false};

    UsbConfigurator usbConf(TerminalInterfaceT{}); 
    Usb usbObj(usbConf);

    std::array<TcpIpConfigurator, MAX_NUM_OF_CLIENTS> tcpIpConfObj =
    {
        SocketInterfaceT{54321, serverSocket, clientsSocket[0], isConnected[0], serverAddress, clientsAddress[0]},
        SocketInterfaceT{54321, serverSocket, clientsSocket[1], isConnected[1], serverAddress, clientsAddress[1]}
    };
    std::array<TcpIp, MAX_NUM_OF_CLIENTS> tcpIpObj = 
    {
        tcpIpConfObj[0],
        tcpIpConfObj[1]
    };

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
    listen(serverSocket, 5);

    std::array<Server, MAX_NUM_OF_CLIENTS> connections = {Server(tcpIpObj[0], usbObj, 0),
                                                          Server(tcpIpObj[1], usbObj, 1)};
    std::thread listener1(&Server::ConnectionManager, std::ref(connections[0]));
    std::thread listener2(&Server::ConnectionManager, std::ref(connections[1]));

    listener1.join();
    listener2.join();

    close(serverSocket);
}
