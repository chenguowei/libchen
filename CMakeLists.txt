cmake_minimum_required(VERSION 3.9)
project(libchen)

set(CMAKE_CXX_STANDARD 11)

add_executable(libchen main.cpp net/Channel.cpp net/EPollPoller.cpp net/EPollPoller.h net/EventLoop.cpp net/EventLoop.h net/Acceptor.cpp net/Acceptor.h net/Socket.cpp net/Socket.h net/TcpConnection.cpp net/TcpConnection.h net/Callback.h net/Buffer.cpp net/Buffer.h net/Endian.h net/InetAddress.cpp net/InetAddress.h net/SocketsOps.cpp net/SocketsOps.h net/TcpServer.cpp net/TcpServer.h example/Echo.cpp net/Types.h)