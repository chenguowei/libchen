//
// Created by 陈国威 on 2018/2/5.
//

#ifndef LIBCHEN_INETADDRESS_H
#define LIBCHEN_INETADDRESS_H

#include <netinet/in.h>
#include <iostream>

using namespace std;

namespace chen
{
namespace net
{

namespace sockets
{
	const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
}

class InetAddress
{

 public:
  explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false, bool ipv6 = false);

  InetAddress(string ip, uint16_t port , bool ipv6 = false);

  explicit InetAddress(const struct sockaddr_in& addr)
  	:addr_(addr)
  {

  }
  ~InetAddress ();



  sa_family_t family() const
  {
	return addr_.sin_family;
  }

  string toIp() const ;
  string toIpPort() const;
  uint16_t toPort() const;

  const struct sockaddr* getSockAddr() const
  {
	return sockets::sockaddr_cast (&addr_);
  }

  uint32_t ipNetEndian() const;
  uint16_t portNetEndian() const
  {
	return addr_.sin_port;
  }

 private:
  struct sockaddr_in	addr_;
};

}
}


#endif //LIBCHEN_INETADDRESS_H
