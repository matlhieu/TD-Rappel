CC = gcc
CF = -std=c.Iheaders
PC = pc/main.c pc/create.c pc/destroy.c pc/read_files.c 
	 pc/sort.c pc/save.c pc/display.c
OBJ = $(PC:.c=.o)

TARGET = exec

all: $(TARGET)

$(TARGET): $(OBJ)
    $(CC) $(CF) -o $@ $^

%.o: %.c 
    $(CC) $(CF) -c -o $@ $<

clean:
	@echo "Cleaning"
	rm -f $(OBJ) $(TARGET)
	@echo "Cleaning completed"
