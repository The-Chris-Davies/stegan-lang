CC = gcc
FLAGS = -Wall -std=c99
LIBS = -lm
TARGET = stegan
OBJS = $(patsubst %.c, %.o, $(wildcard src/*.c))
HEADERS = $(wildcard src/*.h)

default: $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(TARGET) $(LIBS)

clean: 
	-rm -rf $(OBJS)
	-rm -rf $(TARGET)
