CC = gcc
CFLAGS = -O3
LIBS = -lpapi

TARGET = mmm
SRCS = mmm.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

clean:
	rm -f $(TARGET)