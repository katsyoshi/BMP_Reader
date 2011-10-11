GCC	= gcc4.6
TARGET	= bmp
OBJS	= bmp.o main.o
CFLAGS	= -Wall -std=c99

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

.c.o :
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f $(OBJS) $(TARGET)