//
//  main.c
//  TicTacToe
//
//  Created by Yating Sheng on 1/8/12.
//  Copyright 2012 University of Michigan . All rights reserved.
//

#include <stdbool.h>
#include <stdio.h>

#define BOARD_SIDE (3)
#define BOARD_SIZE (BOARD_SIDE * BOARD_SIDE)
#define NOT_FOUND (-1)

enum
{
	NONE = 0,
	PLAYER,
	COMPUTER,
	TIE
};

typedef char Board[BOARD_SIZE];
//typedef char* Board;
//These two lines means exact same thing, except
//			Board b;

void ticTacToeBoardInitialize(Board board)
{
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		board[i] = NONE;
	}
}

void ticTacToeBoardPrintElement(unsigned char c)
{
	char m[] = " OX";
	printf(" %c ", m[c]);
}

void ticTacToeBoardPrintRow(Board board, unsigned char row)
{
	printf("%c ", 'A'+row);
	for (int i = 0; i < BOARD_SIDE; ++i) 
	{
		ticTacToeBoardPrintElement(board[row*BOARD_SIDE+i]);
		
		if (BOARD_SIDE - 1 != i) 
			printf("|");
	}
	printf("\n");
}

void ticTacToeBoardPrintDivider()
{
	printf("  ");
	for (int i = 0; i < BOARD_SIDE; ++i) 
	{
		printf("---");
		
		if (BOARD_SIDE - 1 != i) 
			printf("+");
	}
	printf("\n");
}

void ticTacToeBoardPrint(Board board)
{
	printf("  ");
	for (int i = 0; i < BOARD_SIDE; ++i) 
	{
		printf(" %d ", i+1);
		if (BOARD_SIDE - 1 != i)
			printf(" ");
	}
	printf("\n");
	
	for (int i = 0; i < BOARD_SIDE; ++i) 
	{
		ticTacToeBoardPrintRow(board, i);
		
		if (BOARD_SIDE - 1 != i)
			ticTacToeBoardPrintDivider();
	}
}

unsigned char ticTacToeBoardWinner(Board board);

bool ticTacToeBoardFull(Board board)
{
	for (int i = 0; i < BOARD_SIZE; ++i) 
	{
		if (NONE == board[i])
			return false;
	}
	return true;
}

int ticTacToeBoardFirstEmptySpace(Board board)
{
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		if (NONE == board[i])
			return i;
	}
	return NOT_FOUND;
}

int ticTacToeBoardNextEmptySpace(Board board, int startIndex)
{
	for (int i = startIndex+1; i < BOARD_SIZE; ++i)
	{
		if (NONE == board[i])
			return i;
	}
	return NOT_FOUND;
}

void ticTacToeBoardComputerOdds(Board board, unsigned int* numFutureGames, unsigned int* numFutureGamesNotLost, bool computerTurn)
{
	unsigned char winner = ticTacToeBoardWinner(board);
	
	// Base case: Game is over
	if (NONE != winner)
	{
		++(*numFutureGames);
		if (PLAYER != winner) 
			++(*numFutureGamesNotLost);
		return;
	}
	
	int firstEmptySpace = ticTacToeBoardFirstEmptySpace(board);
	
	while (firstEmptySpace != NOT_FOUND)
	{
		board[firstEmptySpace] = computerTurn ? COMPUTER : PLAYER;
		ticTacToeBoardComputerOdds(board, numFutureGames, numFutureGamesNotLost, !computerTurn);
		
		board[firstEmptySpace] = NONE;
		firstEmptySpace = ticTacToeBoardNextEmptySpace(board, firstEmptySpace);
	}
}
// numFutureGames = 2
// numGamesNotLost = 2
// bestOdds = 1
// bestSpot = 7
// i = 7
// XOO
// OXX
//  

void ticTacToeBoardComputerTurn(Board board)
{
	int bestSpot = 0;
	float bestOdds = 0;
	
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		if (NONE == board[i])
		{
			board[i] = COMPUTER;
			unsigned int numFutureGames = 0;
			unsigned int numFutureGamesNotLost = 0;
			ticTacToeBoardComputerOdds(board, &numFutureGames, &numFutureGamesNotLost, false);
			float odds = (float)numFutureGamesNotLost / numFutureGames;
			if (odds >= bestOdds)
			{
				bestOdds = odds;
				bestSpot = i;
			}
			board[i] = NONE;
		}
	}
	
	unsigned char row = bestSpot / BOARD_SIDE;
	unsigned char col = bestSpot % BOARD_SIDE;
	printf("Placing an X at %c%d...\n", row + 'A', col+1);
	
	board[row*BOARD_SIDE+col] = COMPUTER;
}

void ticTacToeBoardPlayerTurn(Board board)
{
	unsigned char row;
	unsigned char col;
	
	bool validLocation = false;
	do
	{
		printf("Please enter a postion such as B2.\n");
		scanf(" %c%c", &row, &col);
		
		row -= 'A';
		col -= '1';
		
		if (board[row*BOARD_SIDE+col] != NONE)
			printf("You can't play there!\n");
		else if (row > BOARD_SIDE-1 || col > BOARD_SIDE-1)
			printf("That is not a valid location!\n");
		else
			validLocation = true;
	}
	while (false == validLocation);
	
	board[row*BOARD_SIDE+col] = PLAYER;
}

unsigned char ticTacToeBoardWonRow(Board board)
{
	for (int i = 0; i < BOARD_SIDE; ++i) 
	{
		int count = 0;
		for (int j = 0; j < BOARD_SIDE; ++j) 
		{
			if (PLAYER == board[i*BOARD_SIDE+j])
				++count;
			else if (COMPUTER == board[i*BOARD_SIDE+j])
				--count;
		}
		
		if (BOARD_SIDE == count) return PLAYER;
		else if (-BOARD_SIDE == count) return COMPUTER;
	}
	return NONE;
}

unsigned char ticTacToeBoardWonColumn(Board board)
{
	for (int i = 0; i < BOARD_SIDE; ++i) 
	{
		int count = 0;
		for (int j = 0; j < BOARD_SIDE; ++j) 
		{
			if (PLAYER == board[j*BOARD_SIDE+i])
				++count;
			else if (COMPUTER == board[j*BOARD_SIDE+i])
				--count;
		}
		
		if (BOARD_SIDE == count) return PLAYER;
		else if (-BOARD_SIDE == count) return COMPUTER;
	}
	return NONE;
}

unsigned char ticTacToeBoardWonDiagonal(Board board)
{
	int count = 0;
	for (int i = 0; i < BOARD_SIDE; ++i) 
	{
		if (PLAYER == board[i*BOARD_SIDE+i])
			++count;
		else if (COMPUTER == board[i*BOARD_SIDE+i])
			--count;
		
		if (BOARD_SIDE == count) return PLAYER;
		else if (-BOARD_SIDE == count) return COMPUTER;
	}
	
	count = 0;
	for (int i = 0; i < BOARD_SIDE; ++i) 
	{
		if (PLAYER == board[(i+1)*(BOARD_SIDE-1)])
			++count;
		else if (COMPUTER == board[(i+1)*(BOARD_SIDE-1)])
			--count;
		
		if (BOARD_SIDE == count) return PLAYER;
		else if (-BOARD_SIDE == count) return COMPUTER;
	}
	
	return NONE;
}

unsigned char ticTacToeBoardWinner(Board board)
{
	unsigned char r = ticTacToeBoardWonRow(board);
	if (NONE != r)
		return r;
	
	unsigned char c = ticTacToeBoardWonColumn(board);
	if (NONE != c)
		return c;
	
	unsigned char d = ticTacToeBoardWonDiagonal(board);
	if (NONE != d)
		return d;
	
	if (true == ticTacToeBoardFull(board))
		return TIE;
	
	return NONE;
}

int main (int argc, const char * argv[])
{
	Board board;
	ticTacToeBoardInitialize(board);
	// char board[9] 
	// ticTacToeBoardInitialize(board);
	
	bool playerTurn = true;
	
	while (NONE == ticTacToeBoardWinner(board)) 
	{
		ticTacToeBoardPrint(board);
		
		if (true == playerTurn)
			ticTacToeBoardPlayerTurn(board);
		else
			ticTacToeBoardComputerTurn(board);
		
		playerTurn = !playerTurn;
	}
	
	ticTacToeBoardPrint(board);
	switch (ticTacToeBoardWinner(board)) 
	{
		case PLAYER:
			printf("You won!\n") ;
			break;
		case COMPUTER:
			printf("I won!\n");
			break;
		default: // TIE
			printf("Cats game!\n");
			break;
	}
	
    return 0;
}

