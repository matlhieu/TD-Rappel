CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g
SRCS = main.c function.c
OBJS = $(SRCS:.c=.o)
TARGET = student_app

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)
