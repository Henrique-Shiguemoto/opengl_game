COMPILER=g++
COMPILER_FLAGS=-g -Wall -Wextra -pedantic -std=c++11 -Wno-unused-parameter
INCLUDE_PATH=-Isrc/include
LIBRARY_PATH=-Lsrc/lib
LINKER_FLAGS=-lmingw32 -lSDL2main -lSDL2
SRC=src/main.cpp src/glad.c src/Shader.cpp src/VertexBuffer.cpp src/VertexArray.cpp src/IndexBuffer.cpp src/Game.cpp src/Camera.cpp src/Texture.cpp
OBJS=main.o glad.o Shader.o VertexBuffer.o VertexArray.o IndexBuffer.o Game.o Camera.o Texture.o
BIN=src/main.exe
DEBUGGER=gdb

all: compile link

# .cpp and .c files into .o files
compile:
	$(COMPILER) $(SRC) $(COMPILER_FLAGS) -c $(INCLUDE_PATH)

# .o files into executable
link:
	$(COMPILER) $(OBJS) -o $(BIN) $(LIBRARY_PATH) $(LINKER_FLAGS)

# run the executable
run:
	$(BIN)

# call debugger on the executable
debug:
	$(DEBUGGER) $(BIN)

# clear unnecessary files
clean:
	del *.o *.exe *.s *.trace src\*.o src\*.exe src\*.s src\*.trace