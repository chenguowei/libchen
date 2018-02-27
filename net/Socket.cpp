//
// Created by 陈国威 on 2018/2/3.
//

#include "Socket.h"
#include "SocketsOps.h"
#include "InetAddress.h"

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <strings.h>
#include <stdio.h>

using namespace chen;
using namespace chen::net;


Socket::~Socket ()
{
  sockets::close (sockfd_);
}

void Socket::bindAddress (const InetAddress &localaddr)
{
  sockets::bindOrDie (sockfd_, localaddr.getSockAddr ());
}

void Socket::listen ()
{
  sockets::listenOrDie (sockfd_);
}


int Socket::accept (InetAddress *peeraddr)
{
  struct sockaddr_in addr;
  bzero (&addr, sizeof addr);
  int connfd = sockets::accept (sockfd_, &addr);
  if (connfd >= 0)
    {

    }

  return connfd;
}

void Socket::shutdownWrite ()
{
  sockets::shutdownWrite (sockfd_);
}

void Socket::setTcpNoDelay (bool on)
{
  int opt = on ? 1 : 0;
  ::setsockopt (sockfd_, IPPROTO_TCP, TCP_NODELAY,
              &opt, static_cast<socklen_t >(sizeof opt));
}

void Socket::setReuseAddr (bool on)
{
  int opt = on ? 1:0;
  ::setsockopt (sockfd_, SOL_SOCKET, SO_REUSEADDR,
              &opt, static_cast<socklen_t>(sizeof opt));
}

void Socket::setReusePort (bool on)
{
  int opt = on ? 1: 0;
  ::setsockopt (sockfd_, SOL_SOCKET, SO_REUSEPORT,
              &opt, static_cast<socklen_t >(sizeof opt));
}

void Socket::setKeepAlive (bool on)
{
  int opt = on ? 1 : 0;
  ::setsockopt (sockfd_, SOL_SOCKET, SO_KEEPALIVE,
              &opt, static_cast<socklen_t>(sizeof (opt)));
}
