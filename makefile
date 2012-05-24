
CC=gcc
CFLAGS = -m32 -Wall -lm

degree:
	$(CC) $(CFLAGS) closeall.c discharg.c disopt.c globcor.c initial.c input.c degree.c radiat.c snowinput.c snowmodel.c tindex.c turbul.c userfile.c writeout.c scaling.c -o  degree 

meltmod:
	$(CC) $(CFLAGS) closeall.c discharg.c disopt.c globcor.c initial.c input.c meltmod.c radiat.c snowinput.c snowmodel.c tindex.c turbul.c userfile.c writeout.c scaling.c -o meltmod 
