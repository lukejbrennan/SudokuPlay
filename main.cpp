#include <iostream>
#include "Puzzle.h"

int main(){
	
//only reads ints then chars because we call the int constructor before the char constructor

	//Initialize blank Puzzle of ints
	Puzzle <int> intPuzzle;

	//Play game
	intPuzzle.playSudoku();

	return 0;
}

