TARGET := k11s
SRC := $(TARGET).cpp
OBJ := $(TARGET).o

CFLAGS := -g -Wall
LFLAGS := -lm -pthread
OPTION := `pkg-config --cflags --libs opencv4`
CC := g++ 

INCLUDES := -I./common  # usbcamcom.h ファイルのあるディレクトリへのパス

all: $(TARGET)

$(TARGET): $(SRC) 
	$(CC) $(INCLUDES) $^ $(OPTION) -o $@

clean:
	rm -f $(TARGET) $(OBJ)

