#include "nim.h"

#include <stdio.h>
#include <stdlib.h>


int* new_board(int board_size){
	// allocate memory 
	int* arr = (int*)malloc(board_size);

	// error if memory not allocated 
	if (arr == NULL){
		fprintf(stderr, "Error: malloc failed\n");
		exit(-1);
	}
	// return pointer to memory 
	return arr;
}

struct node* mk_nim_hash(int max_hash, int board_size, int* start_board){
	// allocate new nim hash 
	struct node* S = (struct node*)malloc(max_hash * sizeof(struct node));
	
	// error if memory not allocated 
	if (S == NULL){
		fprintf(stderr, "Error: malloc failed.\n");
		exit(-1);
	}
	
	// set all values 
	for (int i = 0; i < max_hash; i++){
		S[i].moves = -1;
		S[i].move = NULL;
		S[i].nimsum = -1;


		S[i].board = hash2board(board_size, start_board, i);
	}

	return S;
}

void free_board(int* board){
	// free board memory 
	free(board);
}

void free_nim_hash(int max_hash, struct node* nim_hash){
	// free every min hash's move and board memory 
	for (int i = 0; i < max_hash; i++){
		free(nim_hash[i].move);
		free(nim_hash[i].board);
	}
	// free the min hash memory 
	free(nim_hash);
}


int* board_from_argv(int board_size, char** argv){
	// create new board 
	int* board = new_board(board_size);

	// set the matches count in each row 
	for (int i = 0; i < board_size; i++){
		board[i] = atoi(argv[i]);
	}
	
	// return board 
	return board;
}

int* copy_board(int board_size, int* board){
	// create new board 
	int* board_copy = new_board(board_size);
	
	// copy the matches count in each row 
	for (int i = 0; i < board_size; i++){
		board_copy[i] = board[i];
	}
	// return copy board 
	return board_copy;
}

int game_over(int board_size, int* board){
	int match_count = 0;
	// count matches 
	for (int i = 0; i < board_size; i++){
		if (board[i] > 0){
			match_count += board[i];
		}
	}

	// return 1 if only one match left in board otherwise 0 
	if (match_count == 1){
		return 1;
	}
	else{
		return 0;
	}
}

void join_graph(struct node* nim_hash, int hash, int board_size, int* start_board) {
	// base condition 
	if (nim_hash[hash].moves != -1){
		return;
	}
	else{
		int index, i, *current_board = nim_hash[hash].board;

		nim_hash[hash].nimsum = compute_nimsum(board_size, current_board);
		nim_hash[hash].moves = 0;
		nim_hash[hash].move = (struct move*)malloc(sizeof(struct move));

		int matches;

		// all possible hashes 
		i = 0;
		while (i < board_size){
			struct move* moves = nim_hash[hash].move;
			int* new_board = copy_board(board_size, current_board);

			matches = new_board[i];
			while (new_board[i] > 0) {
				
				new_board[i] -= 1;
				index = nim_hash[hash].moves;
				nim_hash[hash].moves += 1;
				moves = realloc(moves, nim_hash[hash].moves * sizeof(struct move));

				struct move* current_move = &moves[index];
				current_move->row = i;
				current_move->matches = matches -  new_board[i];
				current_move->hash = board2hash(board_size, start_board, new_board);
			}
			nim_hash[hash].move = moves;
			i++;
		}
		
		// join every move in graph 
		struct move* temp_moves = nim_hash[hash].move;
		int j = 0;
		while (j < nim_hash[hash].moves){
			join_graph(nim_hash, temp_moves[j].hash, board_size, start_board);
			j++;
		}
	}
}

int compute_nimsum(int board_size, int* board){
	int i = 0, sum = 0, count = 0;;
	// compute sum 
	while (i < board_size){
		if(board[i] > 0){
			sum ^= board[i];
			count++;	
		}
		
		i++;
	}
	
	if(count == 0 )
		return !sum;
	else	
		return sum;
}

