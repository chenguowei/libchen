//
// Created by 陈国威 on 2018/2/5.
//

#include "TcpServer.h"

#include "SocketsOps.h"
#include "Acceptor.h"
#include "Callback.h"
#include "InetAddress.h"
#include "EventLoop.h"

#include <boost/bind.hpp>

#include <stdio.h>

using namespace chen;
using namespace chen::net;

TcpServer::TcpServer (EventLoop *loop, const InetAddress &listenAddr, const string &name, Option option)
	:loop_(loop),
	 ipPort_(listenAddr.toIpPort ()),
	 name_(name),
	 acceptor_(new Acceptor(loop, listenAddr, option== kReusePort)),
	 connectionCallback_(defaultConnectionCallback),
	 messageCallback_(defaultMessageCallback),
	 nextConnId_(1)
{
  acceptor_->setNewConnectionCallback (
	  boost::bind (&TcpServer::newConnection, this, _1, _2)
  );

}

TcpServer::~TcpServer ()
{
  for (ConnectionMap::iterator it = connections_.begin ();
	  it != connections_.end (); ++it)
	{
	  TcpConnectionPtr conn = it->second;
	  it->second.reset ();

	  conn.reset ();
	}
}

void TcpServer::newConnection (int sockfd, const InetAddress &peerAddr)
{
  char buf[64];
  snprintf (buf, sizeof buf, "-%s#%d", ipPort_.c_str(), nextConnId_);
  ++nextConnId_;
  string connName = name_ + buf;

  InetAddress localAddr(sockets::getLocalAddr (sockfd));

  TcpConnectionPtr	conn(new TcpConnection(loop_,
  											connName,
  											sockfd,
  											localAddr,
  											peerAddr));
  
  connections_[connName] = conn;
  conn->setConnecctionCallback (connectionCallback_),
  conn->setMessageCallback (messageCallback_),
  conn->setCloseCallback (
	  boost::bind (&TcpServer::removeConnection, this, _1)
  );
  loop_->runInLoop (
	  boost::bind (&TcpConnection::connectEstablished, conn)
  );
  
}

void TcpServer::start ()
{
  assert (!acceptor_->listenning ());
  loop_->runInLoop (
	  boost::bind (&Acceptor::listen, acceptor_)
  );
}

void TcpServer::removeConnection (const TcpConnectionPtr &conn)
{
  loop_->runInLoop (
	  boost::bind (&TcpServer::removeConnectionInLoop, this, conn)
  );
}

void TcpServer::removeConnectionInLoop (const TcpConnectionPtr &conn)
{
  size_t n = connections_.erase (conn->name());
  (void)n;
  //assert (n == 1);
  loop_->runInLoop (
	  boost::bind (&TcpConnection::connectDestroyed, conn)
  );
}

