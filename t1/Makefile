CC = gcc
CFLAGS = -Wall -I.
TARGET = spoofer 
DEPS = 
OBJ = 

all: $(TARGET)

spoofer: spoofer.o $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -fr *.o
	rm -fr $(TARGET)
