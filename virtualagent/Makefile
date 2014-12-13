CC = g++
CFLAGS = -O2 -g -Wall -fmessage-length=0 
LIBS = -lboost_system -lboost_thread -llog4cxx -lgtop-2.0
INCLUDE = -I. -I/usr/include/libgtop-2.0/ -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include
TARGET = virtualagentd

OBJS = virtualagentd.o VirtualAgent.o VirtualTime.o SharedMemory.o MemoryDatabase.o \
MessageProcessor.o CpuMonitor.o Daemonizer.o SignalHandler.o InterfaceConfigurator.o \
TimeGetter.o Configuration.o

OBJS_NET = net/TcpAsyncServer.o net/TcpSession.o net/TcpConnectionManager.o \
net/UdpAsyncServer.o net/UdpSyncClient.o

OBJS_SHARED = shared/qemu-timer.o shared/bin_sem.o

all: $(TARGET)
%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDE)
%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDE)
virtualagentd: $(OBJS) $(OBJS_NET) $(OBJS_SHARED)
	$(CC) -o $@ $^ $(LIBS) $(INCLUDE)

clean:
	rm -f *.o $(TARGET)
