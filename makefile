#myshell is a homework for computer system
#the project is not complete, need to add more directive
#i think callback function will be more suitable for these

CC=gcc

TARGET=main
CTAGS=-Wall -O2 -o $(TARGET)
LINK=-lpthread

OBJECTS=main.o mysh.o csapp.o

all:$(TARGET)
$(TARGET):$(OBJECTS)
	$(CC) $(CTAGS) $(OBJECTS) $(LINK)
clean:
	rm -rf main
	rm -rf *.o

