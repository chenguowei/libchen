//
// Created by 陈国威 on 2018/2/5.
//

#ifndef LIBCHEN_TCPSERVER_H
#define LIBCHEN_TCPSERVER_H

#include "TcpConnection.h"
#include "Callback.h"

#include <map>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace chen
{
namespace net
{

class EventLoop;
class Acceptor;

class TcpServer
{

 public:
  enum Option
  {
	kNoReusePort,
	kReusePort,
  };

  TcpServer(EventLoop* loop, const InetAddress& listenAddr,
  			const string& name,
  			Option option = kNoReusePort);

  ~TcpServer ();

  const string& ipPort() const
  { return ipPort_; }
  const string& name() const
  { return name_; }

  EventLoop* getLoop() const
  { return loop_; }

  void setConnectionCallback(const ConnectionCallback& cb)
  { connectionCallback_ = cb; }

  void setMessageCallback(const MessageCallback& cb)
  { messageCallback_ = cb; }


  void start();

 private:

  void newConnection(int sockfd, const InetAddress& peerAddr);

  void removeConnection(const TcpConnectionPtr& conn);

  void removeConnectionInLoop(const TcpConnectionPtr& conn);

  typedef std::map<string, TcpConnectionPtr> ConnectionMap;

  EventLoop*	loop_;
  Acceptor*		acceptor_;
  const string name_;
  const string ipPort_;

  ConnectionCallback	connectionCallback_;
  MessageCallback		messageCallback_;

  int nextConnId_;
  ConnectionMap			connections_;

};

}
}



#endif //LIBCHEN_TCPSERVER_H
