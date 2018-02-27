//
// Created by 陈国威 on 2018/2/4.
//

#ifndef LIBCHEN_TCPCONNECTION_H
#define LIBCHEN_TCPCONNECTION_H
#include <iostream>

#include "Callback.h"
#include "InetAddress.h"
#include "Buffer.h"

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>

using namespace std;

namespace chen
{
namespace net
{

class EventLoop;
class Channel;
class Socket;
class InetAddress;

class TcpConnection : boost::noncopyable,
 					  public boost::enable_shared_from_this<TcpConnection>
{
 public:
  TcpConnection(EventLoop* loop,
  				const string& name,
				int sockfd,
  				const InetAddress& localaddr,
  				const InetAddress& peeraddr);
  ~TcpConnection ();

  EventLoop* getLoop() const
  { return loop_; }

  const InetAddress& localAddress() const
  { return localaddr_; }
  const InetAddress& peerAddress() const
  { return peeraddr_; }
  bool connected() const { return state_ == kConnected; }
  bool disconnected() const { return state_ == kDisconnected; }

  void send(const void* message, size_t len);
  void send(const string& message);
  void send(Buffer* message);

  void setConnecctionCallback(const ConnectionCallback& cb)
  { connectionCallback_= cb; }

  void setCloseCallback(const CloseCallback& cb)
  { closeCallback_ = cb; }

  void setMessageCallback(const MessageCallback& cb)
  { messageCallback_ = cb; }

  string name() const
  { return name_; }

  void startRead();
  void stopRead();
  bool isrReading() const { return reading_; }


  void connectEstablished();
  void connectDestroyed();

 private:
  enum StateE
  {
	kDisconnected,
	kConnecting,
	kConnected,
	kDisconnecting
  };
  void handleRead();
  void handleWrite();
  void handleClose();
  void handleError();

  void setState(StateE s) { state_ = s; }

  void sendInLoop(const string& message);
  void sendInLoop(const void* message, size_t len);

  void startReadInLoop();
  void stopReadInLoop();

  EventLoop*	loop_;
  const string 	name_;
  bool 			reading_;
  StateE		state_;
  boost::scoped_ptr<Channel> connetChannel_;
  boost::scoped_ptr<Socket>	socket_;
  const InetAddress localaddr_;
  const InetAddress peeraddr_;

  ConnectionCallback	connectionCallback_;
  CloseCallback			closeCallback_;
  MessageCallback		messageCallback_;

  Buffer			inputBuffer_;
  Buffer			outputBuffer_;
};

}
}




#endif //LIBCHEN_TCPCONNECTION_H
