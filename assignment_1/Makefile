CC = gcc
CFLAGS = -O3
LIBS = -lpapi

TARGET = mmm
SRCS = mmm.c

# Build rule
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

# Clean rule
clean:
	rm -f $(TARGET)