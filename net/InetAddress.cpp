//
// Created by 陈国威 on 2018/2/5.
//

#include "InetAddress.h"
#include "Endian.h"
#include "SocketsOps.h"

#include <netdb.h>
#include <strings.h>
#include <netinet/in.h>

using namespace chen;
using namespace chen::net;



static const in_addr_t kInaddrAny = INADDR_ANY;
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;


InetAddress::InetAddress (uint16_t port, bool loopbackOnly, bool ipv6)
{
  bzero (&addr_, sizeof addr_);
  addr_.sin_family = AF_INET;
  addr_.sin_port = sockets::hostToNetwork16 (port);
  in_addr_t ip = loopbackOnly ? kInaddrLoopback: kInaddrAny;
  addr_.sin_addr.s_addr = ip;

}

InetAddress::InetAddress (string ip, uint16_t port, bool ipv6)
{
  bzero (&addr_, sizeof (addr_));
  sockets::fromIpPort (ip.c_str(), port, &addr_);
}

InetAddress::~InetAddress ()
{

}

uint32_t InetAddress::ipNetEndian () const
{
  return addr_.sin_addr.s_addr;
}

string InetAddress::toIpPort () const
{
  char buf[64] = "";
  sockets::toIpPort (buf, sizeof buf, getSockAddr ());

  return buf;
}

string InetAddress::toIp () const
{
  char buf[64] = "";
  sockets::toIp (buf, sizeof buf, getSockAddr ());

  return buf;
}






