CC=gcc
OBJS=sysmon.o
TARGET=sysmon

.SUFFIXES : .c .o

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS)

clean:
	rm -f *.o $(OBJS) $(TARGET)
