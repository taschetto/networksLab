CC = g++
CFLAGS = -std=c++11 -pthread -Wall -I.
TARGET = phantom 
DEPS = colors.h helpers.h interface.h
OBJ = helpers.o interface.o

all: $(TARGET)

$(TARGET): $(TARGET:%=%.o) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -fr *.o
	rm -fr $(TARGET)
