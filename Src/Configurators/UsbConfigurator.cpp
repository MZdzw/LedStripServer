#include <chrono>
#include "UsbConfigurator.hpp"

UsbConfigurator::UsbConfigurator(TerminalInterfaceT&& usbData)
: IProtocol(256), m_UsbData(usbData)
{
}

void UsbConfigurator::Send()
{
    // Send data
    int bytes_written = write(m_UsbData.serialPort, GetSendBuffer().c_str(), GetSendBuffer().length());
    
    // Check if send
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
    GetReceptionBuffer().clear();
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    GetReceivedBytes() = read(m_UsbData.serialPort,
                              GetReceptionBuffer().data(),
                              GetReceptionBuffer().capacity());

    GetReceptionBuffer() = std::string(GetReceptionBuffer().data(), GetReceptionBuffer().capacity());
    if (GetReceivedBytes() > 0)
    {
        *(GetReceptionBuffer().end() - 1) = '\0';
        std::cout << "Received: " << GetReceptionBuffer() << std::endl;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    m_ElapsedTimeDiffReadSeconds = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
}

int UsbConfigurator::GetElapsedTimeBetweenRead() const
{
    return m_ElapsedTimeDiffReadSeconds;
}

bool UsbConfigurator::IsConnected() const
{
    return m_UsbData.isConnected;
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
