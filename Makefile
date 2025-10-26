CC = gcc

CFLAGS = -Wall -Wextra -std=c99

PC = pc/main.c pc/create.c pc/destroy.c pc/read_files.c \
	pc/sort.c pc/save.c pc/display.c
OBJ = $(PC:.c=.o)

TARGET = exec

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CF) -o $@ $^ -lm

%.o: %.c 
	$(CC) $(CF) -c -o $@ $<

clean:
	@echo "Cleaning"
	rm -f $(OBJ) $(TARGET)
	@echo "Cleaning completed"