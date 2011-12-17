
all: stackoverflow-cli

OBJ = requests.o arguments.o stackoverflow-cli.o

requests.o: requests.c stackoverflow-cli.h
arguments.o: arguments.c stackoverflow-cli.h
stackoverflow-cli.o: stackoverflow-cli.c stackoverflow-cli.h

%.o: %.c
	$(CC) -c $< -o $@

stackoverflow-cli: $(OBJ)
	$(CC) -o $@ -lcurl $(OBJ)

clean:
	rm -rf *.o stackoverflow-cli