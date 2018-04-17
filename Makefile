EXEC = Server
LD = gcc
CCFLAGS = -Wall -lpthread

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(LD) $^ -o $@ $(CCFLAGS)
	rm $(OBJ)

.PHONY: clean

clean:
	rm $(EXEC)