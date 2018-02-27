//
// Created by 陈国威 on 2018/2/4.
//

#ifndef LIBCHEN_CALLBACK_H
#define LIBCHEN_CALLBACK_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include "Buffer.h"

namespace chen
{
namespace net
{

	class TcpConnection;
	class Buffer;

	typedef boost::shared_ptr<TcpConnection>	TcpConnectionPtr;

	typedef boost::function<void(const TcpConnectionPtr&)>	ConnectionCallback;
	typedef boost::function<void(const TcpConnectionPtr&)>  CloseCallback;

	typedef boost::function<void (const TcpConnectionPtr&,
	                          Buffer* buffer)>	MessageCallback;

	void defaultConnectionCallback(const TcpConnectionPtr& connfd);

	void defaultMessageCallback(const TcpConnectionPtr& conn,
						Buffer* buffer);
}
}

#endif //LIBCHEN_CALLBACK_H
