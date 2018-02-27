//
// Created by 陈国威 on 2018/2/3.
//

#ifndef LIBCHEN_EPOLLPOLLER_H
#define LIBCHEN_EPOLLPOLLER_H

#include <vector>
#include <map>

struct epoll_event;

namespace chen
{

namespace net
{

class EventLoop;
class Channel;

class EPollPoller
{
 public:
  typedef std::vector<Channel*> ChannelList;

  EPollPoller(EventLoop* loop);
  ~EPollPoller ();

  void poll(int timeOutMs, ChannelList* activeChannels);

  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);


 private:
  static const int kInitEventListSize = 16;

  void fillActiveChannels(int numEvents, ChannelList* activeChannel);
  void update(int oper, Channel* channel);

  typedef std::vector<struct epoll_event> EventList;
  typedef std::map<int, Channel*> ChannelMap;

  EventLoop*  loop_;
  ChannelMap  channels_;
  int epollfd_;
  EventList events_;

};

}

}



#endif //LIBCHEN_EPOLLPOLLER_H
