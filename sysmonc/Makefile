CC=g++
OBJS=sysmonc.o
SRCS=sysmonc.cpp
TARGET=sysmonc
CFLAGS=-std=c++11
LDFLAGS=-pthread

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS) :
	$(CC) $(CFLAGS) -c $(SRCS)

clean:
	rm -f $(OBJS) $(TARGET)
