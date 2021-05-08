CC = gcc
CFLAGS_default = -std=gnu11 -Werror -Wall -Wextra -O1
CFLAGS_auto_vec = -std=gnu11 -Werror -Wall -Wextra -O1 -ftree-vectorize
LDFLAGS = -fopenmp

.PHONY: all clean

all: task2

clean:
	$(RM) task2

task2:
	$(CC) $(CFLAGS_default) $(LDFLAGS) task2.c -o task2 



