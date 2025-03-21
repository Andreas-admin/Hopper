.PHONY: all

all:
	clang++ main.cpp defines.cpp logic.cpp init.cpp datum.cpp -o hopper `pkg-config --libs --cflags raylib` -std=c++17
