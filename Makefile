CC = gcc
CFLAGS = -Wall -ansi
DEPS = customer.h mydefs.h myutils.h simulation2.h
OBJ = main.o customer.o myutils.o simulation2.o
EXENAME = PQS

$(EXENAME): $(OBJ)
	$(CC) -o $@ $^ -lpthread

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(EXENAME) $(OBJ)
