#pragma once
#include "Communication.hpp"

class Network : public ICommunication
{
    public:
    Network(int clientSocket)
        : ICommunication(128)
        , m_ClientSocket(clientSocket)
    {
    }

    ~Network()
    {
        std::cout << "Closing the network connection\n";
        close(m_ClientSocket);
    }

    void Send(const std::string& data) override; 
    const std::string& Read() override;

    static void ReceiverThread(Network& obj);

    private:
    int m_ClientSocket;

};

