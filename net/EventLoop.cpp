//
// Created by 陈国威 on 2018/2/3.
//

#include "EventLoop.h"
#include "EPollPoller.h"
#include "Channel.h"

using namespace chen;
using namespace chen::net;

namespace
{

const int kPollTimeMs = 10000;
}

EventLoop::EventLoop ()
	:poller_(new EPollPoller(this)),
	 currentActiveChannel_(NULL)
{

}

 EventLoop::~EventLoop ()
 {
   activeChannels_.clear ();
 }

void EventLoop::loop ()
{
  //int timeOut = 10;

  while (true)
	{
	  //clear last channels
	  activeChannels_.clear ();
	  poller_->poll (kPollTimeMs, &activeChannels_);

	  for (ChannelList::iterator it = activeChannels_.begin ();
		  it != activeChannels_.end (); ++it)
		{
		  currentActiveChannel_ = *it;
		  currentActiveChannel_->handleEvent ();
		}

	  currentActiveChannel_ = NULL;

	}
}

void EventLoop::runInLoop (const Functor &cb)
{
  cb();
}

void EventLoop::updateChannel (Channel *channel)
{
  poller_->updateChannel (channel);
}

void EventLoop::removeChannel (Channel *channel)
{
  poller_->removeChannel (channel);
}