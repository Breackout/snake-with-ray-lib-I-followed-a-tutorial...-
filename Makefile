CC = clang++
CFLAGS = $(shell pkg-config --cflags raylib)
LDFLAGS = $(shell pkg-config --libs raylib)

SRC = src/main.cpp
OUT = main

build:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)

run: build
	./$(OUT)

clean:
	rm -f $(OUT)
