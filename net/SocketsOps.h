//
// Created by 陈国威 on 2018/2/5.
//

#ifndef LIBCHEN_SOCKETSOPS_H
#define LIBCHEN_SOCKETSOPS_H

#include <arpa/inet.h>
namespace chen
{
namespace net
{

namespace sockets
{

int createNoblockingOrDie(sa_family_t family);


int connect(int sockfd, const struct sockaddr* addr);
void bindOrDie(int sockfd, const struct sockaddr* addr);
void listenOrDie(int sockfd);
int accept(int sockfd, struct sockaddr_in* addr);
ssize_t read(int sockfd, void* buf, size_t len);
ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);
ssize_t write(int sockfd, const void* buf, size_t len);
void close(int sockfd);
void shutdownWrite(int sockfd);

void toIpPort(char* buf, size_t size, const struct sockaddr* addr);
void toIp(char* buf, size_t size, const struct sockaddr* addr);

void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in* addr);

struct sockaddr_in getLocalAddr(int sockfd);

const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
struct sockaddr* sockaddr_cast(struct sockaddr_in* addr);

}

}
}

#endif //LIBCHEN_SOCKETSOPS_H
