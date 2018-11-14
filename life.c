/* Game of Life - Maria Medina Martinez
 */

#include <stdio.h>
#include <stdlib.h>
#include "twoD.h"

void endTheGame(int termCond, int nrows, int ncols, char **a, int gensReached);
void blankArray(char ** a, int nrows, int ncolumns);
int playOne(unsigned int nr, unsigned int nc, char **old, char **new);
int checkNeighbors(unsigned int nr, unsigned int nc, int r, int c, char **a);
int checkArrays(unsigned int nr, unsigned int nc, char **a, char **b);
int checkDeathTermination(int nr, int nc, char **a);
void printArray(int nr, int nc, char **a);

/** Main function.
 * @param argc Number of words on the command line.
 * @param argv Array of pointers to character strings containing the
 *    words on the command line.
 * @return 0 if success, 1 if invalid command line or unable to open file.
 *
 */
int main(int argc, char **argv) {
	printf("Game of Life\n");

	// input command line arguments
	char *inputFileName; // Name of file containing initial grid
	FILE *input; // Stream descriptor for file containing initial grid
	int nrows; // Number of rows in the grid
	int ncolumns; // Number of columns in the grid
	int gens; // Number of generations to produce
	int doPrint = 0; // 1 if user wants to print each generation, 0 if not
	int doPause = 0; // 1 if user wants to pause after each generation, 0 if not

	// 2D arrays used
	char **gridA; // A 2D array to hold the pattern
	char **gridB; // A 2D array to hold a generation
	char **gridC; // A 2D array to hold another generation
	char **temp; // A 2D array to hold the pattern before it's centered

	// used when centering the array
	int maxWidth = 0; // records max number of columns in input file (used to center pattern)
	int tempMaxWidth = 0; // temp max width being compared (used to replace maxWidth if a new max is found)
	int maxHeight = 0; // keeps track of the number of rows in the input file

	// Command line arguments

	// See if there are the right number of arguments on the command line
	if ((argc < 5) || (argc > 7)) {
		// If not, tell the user what to enter.
		printf("Usage:\n");
		printf("  ./life rows columns generations inputFile [print] [pause]\n");
		return EXIT_FAILURE;
	}
	/* Save the command-line arguments.*/
	nrows = atoi(argv[1]); // Convert from character string to integer.
	ncolumns = atoi(argv[2]);
	gens = atoi(argv[3]);
	inputFileName = argv[4];

	// checks to see that input numbers are positive
	if (nrows <= 0 || ncolumns < 0 || gens < 0) {
		printf("Invalid input.\n");
		printf("Only positive arguments permitted.");
		return EXIT_FAILURE;
	}

	// Saves the optional arguments if they exist
	if (argc > 5) {
		switch (argc) {
		case 6:
			// checks argument for printing
			if (argv[5][0] == 'y')
				doPrint = 1;
			else if (argv[5][0] == 'n')
				doPrint = 0;
			else {
				printf(
						"Improper input for [print]. Accepts either 'y' or 'n'\n");
				printf("Program will run, but will not print.");
				doPrint = 0;
			}
			break;
		case 7:
			// checks argument for printing
			if (argv[5][0] == 'y')
				doPrint = 1;
			else if (argv[5][0] == 'n')
				doPrint = 0;
			else {
				printf("Improper input for [print]. Accepts either 'y' or 'n'");
				printf("Program will run, but will not print.");
				doPrint = 0;
			}
			// checks argument for pausing
			if (argv[6][0] == 'y')
				doPause = 1;
			else if (argv[6][0] == 'n')
				doPause = 0;
			else {
				printf("Improper input for [pause]. Accepts either 'y' or 'n'");
				printf("Program will run, but will not pause.");
				doPause = 0;
			}
			break;
		default:
			break;
		}
	}

	// Allocating space for arrays & initializing necessary arrays

	/* Allocate four arrays to hold the gen patterns*/
	gridA = make2Dchar(nrows, ncolumns);
	gridB = make2Dchar(nrows, ncolumns);
	gridC = make2Dchar(nrows, ncolumns);
	temp = make2Dchar(nrows, ncolumns);

	if (!gridA || !gridB || !gridC || !temp) { // If null pointer returned from any array creation, the program exits
		printf("Unable to allocate memory with given array dimensions.");
		return EXIT_FAILURE;
	}

	// set temp array to be filled with blank spaces before beginning
	blankArray(temp, nrows, ncolumns);

	// Reading the file

	/* Open input file to read in values*/
	input = fopen(inputFileName, "r");
	if (!input) {
		printf("Unable to open input file: %s\n", inputFileName);
		return EXIT_FAILURE;
	}

	char cs = fgetc(input); // holds each char as it's read in from the file
	int cr = 0, cc = 0; // keeps track of the current row and column in the temp array

	// reads in the file one character at a time
	while (cs != EOF) {// while it can still read a character
		if (cs == '\n') {
			maxHeight++; // keeps track of # of rows in the file
			cr++; // increases to next row for array
			cc = 0; // column reset to 0
			if (tempMaxWidth > maxWidth) // checks to see if the length of this line was longer than the previous one
				maxWidth = tempMaxWidth;
			tempMaxWidth = 0;
		} else if (cs == 'x') {
			temp[cr][cc] = 'x';
			cc++; // increases column
			tempMaxWidth++;
		} else if (cs == 'o') {
			temp[cr][cc] = ' ';
			cc++; // increases column
			tempMaxWidth++;
		}
		cs = fgetc(input);
	}
	fclose(input);

	// variables to center the array
	int widthDiff = ncolumns - maxWidth; // the difference between the input columns and the ones given by file
	int heightDiff = nrows - maxHeight; // the difference between the input rows and the ones given by file
	int r; // current row of array
	int c; // current column of array
	int tempR = 0; // counter for the temp array
	int tempC = 0; // counter for the temp array

	/* Fills in the initial array with blank spaces until
	 * the row is reached where it must begin to insert
	 * the input from the file.
	 */
	for (r = 0; r < heightDiff / 2; r++) {
		for (c = 0; c < ncolumns; c++) {
			gridA[r][c] = ' ';
		}
	}
	/**
	 * Continues filling in the rows till the end of the array,
	 * but now taking into account the input from the file
	 * stored in a temp array.
	 */
	for (; r < nrows; r++) {
		// Fills the spaces before the input with blank spaces
		for (c = 0; c < widthDiff / 2; c++) {
			gridA[r][c] = ' ';
		}
		// Fills the array with the input
		for (; c < widthDiff + maxWidth; c++) {
			gridA[r][c] = temp[tempR][tempC];
			tempC++;
		}
		// fills rest of the array with blank spaces after input has been added
		for (; c < ncolumns; c++) {
			gridA[r][c] = ' ';
		}
		tempR++; // increases temp's row counter by one to read the next line
		tempC = 0; // resets the temp's column to beginning
	}

	// Begin playing the game

	char **p = gridA; // uses this pointer within for loop to change array being used
	char **q = gridB; // uses this pointer within for loop to hold the new generation being made
	char **spare = gridC; // uses pointer to hold the address of the spare array
	char **tempPlay; // holds one extra array as the gens loop through
	int terminatingCond = 0; // holds reason why the program terminated; 0 = gens all reached, 1 = same gens, 2 = death reached
	int totalGensReached = 0; // keeps track of the gens done before termination reached

	/* Plays the game/loops until the desired number of gens is reached (unless terminated earlier).
	 * The game is not over as long as the loop is running.
	 */
	for (int i = 0; i < gens; i++, totalGensReached++) {
		if (doPrint) // if user wants to print every gen
		{
			printf("\n=================Gen %d==================\n", i);
			printArray(nrows, ncolumns, p); // print the array holding the current generation

			if (playOne(nrows, ncolumns, p, q)
					&& checkArrays(nrows, ncolumns, q, spare)) { // checks to see that a steady state has not been reached
				if (checkDeathTermination(nrows, ncolumns, q)) { // checks to see that death hasn't been reached
					if (doPause) { // if they decide to pause between gens
						getchar(); // wait for a keystroke before going into the next gen
					}
				} else {
					terminatingCond = 2;
					i = gens; // forces loop to end bc organisms are dead
				}
			} else {
				terminatingCond = 1;
				i = gens; // forces loop to end bc steady state was reached
			}
		} else // if the gens are not being printed
		{
			// if the arrays are identical then the for loop is stopped
			if (!playOne(nrows, ncolumns, p, q)
					&& !checkArrays(nrows, ncolumns, q, spare)) {
				terminatingCond = 1;
				i = gens; // forces for loop to exit because a terminating condition was reached
			} else if (!checkDeathTermination(nrows, ncolumns, q)) { // if all the organisms are dead
				terminatingCond = 2;
				i = gens; // forces for loop to exit because a terminating condition was reached
			}
		}
		// shifts the pointers of the arrays for the next gen round
		tempPlay = p; // sets tempPlay to the initial gen
		p = q; // sets p to the newly made array
		q = spare; // q will now hold the next array to hold a new gen
		spare = tempPlay; // spare is set to point to the tempPlay array
	}
	// calls endTheGame to output a final message to the user
	endTheGame(terminatingCond, nrows, ncolumns, p, totalGensReached);

	// free the memory  used
	free(gridA);
	free(gridB);
	free(gridC);
	free(temp);

	return EXIT_SUCCESS;
}
/**
 * Outputs a termination message and prints out the final state of the array.
 *
 * @param termCond The terminating condition of the game
 * @param nrows The number of rows of each array
 * @param ncols The number of columns of each array
 * @param a The array holding the final generation
 * @param gensReached The number of generations achieved before termination
 */
void endTheGame(int termCond, int nrows, int ncols, char **a, int gensReached) {
	// switches to print the final array configuration and message specifying game's end
	printf("\n=================Final State==================\n");
	switch (termCond) {
	case 0: // all of the gens were printed successfully
		printArray(nrows, ncols, a);
		printf(
				"\nA total of %d generations were printed.\nTermination due to desired generations being reached.",
				gensReached);
		break;
	case 1: // a steady state was reached and game was ended early
		printArray(nrows, ncols, a);
		printf(
				"\nA total of %d generations were printed.\nTermination due to generations reaching a steady state.",
				gensReached);
		break;
	case 2: // all organisms died
		printArray(nrows, ncols, a);
		printf(
				"\nA total of %d generations were printed.\nTermination due to all organisms dying.",
				gensReached);
		break;
	}
}
/**
 *  Fills the given array with blank spaces.
 *
 *  @param **a The array that is being filled with blanks
 *  @param nrows The number of rows in the array
 *  @param ncolumns The number of columns in the array
 */
void blankArray(char ** a, int nrows, int ncolumns) {
	/*
	 * Loops through entire array and sets each element to be
	 * a blank space.
	 */
	for (int r = 0; r < nrows; r++) {
		for (int col = 0; col < ncolumns; col++) {
			a[r][col] = ' ';
		}
	}
}
/**
 * Calculates one generation of the game, changing the status of the
 * new input array based on the old.
 *
 * @param nr The number of rows in the arrays
 * @param nc The number of columns in the array
 * @param old The 2D array used to update the new one
 * @param new The 2D array used to hold the updated generation
 * @return zero value if the a termination condition is reached, non-zero if not reached
 */
int playOne(unsigned int nr, unsigned int nc, char **old, char **new) {
	int numNeighbors = 0; // keeps track of number of neighbors of the current element

	/**
	 * Loops through array indices and counts neighbors
	 * of each element in the old array. The same element's
	 * location in the new array is updated based on the
	 * number of neighbors of the element. This is determined
	 * in the switch until every index is looped through.
	 */
	for (int r = 0; r < nr; r++) {
		for (int c = 0; c < nc; c++) {
			numNeighbors = checkNeighbors(nr, nc, r, c, old); // checks to see how many neighbors the cell has

			switch (numNeighbors) {
			// organism dies from loneliness
			case 0:
			case 1:
				new[r][c] = ' ';
				break;
			// the cell survives if it's already alive so nothing is done
			case 2:
				new[r][c] = old[r][c];
				break;
			// new cell is made if 3 neighbors
			case 3:
				new[r][c] = 'x';
				break;
			// cell dies from overcrowding
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				new[r][c] = ' ';
				break;
			}
		}
	}

	// check  to see if the arrays are the same; 0 if they're same
	return checkArrays(nr, nc, old, new);
}
/**
 * Counts the number of neighbors around a cell if they are within the grid boundaries
 *
 * @param nr The total number of rows in the grid
 * @param nc The total number of columns in the grid
 * @param r The row index of the specific cell being checked
 * @param c The column index of the specific cell being checked
 * @param a The 2D array where neighbors of the cell are being counted
 * @return The number of numbers counted for the specified cell
 */
int checkNeighbors(unsigned int nr, unsigned int nc, int r, int c, char **a) {
	int cnt = 0; // keeps track of the neighbor count of an organism

	/* will only have occupied neighbors if r and c are within the boundaries,
	 * so checks boundaries first in each condition.
	 */

	// checks row of neighbors above element
	if (((r - 1) >= 0 && (c - 1) >= 0) && ((r - 1) < nr && (c - 1) < nc)
			&& a[r - 1][c - 1] == 'x')
		cnt++;
	if (((r - 1) >= 0 && c >= 0) && ((r - 1) < nr && c < nc)
			&& a[r - 1][c] == 'x')
		cnt++;
	if (((r - 1) >= 0 && (c + 1) >= 0) && ((r - 1) < nr && (c + 1) < nc)
			&& a[r - 1][c + 1] == 'x')
		cnt++;
	// checks middle left and right neighbors
	if ((r >= 0 && (c - 1) >= 0) && (r < nr && (c - 1) < nc)
			&& a[r][c - 1] == 'x')
		cnt++;
	if ((r >= 0 && (c + 1) >= 0) && (r < nr && (c + 1) < nc)
			&& a[r][c + 1] == 'x')
		cnt++;
	// checks row of neighbors below element
	if (((r + 1) >= 0 && (c - 1) >= 0) && ((r + 1) < nr && (c - 1) < nc)
			&& a[r + 1][c - 1] == 'x')
		cnt++;
	if (((r + 1) >= 0 && c >= 0) && ((r + 1) < nr && c < nc)
			&& a[r + 1][c] == 'x')
		cnt++;
	if (((r + 1) >= 0 && (c + 1) >= 0) && ((r + 1) < nr && (c + 1) < nc)
			&& a[r + 1][c + 1] == 'x')
		cnt++;
	return cnt; // the number of neighbors
}
/**
 * Compares two 2D arrays to see if they are the same
 *
 * @param a The first array being compared
 * @param b The second array being compared
 * @return 0 if arrays are the same or 1 if they are different
 */
int checkArrays(unsigned int nr, unsigned int nc, char **a, char **b) {
	/* Loops through both arrays and compares each cell. If
	 * the cells do not match, then the function returns because
	 * the arrays are not the same.
	 */
	for (int r = 0; r < nr; r++) {
		for (int c = 0; c < nc; c++) {
			// if a cell does not match, then 1 is returned as they are different
			if (a[r][c] != b[r][c])
				return 1;
		}
	}
	return 0;
}
/**
 * Checks whether all the organisms in the given array are dead
 *
 * @param nr The total number of rows in the array
 * @param nc The total number of columns in the array
 * @param a The array being checked for death state
 * @return A non-zero number if there is no termination, zero if it is terminated
 */
int checkDeathTermination(int nr, int nc, char **a) {
	/*
	 * Loops through entire array and checks to see whether
	 * any of the elements are equal to an x. If an x is found,
	 * there is still a living organism, so the function returns.
	 */
	for (int r = 0; r < nr; r++) {
		for (int c = 0; c < nc; c++) {
			if (a[r][c] == 'x')
				return 1; //means there is still an organism
		}
	}
	return 0; // all the organisms are confirmed dead in the array
}
/**
 * Print the given 2D array out
 *
 * @param a The array to be printed out
 * @param nr The total number of rows
 * @param nc The total number of columns
 */
void printArray(int nr, int nc, char **a) {
	/* loops through entirety of the passed in array
	to print out every character*/
	for (int r = 0; r < nr; r++) {
		for (int c = 0; c < nc; c++) {
			printf("%c", a[r][c]);
		}
		printf("\n");
	}
}
