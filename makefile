CC = gcc
CFLAGS = -Wall -Wextra -IcJSON

# Source files
SRCS = gvhdl.c utilities.c cJSON/cJSON.c
OBJS = $(SRCS:.c=.o)

# Name of the executable
TARGET = gvhdl

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
