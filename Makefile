CC = gcc

CFLAGS = -Wall -Wextra -std=c99 -g

TARGET = tp2virtual

SRC_DIR = src

SRCS = $(SRC_DIR)/tp2virtual.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)