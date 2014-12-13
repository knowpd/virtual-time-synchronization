CC = g++
CFLAGS = -O2 -g -Wall -fmessage-length=0 
LIBS = -lboost_system -lboost_thread -llog4cxx -lgtop-2.0
OBJS_EXTERNAL = ../MemoryDatabase.o ../MessageProcessor.o ../MemoryDatabase.o ../SharedMemory.o ../Configuration.o ../TimeGetter ../shared/qemu-timer.o ../shared/bin_sem.o
TARGET = TcpAsyncServerMain UdpAsyncServerMain AsyncServerMain UdpSyncClientMain 


all: $(TARGET)
%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c -o $@ $<

TcpAsyncServerMain: TcpAsyncServerMain.o TcpAsyncServer.o TcpSession.o TcpConnectionManager.o $(OBJS_EXTERNAL)
	$(CC) -o $@ $^ $(LIBS)
UdpAsyncServerMain: UdpAsyncServerMain.o UdpAsyncServer.o $(OBJS_EXTERNAL)
	$(CC) -o $@ $^ $(LIBS)
AsyncServerMain: AsyncServerMain.o TcpAsyncServer.o TcpSession.o TcpConnectionManager.o UdpAsyncServer.o UdpSyncClient.o $(OBJS_EXTERNAL)
	$(CC) -o $@ $^ $(LIBS)
UdpSyncClientMain: UdpSyncClientMain.o UdpSyncClient.o
	$(CC) -o $@ $^ $(LIBS)

#MessageProcessorTestMain.o: MessageProcessor.cpp MemoryDatabase.hpp MessageProcessor.hpp
#chat_client: chat_client.o
#	$(CC) -o $@ $< $(LIBS)

clean:
	rm -f *.o $(TARGET)
