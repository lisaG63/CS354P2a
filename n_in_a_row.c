///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission for CS 354 Fall 2020, Deb Deppeler
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        n_in_a_row.c
// This File:        n_in_a_row.c
// Other Files:      None.
// Semester:         CS 354 Fall 2020
//
// Author:           Weihang Guo
// Email:            wguo63@wisc.edu
// CS Login:         weihang
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          None.
//
// Online sources:   None.
//
////////////////////////////////////////////////////////////////////////////////
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
     
char *DELIM = ",";  // commas ',' are a common delimiter character for data strings
     
/* COMPLETED:       
 * Retrieves from the first line of the input file,
 * the size of the board (number of rows and columns).
 * 
 * fp: file pointer for input file
 * size: pointer to size
 */
void get_dimensions(FILE *fp, int *size) {      
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, fp) == -1) {
        printf("Error in reading the file.\n");
        exit(1);
    }

    char *token = NULL;
    token = strtok(line, DELIM);
    *size = atoi(token);
}       
 
/* Returns 1 if this column is crossed by X or O, 
 * 0 if this column is not crossed 
 * board: heap allocated 2D board
 * size: number of rows and columns
 * j: the column index
 */
int ifColumnCrossed(int **board, int size, int j) {
    int o = 0;//number of Os in this column
    int x = 0;//number of Xs in this column
    //count the numbers of Os and Xs in this column
    for (int i = 0; i < size; i ++) {
        if (* (* (board + i) + j) == 1)
            x ++;
        if (* (* (board + i) + j) == 2)
            o ++;
        if (* (* (board + i) + j) == 0)
            return 0;
            //if this column is not completely filled, return 0
    }
    if (x == size || o == size) {//when X or O cross out this column
        return 1;
    }
    return 0;//otherwise return 0
}
   
/* Returns 1 if this row is crossed by X or O,
 * 0 if this row is not crossed
 * board: heap allocated 2D board
 * size: number of rows and columns
 * i: the row index
 */
int ifRowCrossed(int **board, int size, int i) {
    int o = 0;//number of Os in this row
    int x = 0;//number of Xs in this row
    //count the numbers of Os and Xs in this row
    for (int j = 0; j < size; j ++) {
        if (* (* (board + i) + j) == 1)
            x ++;
        if (* (* (board + i) + j) == 2)
            o ++;
        if (* (* (board + i) + j) == 0)
            return 0;
    //if this row is not completely filled, return 0
    }
    if (x == size || o == size) {
        //when X or O cross out this row
        return 1;
    }
    return 0;//otherwise return 0
}


/* Returns 1 if X wins or O wins by crossing only one row, or there is no winner
 * otherwise return 0
 * board: heap allocated 2D board
 * size: number of rows and columns
 */
int rowCheck(int **board, int size) {
    int rowCrossed = 0;//the number of rows crossed
    //count the number of rows crossed
    for (int i = 0; i < size; i ++) {
        if (ifRowCrossed(board, size, i) == 1)
            rowCrossed ++;
    }
    if (rowCrossed > 1) 
        return 0;//parallel winning rows are invalid
    return 1;//0 or 1 row crossed is valid
}

/* Returns 1 if X wins or O wins by crossing only one column, or there is no winner
 * otherwise return 0
 * board: heap allocated 2D board
 * size: number of rows and columns
 */
int columnCheck(int **board, int size) {
    int columnCrossed = 0;//the number of columns crossed
    //count the number of columns crossed
    for (int j = 0; j < size; j ++) {
        if (ifColumnCrossed(board, size, j) == 1)
            columnCrossed ++;
    }
    if (columnCrossed > 1)
        return 0;//parallel winning rows are invalid
    return 1;//0 or 1 row crossed is valid
}

/* TODO:
 * Returns 1 if and only if the board is in a valid state.
 * Otherwise returns 0.
 *
 * board: heap allocated 2D board
 * size: number of rows and columns
 */
int n_in_a_row(int **board, int size) {
    //checks size: odd size
    if (size % 2 != 1)
        return 0;//invalid size
    //checks nubmers of Os and Xs: either the same number Xs as Os, or 1
    //more X than O
    int x = 0;//the number of Xs
    int o = 0;//the nubmer of Os
    //count the numbers of Xs and Os in this board
    for (int i = 0; i < size; i ++) {
        for (int j = 0; j < size; j ++) {
            if (* (* (board + i) + j) == 1)
                x ++;
            if (* (* (board + i) + j) == 2)
                o ++;
        }
    }
    if (!(o == x || x == (o + 1)))
        return 0;//invalid numbers of Xs and Os

    //checks winner: either no winner or one winner
    //checks the winning line: either one winning line (i.e., row, column,
    //or diagonal), or two winning lines that intersect on one mark
    if (rowCheck(board, size) == 0 || columnCheck(board, size) == 0)
        return 0;
    return 1;//otherwise valid
}
   
/* PARTIALLY COMPLETED:
 * This program prints Valid if the input file contains
 * a game board with either 1 or no winners; and where
 * there is at most 1 more X than O.
 * 
 * argc: CLA count
 * argv: CLA value
 */
int main(int argc, char *argv[]) {              
     
    //TODO: Check if number of command-line arguments is correct.
    if (argc != 2) {
        printf("Usage: ./n_in_a_row <input_filename>\n");
        exit(1);
    }
    //Open the file and check if it opened successfully.
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL) {
        printf("Can't open file for reading.\n");
        exit(1);
    }

    //Declare local variables.
    int size;

    //TODO: Call get_dimensions to retrieve the board size.
    get_dimensions(fp, &size);
    //TODO: Dynamically allocate a 2D array of dimensions retrieved above.
    int **board = malloc(sizeof(int *) * size);
    if (board == NULL) {//check malloc return value
        printf("Can't create the board.\n");
        exit(1);
    }
    //allocate each of the 1D arrays of board
    for (int r = 0; r < size; r ++) {
        *(board + r) = malloc(sizeof(int) * size);
        if ((board + r) == NULL) {//check malloc return value
            printf("Can't create the board.\n");
            exit(1);
        }
    }
    //Read the file line by line.
    //Tokenize each line wrt the delimiter character to store the values in your 2D array.
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    for (int i = 0; i < size; i++) {

        if (getline(&line, &len, fp) == -1) {
            printf("Error while reading the file.\n");
            exit(1);
        }

        token = strtok(line, DELIM);
        for (int j = 0; j < size; j++) {
            //TODO: Complete the line of code below
            //to initialize your 2D array.
            *(*(board + i) + j) = atoi(token);
            token = strtok(NULL, DELIM);
        }
    }

    //TODO: Call the function n_in_a_row and print the appropriate
    //output depending on the function's return value.
    if (n_in_a_row(board, size) == 1) {
        printf("valid\n");
    } else {
        printf("invalid\n");
    }

    //TODO: Free all dynamically allocated memory.
    for (int i = 0; i < size; i ++) {
        free(*(board + i));//free the 1D array
    }
    free(board);//free board
    board = NULL;
    //Close the file.
    if (fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    } 
     
    return 0;       
}       



                                        // FIN
