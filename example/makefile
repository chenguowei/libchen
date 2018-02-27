
CC=gcc
CXX=g++

NETLIB=../net

INC= -I$(NETLIB) -I/usr/include/boost


TARGET=echo

LIB=-lphtread


CFLAGS= -m64 -g -Wall -DDEBUG -lstdc++
TARGET=echo

OBJ+=$(NETLIB)/Acceptor.o \
     $(NETLIB)/Buffer.o \
     $(NETLIB)/Channel.o \
     $(NETLIB)/EPollPoller.o \
     $(NETLIB)/EventLoop.o \
     $(NETLIB)/InetAddress.o \
     $(NETLIB)/Socket.o \
     $(NETLIB)/SocketsOps.o \
     $(NETLIB)/TcpConnection.o \
     $(NETLIB)/TcpServer.o \
     ./Echo.o

$(TARGET): $(OBJ) $
	$(CXX) $(CFLAGS) -o $@ $^

checklink:$(OBJ)
	$(CXX) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INC) -c -o $@ $<


clean:
	rm -f $(OBJ)
	rm -f $(TARGET)

