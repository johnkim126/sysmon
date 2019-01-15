CC=g++
OBJS=sysmond.o main.o
SRCS=sysmond.cpp main.cpp
TARGET=sysmond
CFLAGS=-std=c++11
LDFLAGS=-pthread

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

#.cpp.o:
#	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

$(OBJS) :
	$(CC) $(CFLAGS) -c $(SRCS)

clean:
	rm -f $(OBJS) $(TARGET)
