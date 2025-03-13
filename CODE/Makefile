# Makefile for CPU Scheduler Implementation

# Compiler and flags
CC = gcc
CFLAGS = -Wall

# Target executable
TARGET = cpu_scheduler

# Source files
SRC = skeleton_code_assignment_3.c

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean rule
clean:
	rm -f $(TARGET) *.o

# Phony targets
.PHONY: all clean 