
CC=gcc

SRC=redis-test-async.c reactor.c chainbuffer/buffer.c 

TARGET=async

CFLAGS=-lhiredis -ggdb

all: $(TARGET)
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)
	
	
clean:
	rm -rf $(TARGET)