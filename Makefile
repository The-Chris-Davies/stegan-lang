CC = gcc
FLAGS = -Wall -std=c99
LIBS = -lm
OBJS = interpreter.o interpreterDefs.o
TARGET = stegan
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

default: $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(TARGET) $(LIBS)

clean: 
	-rm -rf $(OBJS)
	-rm -rf $(TARGET)
