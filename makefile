
CC=gcc
CFLAGS = -m32 -Wall -O0 -g
LDFLAGS = -lm
DEG_SOURCES = degree.c closeall.c discharg.c disopt.c globcor.c initial.c input.c radiat.c snowinput.c snowmodel.c tindex.c turbul.c userfile.c writeout.c scaling.c
DEG_OBJECTS = $(DEG_SOURCES:.c=.o)
MM_SOURCES = meltmod.c closeall.c discharg.c disopt.c globcor.c initial.c input.c radiat.c snowinput.c snowmodel.c turbul.c userfile.c writeout.c
MM_OBJECTS = $(MM_SOURCES:.c=.o)

# all, degree and meltmod are the targets we usually care about.
all: degree meltmod

degree: $(DEG_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DEG_OBJECTS) -o degree 

meltmod: $(MM_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(MM_OBJECTS) -o meltmod 

clean:
	rm -rf degree meltmod *.o *.dSYM

# object files for work debugging

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@