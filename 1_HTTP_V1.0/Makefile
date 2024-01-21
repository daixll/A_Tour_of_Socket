CC = g++
CFLAGS = -std=c++23

all: clean server

server: server.cpp
	$(CC) $(CFLAGS) -o server SOURCES/server.cpp SOURCES/tool.cpp SOURCES/main.cpp

clean:
	rm -f server