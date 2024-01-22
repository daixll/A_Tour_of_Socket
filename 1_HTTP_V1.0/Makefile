CC = g++
CFLAGS = -std=c++23

all: clean server

server: SOURCES/server.cpp SOURCES/tool.cpp main.cpp
	$(CC) $(CFLAGS) -o server SOURCES/server.cpp SOURCES/tool.cpp main.cpp

clean:
	rm -f server