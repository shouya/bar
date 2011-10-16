CC = gcc
AR = ar
CFLAGS = -Wall -ansi
LIBS = -lSDL

BIN_DIR = bin
SRC_DIR = src
INC_DIR = include
LIB_DIR = lib
WORK_DIR = `pwd`

SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)

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

all: libbar.so demo


libbar.so: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^ $(LIBS) -shared -fpic

demo:
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@	\
	 game/demo/demo_export.c $(BIN_DIR)/libbar.so


%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(BIN_DIR)/*
	rm -f $(SRC_DIR)/*.o

rebuild: clean all


.PHONY : clean





