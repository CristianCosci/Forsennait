CC = gcc
CFLAGS = -Werror -Wall -pedantic -lrt 
OBJ = gamelib.o main.o

.PHONY:	clean

%.o : %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

forsennait : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

main : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean : 
	@ echo "rimozione file compilati"
	@ rm *.o
	@ if [ -e forsennait ]; then rm forsennait; fi
	@ if [ -e main ]; then rm main; fi