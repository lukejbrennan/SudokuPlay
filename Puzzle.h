#ifndef Puzzle_H
#define Puzzle_H

#include <iostream>
using namespace std;

#include <vector>
#include <cstdlib>
#include <fstream>
#define VICTORY 1
#define VALID_MOVE 1

template <typename T>
class Puzzle{
										
	public:
		//default constructor (reads in the file sets values for puzzle
		Puzzle(); 
	
		//play the game (display board, ask for a move, call moveCheck(), call winCheck() )
		void playSudoku();

		//Determine if an entry is valid or not on a board		
		//VALID_MOVE is returned if move is acceptable and valid
		int moveCheck(int row_in, int col_in, int numChoice);

		//Determine if the player has won the game yet. 
		//VICTORY is output if the player has won
		int winCheck(); 

		//display puzzle
		void printPuzzle();

	private:
		//vector of vectors
		vector<vector <T> > puzzle; 
		int size;		

}; //end class Puzzle

template<typename T>
Puzzle<T>::Puzzle(){
	size=9;
	string fileName;
	vector<T> tempVec;
	T value; //char or int (for this program)

	//Readin file to fileName
	cout << "Please give a file name: ";
	cin >> fileName;
	ifstream myReadFile;
	myReadFile.open(fileName.c_str());

	//obtain values of fileName until EOF
	while(!myReadFile.eof()){
		for(int row=0; row<size; row++){ 
			for(int col=0; col<size; col++){ 
				myReadFile >> value; //readin one value at a time
				if(value == ' ') continue; //if there's a space in the file, don't put it into the puzzle
				else tempVec.push_back(value); //readin values to the rowVector, tempVec				
			}
			puzzle.push_back(tempVec); //pushes each row into the puzzle
			tempVec.clear(); // erases the row in preparation for the new row
		}
	}
}//end default constructor 

//play the game (display board, ask for a move, call moveCheck(), call winCheck() )
template<typename T>
void Puzzle<T>::playSudoku(){
	int rowChoice, colChoice, numChoice; //inputs from the user
	int row, col; //row=rowChoice-1 so that the first row=0 (similar with col)	
	int moveValidity= !VALID_MOVE; // These are equal when a valid move is chosen.
	int winStatus= !VICTORY; 	
	while(winStatus !=VICTORY){ //while the player has not won	
		//display board
		printPuzzle();
	
		// ask for a move and keep asking until a valid move has been selected
		while(!moveValidity){ //while move choice is invalid
			//ask for a move
			cout << "Select a row" << endl;
			cin >> rowChoice;
			//Check row input
			if( rowChoice>size || rowChoice <1 ){
				cout << "That's not a row..." << endl;
				moveValidity= !VALID_MOVE;
				break;
			}
			//Check Column input
			cout << "Select a column" << endl;
			cin >> colChoice;
			if( colChoice>size || colChoice <1 ){
				cout << "That's not a column..." << endl;
				moveValidity= !VALID_MOVE;
				break;
			}
			//shift rowChoice and colChoice over by one
			row= rowChoice-1;
			col= colChoice-1;

			//Check position to avoid overwriting (doesn't allow player to erase any past mistakes...)
			if( puzzle[row][col]!=0 ){
				cout << "There's already a number there, and you can't overwrite it!" << endl;
				break;
			}
			//Check number input
			cout << "Select a number [1-9]" << endl;
			cin >> numChoice;	
			if( numChoice>9 || numChoice <1 ){
				cout << "That's not a number between 1 and 9..." << endl;
				moveValidity= !VALID_MOVE;
				break;
			}
			
			
			//call "moveCheck()" which returns VALID_MOVE if the move is valid
			moveValidity=moveCheck(row, col, numChoice); 
		}
		if(moveValidity) puzzle[row][col]= numChoice; // if the move is valid, add it to the board
		moveValidity=!VALID_MOVE; //reset option for the next iteration
		//call "winCheck()"
		winStatus=winCheck();
	}

}//end of playSudoku function

template<typename T>
int Puzzle<T>::moveCheck(int row_in, int col_in, int numChoice){
	vector<T> numHistory; //vector to keep track of duplicated numbers
	
	
	//ENSURE COLUMNS contain no repeat numbers [1-9]	
	numHistory.push_back(numChoice); // add numChoice into history of numbers 
	for(int row=0; row<size; row++){ //iterate through rows
		for(int i=0; i<numHistory.size(); i++){ //iterate through numHistory
			//if there is a match (other than zero) in the current column to numHistory, selection is invalid
			if(puzzle[row][col_in]==numHistory[i] && puzzle[row][col_in]!=0){
				cout << numChoice << " already exists in the column " << (col_in+1);
				cout << ". No changes made to the puzzle board" << endl;				
				return !VALID_MOVE;
			} 
			//if there is no match between numHistory and the numbers in current column and if the current position is
			//not zero, then add the value at the position on the board where there is not a match
			else if (puzzle[row][col_in]!=numHistory[i] && puzzle[row][col_in]!=0){  
				numHistory.push_back(puzzle[row][col_in]); 
				break;	//once we've recorded the number that did not match our numHistory, we can move to the next row		
			}
			else break; //this is the case where the current position of puzzle is zero
		}			
	}
	numHistory.clear();

	//ENSURE ROWS contain no repeat numbers [1-9]
	numHistory.push_back(numChoice); // add numChoice into history of numbers 
	for(int col=0; col<size; col++){
		for(int i=0; i<numHistory.size(); i++){
			if(puzzle[row_in][col]==numHistory[i] && puzzle[row_in][col]!=0){
				cout << numChoice << " already exists in the row " << (row_in+1);
				cout << ". No changes made to the puzzle board" << endl;				
				return !VALID_MOVE;
			} 		
			//add non-documented numbers to numHistory (if the numbers are not zero)	
			else if (puzzle[row_in][col]!=numHistory[i] && puzzle[row_in][col]!=0){
				numHistory.push_back(puzzle[row_in][col]); 
				break;	//once we've recorded the number that didn't match numHistory, we can move to the next column		
			}
			else break; //this is the case where the current position of puzzle is zero
		}			
	}
	numHistory.clear();

	//ENSURE every mini grid contains no repeat numbers [1-9]
	numHistory.push_back(numChoice); // add numChoice into history of numbers
 
	//move row_in and col_in to the top left hand corner of the current box
	while(row_in % 3 != 0) row_in--; 
	while(col_in % 3 !=0) col_in--; 

	//iterate through mini-grid revelant to selected row and col 
	for(int col=col_in; col<col_in+3; col++){
		for(int row=row_in; row<row_in+3; row++){
			for(int i=0; i<numHistory.size(); i++){ //iterate through numHistory to check for a match
				//if there's a match between numHistory and the value at the current row, col position
				// of the puzzle (and if the match isn't zero!), then output error (can't do that) 
				if(puzzle[row][col]==numHistory[i] && puzzle[row][col]!=0){ 
					cout << numChoice << " already exists in the current mini-grid. ";
					cout << "No changes made to the puzzle board" << endl;				
					return !VALID_MOVE;
				}
				//add non-documented numbers to numHistory (if the numbers are not zero)
				else if (puzzle[row][col]!=numHistory[i] && puzzle[row][col]!=0){
					numHistory.push_back(puzzle[row][col]);
					break;			
				}
				else break; //this is the case where the current position of puzzle is zero
			}
		}
	}
	numHistory.clear();
	return VALID_MOVE;
}//end moveCheck function 

template<typename T>
int Puzzle<T>::winCheck(){
	for (int row=0; row<size; row++){
		for (int col=0; col<size; col++){
			if( puzzle[row][col] == 0 ){
				//cout << "Keep trying!" << endl; //shows that the game has not yet been won				
				return !VICTORY; //if board is empty
			}		
		}		 
	}
	// if we didn't hit any 0's as we swept through the board,  
	// then the board is full with valid inputs and therefore the
	// game is won. 
	cout << "Congratulations! You won! Here is your completed puzzle: " << endl; //victory message
	printPuzzle(); 
	return VICTORY;
}//end winCheck function

template<typename T>
void Puzzle<T>::printPuzzle(){
	cout << endl << "New Board:" << endl << endl;
	for(int row=0; row<size; row++){
		for(int col=0; col<size; col++){
			cout << puzzle[row][col] << " ";
		}
		cout << endl; //print new line for next row
	}

}//end printPuzzle function

#endif

