all: main

main: main.cpp Puzzle.h
	g++ -g main.cpp -o main

clean:
	rm main
