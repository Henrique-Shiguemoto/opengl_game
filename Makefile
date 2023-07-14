COMPILER=g++
COMPILER_FLAGS=-g -Wall -Wextra -pedantic -std=c++11 -Wno-unused-parameter
INCLUDE_PATH=-Isrc/include
LIBRARY_PATH=-Lsrc/lib
LINKER_FLAGS=-lmingw32 -lSDL2main -lSDL2
SRC=src/main.cpp src/glad.c src/VertexBuffer.cpp src/VertexArray.cpp src/IndexBuffer.cpp
OBJS=main.o glad.o VertexBuffer.o VertexArray.o IndexBuffer.o
BIN=src/main.exe
DEBUGGER=gdb

all: compile link

compile:
	$(COMPILER) $(SRC) $(COMPILER_FLAGS) -c $(INCLUDE_PATH)

link:
	$(COMPILER) $(OBJS) -o $(BIN) $(LIBRARY_PATH) $(LINKER_FLAGS)

run: all
	$(BIN)

debug: all
	$(DEBUGGER) $(BIN)

clean:
	del *.o *.exe src\*.o src\*.exe