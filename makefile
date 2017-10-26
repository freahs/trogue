BIN_NAME=trogue

INC_DIR=inc
SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin
LIB_DIR=lib

RELEASE_FLAGS=-DNDEBUG
DEBUG_FLAGS=#-Q

_OBJ = xterm_display.o shadowcast.o scene.o movementsystem.o tilesystem.o playersystem.o aisystem.o tile.o
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

#fdiagnostics requirec gcc 4.9+
CC_FLAGS=-fdiagnostics-color=always -std=c++11 -Wall -pedantic -pthread -g$(CFLAGS)

CC=g++

all: debug

debug: debug_compile
debug_compile: CC_FLAGS += $(DEBUG_FLAGS)
debug_compile: compile

release: clean release_compile
release_compile: CC_FLAGS += $(RELEASE_FLAGS)
release_compile: compile

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.hpp
	$(CC) $(CC_FLAGS) -I $(INC_DIR) -c -o $@ $<

compile: $(SRC_DIR)/$(BIN_NAME).cpp $(OBJ)
	$(CC) $(CC_FLAGS) -I $(INC_DIR) -L $(LIB_DIR) $^ -ltyra -o $(BIN_DIR)/$(BIN_NAME)

.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(BIN_DIR)/*
	cp ../libtyra/lib/libtyra.a ./lib/
	cp -r ../libtyra/inc/* ./inc/tyra/
	cp ../ansi_format/inc/ansi_format.hpp ./inc/

lib:
