CC = gcc
CFLAGS = -Wall -ansi
LIBS = -lSDL

BIN_DIR = bin
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
WORK_DIR = `pwd`

SOURCES = $(wildcard src/*.c)
OBJECTS = $(subst, $(SRC_DIR)/,$(OBJ_DIR)/, $(subst .c,.o,$(SOURCES)))

DEBUG = no
PROFILE = no
OPTIMIZATION = -O3

ifeq ($(DEBUG), yes)
	OPTIMIZATION = -O0
	CFLAGS += -g
endif

ifeq ($(PROFILE), yes)
	CFLAGS += -pg
endif

CFLAGS += $(OPTIMIZATION)
CFLAGS += -I$(INC_DIR)

test: all

all: libbar


libbar: $(OBJECTS)
	gcc $(CFLAGS) -o $(LIB_DIR)/$@ $^ $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $(WORK_DIR)/$(OBJ_DIR)/$@

clean:
	rm -f $(BIN_DIR)/*
	rm -f $(OBJ_DIR)/*.o

rebuild: clean all


.PHONY : clean





