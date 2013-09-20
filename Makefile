# Handy global variables
MODEL_SRC = ./src
MODEL_BUILD_TMP = ./build
BINARY_DIR = ./bin

# Compiler Flags
CC=gcc
CFLAGS = -Wall -O0 -g -m32
LDFLAGS = -lm

#### Model Building Stuff ####


# Generic object file build rule
$(MODEL_BUILD_TMP)/%.o: $(MODEL_SRC)/%.c
	@echo "Building $@"
	@mkdir -p $(MODEL_BUILD_TMP)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@ 

## DETIM

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
								scaling.c \
								detim.c

DETIM_OBJECTS = $(addprefix $(MODEL_BUILD_TMP)/, $(DETIM_SOURCES:.c=.o))

detim: $(DETIM_OBJECTS)
	@mkdir -p $(BINARY_DIR)
	@echo "Building DETiM:"
	$(CC) $(CFLAGS) $(DETIM_OBJECTS) -o  $(BINARY_DIR)/detim $(LDFLAGS)

## Source-files requires by DEBaM
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
								writeout.c \
								debam.c

DEBAM_OBJECTS = $(addprefix $(MODEL_BUILD_TMP)/, $(DEBAM_SOURCES:.c=.o))

debam: $(DEBAM_OBJECTS)
	@mkdir -p $(BINARY_DIR)
	@echo "Building DEBAM:"
	$(CC) $(CFLAGS) $(DEBAM_OBJECTS) -o  $(BINARY_DIR)/debam $(LDFLAGS)


models: debam detim


#### Utility Programs ####

# Grid tools
ascigrid: ./util/grid_tools/ascigrid.c
	@mkdir -p $(BINARY_DIR)
	@echo "Building ascigrid"
	$(CC) $(CFLAGS)  $< -o $(BINARY_DIR)/$@ $(LDFLAGS)

gridasci: util/grid_tools/gridasci.c
	@mkdir -p $(BINARY_DIR)
	@echo "Building gridasci"
	$(CC) $(CFLAGS)  $< -o $(BINARY_DIR)/$@ $(LDFLAGS)

gridtools: ascigrid gridasci

# Shading calculator

shading: ./util/shading/shading.c
	@mkdir -p $(BINARY_DIR)
	@echo "Building shading"
	$(CC) $(CFLAGS) $< -o $(BINARY_DIR)/$@ $(LDFLAGS)

utils: gridtools shading

all: models utils



clean:
	rm -r $(MODEL_BUILD_TMP)
	rm -r $(BINARY_DIR) 
