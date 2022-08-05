CC = gcc
CFLAGS = -Wall -g 
OBJ = main.o sr.o
sr:$(OBJ)
	$(CC) $(CFLAGS) $^ -o $@
%.c:%.o
	$(CC) $(CFLAGS) $^ -o $@

.PHONY:clean
clean:
	$(RM) *.o sr -r
