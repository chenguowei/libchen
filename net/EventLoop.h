//
// Created by 陈国威 on 2018/2/3.
//

#ifndef LIBCHEN_EVENTLOOP_H
#define LIBCHEN_EVENTLOOP_H

#include <vector>
#include <boost/function.hpp>

namespace chen
{

namespace net
{

	class Channel;
	class EPollPoller;

	class EventLoop
	{
	 public:

	  typedef boost::function<void()>	Functor;

	  EventLoop();
	  ~EventLoop ();

	  void loop();

	  void runInLoop(const Functor& cb);

	  void updateChannel(Channel* channel);
	  void removeChannel(Channel* channel);
	 private:
	  void handleRead();

	  typedef std::vector<Channel*> ChannelList;

	  EPollPoller*	poller_;
//	  Channel*		channel_;


	  ChannelList	activeChannels_;
	  Channel*		currentActiveChannel_;
	};

}
}



#endif //LIBCHEN_EVENTLOOP_H
