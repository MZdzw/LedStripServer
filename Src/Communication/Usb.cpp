#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "Usb.hpp"
#include "Common.hpp"

enum class UsbMessageE
{
    REMOVE_SECTOR, ADD_SECTOR, SET_DIODE_HSV,
    SET_DIODE_RGB, SET_SECTOR_HSV, SET_SECTOR_RGB,
    SET_SECTOR_RAINBOW, SPAWN_DIODE, SET_ANIMATION_SPEED,
    SET_ROLLING_EFFECT, SET_DIMMING_ENTIRE_EFFECT,
    SET_NO_ANIMATION, LED_STRIP_REQ, MAX
};


void Usb::Send()
{
}

void Usb::Read()
{
    m_ReceivedMsg.clear();
    m_UsbConf.Read();
    const int timeDiff = m_UsbConf.GetElapsedTimeBetweenRead();
    // timeDiff not usefull here?
    // The data in Reception buffer in UsbConfigurator should be binary
    // we need to convert it from binary to ASCII
    if (m_UsbConf.GetReceivedBytes() > 0)
    {
        const char* const rawBytes = m_UsbConf.GetReceptionBuffer().data();
        std::for_each(rawBytes, rawBytes + m_UsbConf.GetReceivedBytes(), [this](const char& byte){
            #include <iostream>
            std::cout << "Append: "<< std::to_string(byte) << std::endl;
            this->m_ReceivedMsg += std::to_string(byte);
        });
    }
 

}

bool Usb::TranslateMsgToUsb(const std::string &tcpIpMsg)
{
    return false;
}

void Usb::UsbSenderThread()
{
//    std::vector<std::string> usbVars;
//    std::vector<char> usbVarsNum;
//    bool areValuesValid = false;
//    std::unique_lock<std::mutex> lock(g_Mutex);
//    while(1)
//    {   
//        usbVars.clear();
//        usbVarsNum.clear();
//        g_Cv.wait(lock);
//        areValuesValid = obj.TranslateMsgFromNetworkToUsb(usbVars);
//        if (areValuesValid)
//        {
//            for (int i = 0; i < 6; i++)
//                usbVarsNum.push_back(0xAA);
//            for (auto& var : usbVars)
//            {
//                usbVarsNum.push_back(stoi(var));
//            }
//            for (int i = 0; i < 6; i++)
//                usbVarsNum.push_back(0xAA);
//
//            std::cout << "Sending USB data...\n";
//            obj.Send(std::string(usbVarsNum.begin(), usbVarsNum.end()));
//        }
//        if (!g_Session)
//        {
//            obj.ChangeToNonBlocking();
//            break;
//        }
//    }
}

void Usb::UsbReaderThread()
{
 //   //std::unique_lock<std::mutex> lock(g_Mutex);
 //   while(1)
 //   {
 //       Read();

 //       // if we close the port
 //       if (!g_Session)
 //       {
 //           break;
 //       }
 //   }
}
