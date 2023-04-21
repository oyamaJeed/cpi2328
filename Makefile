# TARGET NAME
TARGET := test2

# SOURCE FILES
SRC := $(TARGET).c

# OBJECT FILES
OBJ := $(TARGET).o

# SHARED OBJECT FILES
SO  := libdlink.so

# COMPILER OPTION
CFLAGS := -g -Wall

LFLAGS := -lm -pthread

SHARED := -shared

CC := gcc $(CFLAGS) $(LFLAGS)

all : $(TARGET)

# LINK OBJECTS
$(TARGET): $(OBJ) 
	$(CC) -o $(TARGET) $(SRC) -L. -ldlink
	export LD_LIBRARY_PATH="/home/meit2328/cpi2328"
	
# COMPILE SOURCES
.c.o:
	$(CC) -c $^

# CLEAN FILES (PHONY TARGET)
clean:
	rm -f $(TARGET) $(OBJ)

