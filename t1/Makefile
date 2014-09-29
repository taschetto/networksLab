CC = g++
CFLAGS = -Wall -I. -std=c++11
TARGET = spoofer 
DEPS = defines.h helpers.h ethernet.h arp.h ip.h
OBJ = helpers.cpp ethernet.cpp arp.cpp ip.cpp

all: $(TARGET)

$(TARGET): $(TARGET:%=%.o) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -fr *.o
	rm -fr $(TARGET)
