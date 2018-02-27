//
// Created by 陈国威 on 2018/2/5.
//
#include "../net/EventLoop.h"
#include "../net/TcpServer.h"

#include <set>
#include <iostream>
#include <stdio.h>

#include <boost/bind.hpp>

using namespace chen;
using namespace chen::net;

class EchoServer
{
 public:
  EchoServer(EventLoop* loop, const InetAddress& listenAddr)
      :server_(loop, listenAddr, "EchoServer")
  {
    server_.setConnectionCallback (
        boost::bind(&EchoServer::onConnection, this, _1)
    );
    server_.setMessageCallback (
        boost::bind (&EchoServer::onMessage, this, _1, _2)
    );

  }
  ~EchoServer ()
  {

  }

  void start()
  {
    server_.start();
  }

 private:

  void onConnection(const TcpConnectionPtr& conn)
  {
    std::cout << conn->localAddress ().toIpPort () << " ->"
                                                   << conn->peerAddress ().toIpPort () << " is"  << endl;
    if (conn->connected ())
      {
        connections_.insert (conn);
      }
    else
      {
        connections_.erase (conn);
      }

  }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buffer)
  {
    while(buffer->readableBytes ())
      {
        const char* data = buffer->peek ();
        int32_t len = strlen (data);
        std::cout << "EchoServer::onMessage() data= ";
        std::cout << data << endl;
        buffer->retrieveAll ();
        conn->send (data, len);
      }
  }

  typedef std::set<TcpConnectionPtr> ConnectionList;
  TcpServer		server_;
  ConnectionList	connections_;
};

int main(int argc, char *argv[])
{
  std::cout << "pid = " << getpid () << endl;

  if (argc > 1)
    {
      EventLoop loop;
      uint16_t port = static_cast<uint16_t>(atoi (argv[1]));
      InetAddress serverAddr(port);
      EchoServer server(&loop, serverAddr);
      server.start ();
      loop.loop ();
    }
  else
    {
      printf ("usage: %s port\n", argv[0]);
    }

  exit (0);

}
