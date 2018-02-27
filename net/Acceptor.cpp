//
// Created by 陈国威 on 2018/2/3.
//

#include "Acceptor.h"

#include "Channel.h"
#include "SocketsOps.h"
#include "InetAddress.h"

#include <errno.h>
#include <fcntl.h>

using namespace chen;
using namespace chen::net;

Acceptor::Acceptor (EventLoop *loop, const InetAddress &listenaddr, bool reuseport)
	:loop_(loop),
	 acceptSocket_(sockets::createNoblockingOrDie (listenaddr.family ())),
	 acceptChannel_(loop, acceptSocket_.fd()),
	 listenning_(false)
{
  acceptSocket_.setKeepAlive (true);
  acceptSocket_.setReuseAddr (true);
  acceptSocket_.setReusePort (reuseport);
  acceptSocket_.bindAddress (listenaddr);
  acceptChannel_.setReadCallback (
	  boost::bind (&Acceptor::handleRead, this)
  );

}

Acceptor::~Acceptor ()
{
  acceptChannel_.disableAll ();
  acceptChannel_.remove ();
}

void Acceptor::listen ()
{
  listenning_ = true;
  acceptSocket_.listen ();

  // 调用Channel::update() 把 sockfd 添加到 epoll_ctl中
  acceptChannel_.enableReading ();
}

void Acceptor::handleRead ()
{
  InetAddress peeAddr;

  int connfd = acceptSocket_.accept (&peeAddr);
  if (connfd >= 0)
	{
	  if (newConnectionCallback_)
		{
		  newConnectionCallback_(connfd, peeAddr);
		}
	  else
		{
		  sockets::close (connfd);
		}
	}
  else
	{
	  std::cout << "in Acceptor::handleRead connfd=" << connfd << endl;
	  if (errno == EMFILE)
		{

		}
	}
}