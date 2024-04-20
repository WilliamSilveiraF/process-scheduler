# Compiler to use
CC = g++

# Compiler flags
CFLAGS = -Wall -std=c++11 -IReadFile -IScheduler -IProcess -ISchedulingAlgorithm -ICPU -IFCFS -ISJF -IPNP -IPP -IRoundRobin

# Target executable name
TARGET = main

# .cc files 
SRC_FILES = Scheduler/Scheduler.cc Process/Process.cc CPU/CPU.cc FCFS/FCFS.cc read_file.cc
OBJ_FILES = $(SRC_FILES:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cc
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_FILES) $(TARGET)