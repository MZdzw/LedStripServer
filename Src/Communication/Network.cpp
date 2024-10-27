#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <chrono>
#include <fstream>

#include "Network.hpp"
#include "Common.hpp"

std::mutex g_Mutex;
std::condition_variable g_Cv;
std::string g_Msg;

bool g_Session;

void Network::Send(const std::string& data)
{
    std::cout << "Sending the data...\n";
    int n = write(m_ClientSocket, data.c_str(), data.length());
}

const std::string& Network::Read()
{
    GetBuffer().clear();
    memset(GetBuffer().data(), 0, GetBuffer().capacity());
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    if (recv(m_ClientSocket, GetBuffer().data(), GetBuffer().capacity(), 0) <= 0)
    {
        GetBuffer() = g_MsgToClose;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
    GetBuffer() = std::string(GetBuffer().data());
    g_Msg = GetBuffer();
    return GetBuffer();
}

void Network::ReceiverThread(Network& obj)
{
    std::string message;
    std::string messageOut;
    g_Session = true;
    while(1)
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        message = obj.Read();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << message << std::endl;
        // Messages sometimes come in chunks.
        // We need to check if whole msg arrived
        // Check according to time
        if (std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() < 1)
        {
            messageOut.append(message);
        }
        else 
        {
            messageOut.clear();
            messageOut.append(message);
        }

        std::cout << "Full message after concatenation is: " << messageOut << "\n";
        // close the socket when the specific msg arrive
        if (messageOut == g_MsgToClose)
        {
            g_Session = false;
            g_Cv.notify_one();
            break;
        }
        // notify Usb to send only if we have proper command from client
        if ((*(messageOut.begin()) == 'A') && (*(messageOut.end() - 1) == 'A'))
        {
            g_Msg = messageOut;
            g_Cv.notify_one();
        }
        // after reception of message
        // send response
        obj.Send(message);
    }
}

