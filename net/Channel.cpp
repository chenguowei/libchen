//
// Created by 陈国威 on 2018/2/3.
//

#include "Channel.h"
#include <sys/poll.h>
#include "EventLoop.h"

using namespace chen;
using namespace chen::net;

const int Channel::kNodeEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel (EventLoop *loop, int fd)
	:loop_(loop),
	 fd_(fd),
	 events_(0),
	 revents_(0),
	 index_(-1)

{

}

Channel::~Channel ()
{

}

void Channel::handleEvent ()
{
  if ((revents_ & POLLHUP) && !(revents_ & POLLIN))
	{
	  cout << "Channel::handleEvent() fd = " << fd_ << endl;
	  if (closeCallback_)
		closeCallback_();
	}

  if (revents_ & POLLNVAL)
	{
	  cout << "fd = " << fd_ << " Channel::handleEvent() POLLNVAL\n";
	}

  if (revents_ & (POLLERR | POLLNVAL))
	{
	  if (errorCallback_)
		errorCallback_();
	}

  if (revents_ & (POLLIN | POLLPRI))
	{
	  if (readCallback_)
		readCallback_();
	}

  if (revents_ & POLLOUT)
	{
	  if (writeCallback_)
		writeCallback_();
	}
}

void Channel::update ()
{
  loop_->updateChannel (this);
}

void Channel::remove ()
{
  loop_->removeChannel (this);
}
