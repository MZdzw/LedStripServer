#ifndef __SERVER_HPP__
#define __SERVER_HPP__
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <condition_variable>

class ServerTCP
{
private:
    int m_PortNumber;
    int m_Socketfd;
    int m_ClientSocketfd;
    bool m_ClientConnected;
    sockaddr_in m_ServerAddress;
    bool m_IsConnected;


public:
    ServerTCP(int portNumber)
    : m_PortNumber(portNumber), m_Socketfd(-1),
      m_ClientSocketfd(-1), m_ClientConnected(false),
      m_ServerAddress(sockaddr_in{AF_INET, htons(m_PortNumber), INADDR_ANY}),
      m_IsConnected(false)
    {
        m_Socketfd = socket(AF_INET, SOCK_STREAM, 0);
        bind(m_Socketfd, reinterpret_cast<sockaddr*>(&m_ServerAddress), sizeof(m_ServerAddress));
    }

    ~ServerTCP()
    {
        close(m_Socketfd);
    }

    void ListenConnections();
    int GetSocket() const
    {
        return m_ClientSocketfd;
    }

    static void TcpHandlerThread();
    static void ReadConfContent();
};

#endif
