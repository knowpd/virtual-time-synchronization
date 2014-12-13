CC = g++
CFLAGS = -O2 -g -Wall -fmessage-length=0
OBJS = qemu-timer.o bin_sem.o
LIBS = -lrt -lpthread
TARGET =

all: $(OBJS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<	
		
clean:
	rm -f $(OBJS) $(OBJS2) $(TARGET)
