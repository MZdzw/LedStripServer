cmake_minimum_required(VERSION 3.22)

add_subdirectory(Test_TcpIp)
add_subdirectory(Test_Usb)

add_test(NAME Test_TcpIp COMMAND Test_TcpIp)
add_test(NAME Test_Usb COMMAND Test_Usb)