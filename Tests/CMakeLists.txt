cmake_minimum_required(VERSION 3.22)

add_subdirectory(Test_TcpIp)

add_test(NAME Test_TcpIp COMMAND Test_TcpIp)