OBJS = go.o 


CC=gcc 
CFLAGS= -O2 -I/home/briansm/denoise_speex/libspeex
LDFLAGS=  -O -lm

all:    $(OBJS)
	$(CC) -o go go.o -lasound -lspeexdsp
	





clean: 
	rm -f *.o *~

.c.o:	$<
	$(CC) $(CFLAGS) -c $<

