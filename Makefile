MODEL_SRC = ./src
MODEL_BUILD_TMP = ./build
BINARY_DIR = ./bin

CC=gcc
CFLAGS = -Wall -O0 -g
LDFLAGS = -lm

$(MODEL_BUILD_TMP)/%.o: $(MODEL_SRC)/%.c
	@echo "Building $@"
	@mkdir -p $(MODEL_BUILD_TMP)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@


## Source-files Required by DETIM
DETIM_SOURCES = closeall.c \
							 	discharg.c \
								disopt.c \
								globcor.c \
							 	grid.c \
								initial.c \
								input.c \
								radiat.c \
								snowinput.c \
							 	snowmodel.c \
								skintemperature.c \
								tindex.c \
								turbul.c\
							 	userfile.c \
								writeout.c \
								scaling.c

DETIM_OBJECTS = $(addprefix $(MODEL_BUILD_TMP)/, $(DETIM_SOURCES:.c=.o))

detim: $(DETIM_OBJECTS)
	@mkdir -p $(BINARY_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DETIM_OBJECTS) $(MODEL_SRC)/detim.c -o  $(BINARY_DIR)/detim


DEBAM_SOURCES = closeall.c \
								discharg.c \
								disopt.c \
								globcor.c \
								grid.c \
								initial.c \
								input.c \
								radiat.c \
								snowinput.c \
								snowmodel.c \
								skintemperature.c \
								turbul.c \
								userfile.c \
								writeout.c

DEBAM_OBJECTS = $(addprefix $(MODEL_BUILD_TMP)/, $(DEBAM_SOURCES:.c=.o))

debam: $(DEBAM_OBJECTS)
	@mkdir -p $(BINARY_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DEBAM_OBJECTS) $(MODEL_SRC)/debam.c -o  $(BINARY_DIR)/debam


clean:
	rm -r $(MODEL_BUILD_TMP)
	rm -r $(BINARY_DIR) 
