//
// Created by 陈国威 on 2018/2/5.
//

#include "SocketsOps.h"
#include "Endian.h"
#include "Types.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/uio.h>
#include <assert.h>

#include <boost/implicit_cast.hpp>

using namespace chen;
using namespace chen::net;

namespace
{

typedef struct soackaddr SA;

void setNonBlockAndCloseExec(int sockfd)
{
  int flags = ::fcntl (sockfd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  int ret = ::fcntl (sockfd, F_SETFL, flags);

  flags = ::fcntl (sockfd, F_GETFD, 0);
  flags |= FD_CLOEXEC;
  ret = ::fcntl (sockfd, F_SETFD, flags);

  (void)ret;
}
}

const struct sockaddr* sockets::sockaddr_cast (const struct sockaddr_in *addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast <const void*>(addr));
}

struct sockaddr* sockets::sockaddr_cast ( struct sockaddr_in *addr)
{
  return static_cast<struct sockaddr*>(implicit_cast <void*>(addr));
}

int sockets::createNoblockingOrDie (sa_family_t family)
{
  int sockfd = ::socket (family, SOCK_STREAM, IPPROTO_TCP);

  setNonBlockAndCloseExec (sockfd);

  return sockfd;
}

void sockets::bindOrDie (int sockfd, const struct sockaddr *addr)
{
  int ret = ::bind (sockfd, addr, static_cast<socklen_t >(sizeof (struct sockaddr_in)));
  if (ret < 0)
    {

    }
}

void sockets::listenOrDie (int sockfd)
{
  int ret = ::listen (sockfd, SOMAXCONN);
  if (ret < 0)
    {

    }
}

int sockets::accept (int sockfd, struct sockaddr_in *addr)
{
  socklen_t len = static_cast<socklen_t >(sizeof addr);
  int connfd = ::accept (sockfd, sockaddr_cast (addr), &len);
  setNonBlockAndCloseExec (connfd);

  if (connfd < 0)
    {
      int saveErrno = errno;
      switch (saveErrno)
        {
          case EAGAIN:
          case ECONNABORTED:
          case EINTR:
          case EPROTO:
          case EPERM:
          case EMFILE:

            errno = saveErrno;
          break;

          case EBADF:
          case EFAULT:
          case EINVAL:
          case ENFILE:
          case ENOBUFS:
          case ENOTSOCK:
          case EOPNOTSUPP:

            break;
          default:break;
        }
    }

  return connfd;
}

int sockets::connect (int sockfd, const struct sockaddr *addr)
{
  return ::connect (sockfd, addr, static_cast<socklen_t>(sizeof addr));
}

ssize_t sockets::read (int sockfd, void *buf, size_t len)
{
  return ::read (sockfd, buf, len);
}

ssize_t sockets::readv (int sockfd, const struct iovec *iov, int iovcnt)
{
  return ::readv(sockfd, iov, iovcnt);
}

ssize_t sockets::write (int sockfd, const void *buf, size_t len)
{
  return ::write (sockfd, buf, len);
}

void sockets::close (int sockfd)
{
  ::close (sockfd);
}

void sockets::shutdownWrite (int sockfd)
{
  ::shutdown (sockfd, SHUT_WR);
}

void sockets::fromIpPort (const char *ip, uint16_t port, struct sockaddr_in *addr)
{
  addr->sin_family = AF_INET;
  addr->sin_port = hostToNetwork16 (port);

  if (::inet_pton (AF_INET, ip, &addr->sin_addr) < 0)
    {

    }
}


struct sockaddr_in sockets::getLocalAddr(int sockfd)
{
  struct sockaddr_in localaddr;
  bzero (&localaddr, sizeof localaddr);
  socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
  if (::getsockname (sockfd, sockaddr_cast (&localaddr), &addrlen) < 0)
    {

    }

  return localaddr;
}

void sockets::toIpPort (char *buf, size_t size, const struct sockaddr *addr)
{
  toIp (buf, size, addr);
  size_t  end = ::strlen(buf);
  const struct sockaddr_in* addr4 = (struct sockaddr_in*)addr;
  uint16_t port = sockets::networkToHost16 (addr4->sin_port);
  assert (size > end);
  snprintf (buf+end, size-end, ":%u", port);
}

void sockets::toIp (char *buf, size_t size, const struct sockaddr *addr)
{
  assert (size >= INET_ADDRSTRLEN);
  const struct sockaddr_in* addr4 = (struct sockaddr_in*)addr;
  ::inet_ntop (AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t >(size));
}

