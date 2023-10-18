SRC_DIR = src
BUILD_DIR = build/debug
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_NAME = output
INCLUDE_PATHS = -I include/SDL2 -I include/SDL2_image/SDL2 -I include/SDL2_ttf/SDL2 -I include/MiniAudio.h
LIBRARY_PATHS = -L/opt/homebrew/Cellar/sdl2/2.26.1/lib -L/opt/homebrew/Cellar/sdl2_image/2.6.3_1/lib -L/opt/homebrew/Cellar/sdl2_ttf/2.20.2/lib
LINKER_FLAGS = -lsdl2 -lsdl2_image -lsdl2_ttf
COMPILER_FLAGS = -std=c++11 -Wall -O0 -g

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)