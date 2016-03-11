OBJS  = go.o


CC=gcc 
CFLAGS= -O2
LDFLAGS=  -O -lm

all:    $(OBJS)
	$(CC) -o go go.o -lm  -lfftw3f -lspeexdsp -lasound



clean: 
	rm -f *.o *~

.c.o:	$<
	$(CC) $(CFLAGS) -c $<

