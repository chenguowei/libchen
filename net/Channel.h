#ifndef CHEN_NET_CHANNEL_H
#define CHEN_NET_CHANNEL_H

#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

using namespace std;

namespace chen
{

namespace net
{
	class EventLoop;

	class Channel : boost::noncopyable
	{
	 public:
	  typedef boost::function<void()> EventCallback;

	  Channel(EventLoop* loop, int fd);
	  ~Channel ();

	  void handleEvent();
	  void setReadCallback(const EventCallback& cb)
	  { readCallback_ = cb;}

	  void setWriteCallback(const EventCallback& cb)
	  { writeCallback_ = cb; }

	  void setCloseCallback(const EventCallback& cb)
	  { closeCallback_ = cb;}

	  void setErrorCallback(const EventCallback& cb)
	  { errorCallback_ = cb; }


	  bool isNodeEvent() const
	  {
		return true;
	  }

	  int events() const
	  { return events_;}

	  int fd() const
	  { return fd_; }

		void set_revents(int revents)
	  { revents_ = revents;}

	  int revents() const
	  { return revents_; }

	  int index() const
	  { return index_; }
	  void set_index(int index)
	  { index_ = index; }


	  void enableReading() { events_ |= kReadEvent; update();}
	  void disableReading() { events_ &= kReadEvent; update ();}
	  void enableWriting() { events_ |= kWriteEvent; update ();}
	  void disableWriting() { events_ &= kWriteEvent; update ();}
	  void disableAll() { events_ = kNodeEvent; update ();}
	  bool isWriting() const { return events_ & kWriteEvent; }
	  bool isReading() const { return events_ & kReadEvent; }

	  void remove();
	 private:
	  void update();

	  EventLoop*	loop_;
	  int 			index_;
	  const int 	fd_;
	  int  events_;
	  int  revents_;
	  static const int kNodeEvent;
	  static const int kReadEvent;
	  static const int kWriteEvent;

	  EventCallback readCallback_;
	  EventCallback writeCallback_;
	  EventCallback closeCallback_;
	  EventCallback errorCallback_;

	};

}
}

#endif	//CHEN_NET_CHANNEL_H