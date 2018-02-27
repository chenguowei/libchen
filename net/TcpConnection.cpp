//
// Created by 陈国威 on 2018/2/4.
//
#include <boost/bind.hpp>

#include "TcpConnection.h"
#include "Socket.h"
#include "EventLoop.h"
#include "Channel.h"
#include "SocketsOps.h"

#include <errno.h>

using namespace chen;
using namespace chen::net;


void chen::net::defaultConnectionCallback (const TcpConnectionPtr &connfd)
{
  std::cout << connfd->localAddress ().toIpPort () << " ->" << connfd->peerAddress ().toIpPort () << endl;
}

void chen::net::defaultMessageCallback (const TcpConnectionPtr &conn, Buffer *buffer)
{
  buffer->retrieveAll ();
}

TcpConnection::TcpConnection (EventLoop *loop,
							  const string &name,
							  int sockfd,
							  const InetAddress &localaddr,
							  const InetAddress &peeraddr)
	:loop_(loop),
	 socket_(new Socket(sockfd)),
	 connetChannel_(new Channel(loop, sockfd)),
	 name_(name),
	 localaddr_(localaddr),
	 peeraddr_(peeraddr),
	 reading_(true),
	 state_(kConnecting)
{
  connetChannel_->setReadCallback (
	  boost::bind(&TcpConnection::handleRead, this));

  connetChannel_->setWriteCallback (
	  boost::bind(&TcpConnection::handleWrite, this));

  connetChannel_->setCloseCallback (
	  boost::bind (&TcpConnection::handleClose, this)
  );

  connetChannel_->setErrorCallback (
	  boost::bind (&TcpConnection::handleError, this)
  );

  socket_->setKeepAlive (true);
}

TcpConnection::~TcpConnection ()
{
  assert (state_ == kDisconnected);
}


void TcpConnection::send (const void *message, size_t len)
{
  send(string(static_cast<const char*>(message), len));
}

void TcpConnection::send (const string &message)
{
	if (kConnected)
	  {
		sendInLoop (message);
	  }
}

void TcpConnection::send(Buffer* buffer)
{
  if (state_ == kConnected)
	{
	  sendInLoop (buffer->peek (), buffer->readableBytes ());
	  buffer->retrieveAll ();
	}
}

void TcpConnection::sendInLoop (const string &message)
{
  sendInLoop (message.data(), message.size ());
}

void TcpConnection::sendInLoop (const void *message, size_t len)
{
  ssize_t nwrote = 0;
  size_t remaining = len;
  bool faultError = false;
  if (state_ == kDisconnected)
	{
	  std::cout << "disconnected, give up writing\n";
	  return;
	}

  if (!connetChannel_->isWriting () && outputBuffer_.readableBytes () == 0)
	{
	  nwrote = sockets::write (connetChannel_->fd (), message, len);
	  if (nwrote >= 0)
		{
		  remaining = len - nwrote;
		  if (remaining == 0)
			{

			}
		}
	  else	//nwrote < 0
		{
		  nwrote = 0;
		  if (errno != EWOULDBLOCK)
			{
			  std::cout << "Tcpconnection::sendInLoop\n";
			  if (errno == EPIPE || errno == ECONNRESET)
				{
				  faultError = true;
				}
			}
		}
	}

  assert (remaining <= len);
  if (!faultError && remaining > 0)
	{
	  size_t oldLen = outputBuffer_.readableBytes ();

	  outputBuffer_.append (static_cast<const char*>(message)+nwrote, remaining);
	  if (!connetChannel_->isWriting ())
		{
		  connetChannel_->enableWriting ();
		}
	}
}

void TcpConnection::startRead ()
{
  loop_->runInLoop (
	  boost::bind (&TcpConnection::startReadInLoop, this)
  );
}

void TcpConnection::startReadInLoop ()
{
  if (!reading_ || !connetChannel_->isReading ())
	{
	  connetChannel_->enableReading ();
	  reading_ = true;
	}
}

void TcpConnection::stopRead ()
{
  loop_->runInLoop (
	  boost::bind (&TcpConnection::stopReadInLoop, this)
  );
}

void TcpConnection::stopReadInLoop ()
{
  if (reading_ || connetChannel_->isReading ())
	{
	  connetChannel_->disableReading ();
	  reading_ = false;
	}
}


void TcpConnection::connectEstablished ()
{
  assert (state_ == kConnecting);
  setState (kConnected);
  connetChannel_->enableReading ();

  connectionCallback_(shared_from_this ());
}

void TcpConnection::connectDestroyed ()
{
  if (state_ == kConnected)
	{
	  setState (kDisconnected);
	  connetChannel_->disableAll ();

	  connectionCallback_(shared_from_this ());

	}

  connetChannel_->remove ();
}

void TcpConnection::handleRead()
{
  int savedErrno = 0;
  ssize_t n = inputBuffer_.readFd (connetChannel_->fd (), &savedErrno);
  if (n > 0)
	{
	  messageCallback_(shared_from_this(), &inputBuffer_);
	}
  else if (n == 0)
	{
	  handleClose ();
	}
  else
	{
	  errno = savedErrno;
	  handleError ();
	}

}

void TcpConnection::handleWrite ()
{
  if (connetChannel_->isWriting ())
	{
	  ssize_t n = sockets::write (connetChannel_->fd (),
	  								outputBuffer_.peek (),
	  								outputBuffer_.readableBytes ());

	  if (n > 0)
		{
		  outputBuffer_.retrieve (n);
		  if (outputBuffer_.readableBytes () == 0)
			{
			  connetChannel_->disableWriting ();

			  if (state_ == kDisconnecting)
				{

				}

			}
		}
	  else
		{

		}
	}
  else
	{

	}
}

void TcpConnection::handleClose ()
{

  assert (state_ == kConnected || state_ == kDisconnecting);

  setState (kDisconnected);
  connetChannel_->disableAll ();

  TcpConnectionPtr guardThis(shared_from_this ());
  connectionCallback_(guardThis);

  closeCallback_(guardThis);
}

void TcpConnection::handleError ()
{

}

