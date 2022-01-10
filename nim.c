#include "nim.h"

#include <stdio.h>
#include <stdlib.h>



int* new_board( int board_size ){
    int* arrBoard;
    arrBoard = malloc(sizeof(int)* board_size);

    //GIVE ERROR

    return arrBoard;

}

struct node *mk_nim_hash( int max_hash, int board_size, int*start_board ){
    struct node * structNode;
    structNode = malloc(sizeof(struct node) * max_hash);

    for(int i = 0 ; i < max_hash ; i++){

        structNode[i].moves = -1;
        structNode[i].move = NULL;
        structNode[i].nimsum = -1;
        structNode[i].board = hash2board(board_size, start_board ,i);
    }

    return structNode;
}


void free_board( int*board ){
    free(board);
}

void free_nim_hash( int max_hash, struct node *nim_hash ){
    for (int i = 0; i < max_hash; i++){
		free(nim_hash[i].move);
		free(nim_hash[i].board);
	}
	free(nim_hash);

}

int*board_from_argv( int board_size, char**argv ){
    int * new_board;

    new_board = malloc(sizeof(int)* board_size);

    for(int i = 0 ; i < board_size ; i++){
        new_board[i] = atoi(argv[i]);

    }

    return new_board;

}

int * copy_board( int board_size, int*board ){
    int * new_board;

    new_board = malloc(sizeof(int)* board_size);

    for(int i = 0 ; i < board_size ; i++){
        new_board[i] = board[i];

    }

    return new_board;

}


int game_over( int board_size, int*board ){
    int count = 0;
    for(int i = 0 ; i < board_size ; i++){

        count = count + board[i];
    }

    if(count == 1){
        return 1;
    }
    else{
        return 0;
    }

}


void join_graph( struct node *nim_hash, int hash, int board_size,int*start_board ){
    if( nim_hash[hash].moves != -1){
        return;
    }
    else{
        int *curr_board = nim_hash[hash].board;

        int moves= 0;
        for(int i = 0; i < board_size ; i++){
            moves = moves + curr_board[i];
        }
        nim_hash[hash].moves = moves;

        nim_hash[hash].nimsum = compute_nimsum(board_size, nim_hash[hash].board);

        nim_hash[hash].move = malloc(sizeof(struct move)*(moves));

        int moveIndex = 0;
        for(int i = 0; i < board_size ; i++){
            for(int j = 1; j <= curr_board[i] ; j++){
                nim_hash[hash].move[moveIndex].row = i;
                nim_hash[hash].move[moveIndex].matches = j;
                moveIndex++;
            }
        }

        int * newBoard;
        newBoard = malloc(board_size*sizeof(int));
        //newBoard = copy_board(board_size,start_board);
        //nim_hash[hash].board = start_board;
    
        moveIndex = 0;
        for(int i = 0; i < board_size ; i++){
            for(int j = 1; j <= curr_board[i]; j++){
                newBoard = copy_board(board_size,curr_board);
                
                newBoard[i] = curr_board[i] - j;

                nim_hash[hash].move[moveIndex].hash = board2hash(board_size,start_board,newBoard);
                join_graph(nim_hash,nim_hash[hash].move[moveIndex].hash ,board_size,start_board);
                moveIndex++;
                

            }
        }
    }
}


int compute_nimsum( int board_size, int *board ){
    int xor = board[0];
    int count = 0;

    if(board[0] >= 1){
        count++;
    }


    for(int i = 1; i < board_size ; i++){
        
            xor = xor^board[i];
            if(board[i] >= 1){
                count++;
            }
    }

    if(count <= 1){
        return ~xor;
    }


    return xor;


}
