
CC=gcc
CFLAGS = -m32 -Wall -O0 -g
LDFLAGS = -lm
SOURCES = closeall.c discharg.c disopt.c globcor.c initial.c input.c radiat.c snowinput.c snowmodel.c tindex.c turbul.c userfile.c writeout.c scaling.c
OBJECTS = $(SOURCES:.c=.o)


# all, degree and meltmod are the targets we usually care about.
all: degree meltmod

degree: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) degree.c -o  degree 

meltmod: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) meltmod.c -o meltmod 

clean:
	rm -rf degree meltmod *.o *.dSYM

# object files for work debugging

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@