#pragma once
#ifndef TESTING

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

struct SocketInterfaceT
{
    int portNumber;
    int& socketFd;
    int& clientSocketFd;
    bool clientConnected;
    sockaddr_in& serverAddress;
    sockaddr_in& clientAddress;
};
#else

struct SocketInterfaceT
{
    int portNumber;
};


#endif