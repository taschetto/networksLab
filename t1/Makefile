CC = gcc
CFLAGS = -Wall -I.
TARGET = spoofer 
DEPS = defines.h helpers.h packets.h
OBJ = helpers.c packets.c

all: $(TARGET)

$(TARGET): $(TARGET:%=%.o) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -fr *.o
	rm -fr $(TARGET)
