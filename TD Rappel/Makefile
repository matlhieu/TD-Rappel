CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g
SRCDIR = pc
SRCS = $(SRCDIR)/main.c $(SRCDIR)/function.c
OBJS = $(SRCS:.c=.o)
TARGET = student_app

.PHONY: all clean doc

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(SRCDIR)/*.o $(TARGET)

doc:
	@if [ -f Doxyfile ]; then doxygen Doxyfile; else echo "No Doxyfile found. Create one to generate docs."; fi
