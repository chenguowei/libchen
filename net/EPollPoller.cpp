//
// Created by 陈国威 on 2018/2/3.
//

#include "EPollPoller.h"
#include <iostream>
#include "Channel.h"
#include "EventLoop.h"

#include <errno.h>
#include <assert.h>
#include <poll.h>
#include <sys/epoll.h>

using namespace std;

using namespace chen;
using namespace chen::net;


namespace
{
const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;
}

EPollPoller::EPollPoller (EventLoop *loop)
	:loop_(loop),
	 epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
	 events_(kInitEventListSize)
{

}

EPollPoller::~EPollPoller ()
{
  ::close (epollfd_);
}

void EPollPoller::poll (int timeOutMs, ChannelList *activeChannels)
{
  int numEvents = ::epoll_wait(epollfd_, &*events_.begin(),
							   static_cast<int>(events_.size ()), timeOutMs);
  int saveErrno = errno;
  if (numEvents > 0)
	{
	  cout << "EPollPoller::poll number = " << numEvents << endl;
	  fillActiveChannels (numEvents, activeChannels);
	  if (static_cast<size_t >(numEvents) == events_.size ())
		{
		  events_.resize (numEvents + events_.size ());
		}
	}
  else if (numEvents == 0)
	{
	  //cout << "nothing happend\n";
	}
  else
	{
	  if (saveErrno != EINTR)
		{
			errno = saveErrno;
		    cout << "EPollPoller::poll errno";
		}
	}
}

void  EPollPoller::fillActiveChannels (int numEvents, ChannelList *activeChannel)
{
  for (int i = 0; i < numEvents; ++i)
	{
	  Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
	  int fd = channel->fd ();
	  ChannelMap::const_iterator it = channels_.find (fd);
	  assert (it != channels_.end ());
	  assert (it->second == channel);

	  channel->set_revents (events_[i].events);
	  activeChannel->push_back (channel);
	}
}

void EPollPoller::updateChannel (Channel *channel)
{
  const int index =  channel->index ();

  if (index == kNew || index == kDeleted)
	{
	  int fd = channel->fd ();
	  if (index == kNew)
		{
		  channels_[fd] = channel;
		}
	  else
		{

		}

	  channel->set_index (kAdded);
	  update (EPOLL_CTL_ADD, channel);
	}
  else
	{
	  // update exiting  one with EPOLL_CTL_MOD/DEL
	  int fd = channel->fd ();
	  (void)fd;
	  if (channel->isNodeEvent ())
		{
		  update (EPOLL_CTL_DEL, channel);
		  channel->set_index (kDeleted);
		}
	  else
		{
		  update (EPOLL_CTL_MOD, channel);
		}
	}
}

void EPollPoller::removeChannel (Channel *channel)
{
  int fd = channel->fd ();
  assert (channels_.find (fd) != channels_.end ());
  assert (channels_[fd] == channel);
  assert (channel->isNodeEvent ());
  int index = channel->index ();
  assert (index == kAdded || index == kDeleted);
  size_t n = channels_.erase (fd);
  (void)n;
  assert (n == 1);

  if (index == kAdded)
	{
	  update (EPOLL_CTL_DEL, channel);
	}
  channel->set_index (kNew);
}

void EPollPoller::update (int oper, Channel *channel)
{
  struct epoll_event event;
  bzero (&event, sizeof event);
  event.events = channel->events ();
  event.data.ptr = channel;
  int fd = channel->fd ();

  if (::epoll_ctl(epollfd_, oper, fd, &event) < 0)
	{
	  if (oper == EPOLL_CTL_DEL)
		{
		  cout << "eopll_ctl op = " << oper << endl;
		}
	  else
		{
		  cout << "epoll_ct op =" << oper << "fd = " << fd << endl;
		}

	}
}