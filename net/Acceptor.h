//
// Created by 陈国威 on 2018/2/3.
//

#ifndef LIBCHEN_ACCEPTOR_H
#define LIBCHEN_ACCEPTOR_H

#include "Channel.h"
#include "Socket.h"

namespace chen
{
namespace net
{
	class InetAddress;
	class EventLoop;


	class Acceptor
	{
	 public:
	  typedef boost::function<void (int sockfd, const InetAddress&)>  NewConnectionCallback;

	  Acceptor(EventLoop* loop, const InetAddress& listenaddr, bool reuseport);
	  ~Acceptor ();


	  void setNewConnectionCallback(const NewConnectionCallback& cb)
	  {
		newConnectionCallback_ = cb;
	  }

	  bool listenning() const { return listenning_;}
	  void listen();

	 private:
	  void handleRead();

	  EventLoop*	loop_;
	  Socket	acceptSocket_;
	  Channel	acceptChannel_;
	  NewConnectionCallback	newConnectionCallback_;
	  bool 		listenning_;


	};

}
}


#endif //LIBCHEN_ACCEPTOR_H
