CC = g++
CFLAGS = -std=c++11 -Wall -I.
TARGET = spoofer 
DEPS = defines.h helpers.h ethernet.h arp.h ip.h
OBJ = helpers.o ethernet.o arp.o ip.o

all: $(TARGET)

$(TARGET): $(TARGET:%=%.o) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -fr *.o
	rm -fr $(TARGET)
