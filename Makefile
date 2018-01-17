CC = gcc
FLAGS = -Wall -std=c99
LIBS = -lm
TARGET = stegan
OBJS = $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))
HEADERS = $(wildcard src/*.h)

default: $(TARGET)

obj/%.o:src/%.c $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@echo "\033[94mLinking...\033[0m"
	$(CC) $(FLAGS) $(OBJS) -o $(TARGET) $(LIBS)

clean:
	@echo $(OBJS)
	@echo $(HEADERS)
	@echo "\033[91mCleaning...\033[0m"
	-rm -rf $(OBJS)
	-rm -rf $(TARGET)
