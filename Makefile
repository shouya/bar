CC = gcc
AR = ar
CFLAGS = -Wall -ansi
LIBS = -lSDL

BIN_DIR = bin
SRC_DIR = src
INC_DIR = include
LIB_DIR = lib
WORK_DIR = `pwd`

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)

DEBUG = yes
PROFILE = yes
OPTIMIZATION = -O3

AI=ai/pierre_dellacherie_ai.c

SOURCES += $(SRC_DIR)/$(AI)

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

all: libbar.so demo auto_ai


libbar.so: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^ $(LIBS) -shared -fpic

demo:
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@	\
	 game/demo/demo_export.c $(BIN_DIR)/libbar.so

auto_ai:
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@	\
	 game/auto_ai/auto_ai.c $(BIN_DIR)/libbar.so


%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(BIN_DIR)/*
	rm -f $(SRC_DIR)/*.o

rebuild: clean all


.PHONY : clean





