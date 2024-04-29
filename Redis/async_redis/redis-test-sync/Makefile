
CC=gcc

SRC=redis-test-sync.c

TARGET=st

CFLAGS=-lhiredis

all: $(TARGET)
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)
	
clean:
	rm -rf $(TARGET)