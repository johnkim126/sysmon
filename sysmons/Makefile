CC=g++
OBJS=sysmons.o main.o
SRCS=sysmons.cpp main.cpp
TARGET=sysmons
CFLAGS=-std=c++11
LDFLAGS=-pthread

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS) :
	$(CC) $(CFLAGS) -c $(SRCS)

clean:
	rm -f $(OBJS) $(TARGET)
