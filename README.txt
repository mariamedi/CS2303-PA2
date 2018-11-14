Programming Assignment 2 - Maria Medina Martinez
------------------------------------------------------------------------------
Purpose of assignment: 

To simulate John Conway's Game of Life. The program must take in the user's command line arguments
and play the game with a grid of their desired dimensions and for their desired number of generations. The cells of the grid either survive or die from one generation to the next. New cells
can also be born. 
------------------------------------------------------------------------------
Description of program:

My program is split up in several different functions:

void endTheGame(int termCond, int nrows, int ncols, char **a, int gensReached);
void blankArray(char ** a, int nrows, int ncolumns);
int playOne(unsigned int nr, unsigned int nc, char **old, char **new);
int checkNeighbors(unsigned int nr, unsigned int nc, int r, int c, char **a);
int checkArrays(unsigned int nr, unsigned int nc, char **a, char **b);
int checkDeathTermination(int nr, int nc, char **a);
void printArray(int nr, int nc, char **a);

The main function of my program is responsible for the following:
	1) Reading and recording command line arguments
	2) Rejecting incorrect command line arguments 
	3) Opening and reading the input file
	4) Recording the values of the input file into a temp array
	5) Centering the input values into the first official array (gridA) 
	6) Playing the game for the input number of generations 
	7) Exiting once a terminating condition has been met

The main accomplishes the above actions with the help of the listed functions above.
The functions playOne(), checkArrays(), and checkDeathTermination() are all involved with checking
if a termination condition has been reached. checkArrays() sees if two arrays have reacehd a steady 
state and is utilized both in the main() and playOne(). checkDeathTermination() is used in main()
to check of all the organisms in an array have been wiped. playOne() is called from main every time 
a new generation is being calculated and it subsequently calls checkNeighbors() to decide what the 
new generation will look like. checkNeighbors() goes through each element in its given array and 
counts how many organisms neighbor it. playOne() decides if an organism will live or not based on
the neighbors it gets back from checkNeighbors(). blankArray() is only used within the main to
initialize the array that will temporarily hold the input from the given file. printArray() is 
used in both main() and endTheGame() when the user either wants the array to be printed for each
generation, or when endTheGame wants to output the final state of the game.
------------------------------------------------------------------------------
How to run the program:

The program is run by the following:
	./life rows columns generations inputFile [print] [pause]

rows, columns, and generations are strictly positive and the program exits if negative input is given.
[print] and [pause] accept 'y' or 'n', and will default to 'n' for both if improper input is given.

------------------------------------------------------------------------------
Loop invariants:

Invariants are listed in the order they appear throughout the program.

1) main - while loop to read file
	This while loop continues until the end of the given file is reached, and no more characters
	can be read by fgetc. Each character fgetc retrieves is tested to see if it is equal to 
	'\n', 'o', or 'x'. If an 'x' or an 'o' is read, the temp array holding the file's input is 
	updated to reflect the new character retrieved. On each iteration of the while loop, either
	a new character is added to temp, or the variables holding the current location of the temp
	index being looked at are changed to reflect the start of a new row in the temp array. The 
	values are added to the temp array starting in the upper left hand corner, and are added to
	to reflect their positions in the input file.

2) main - for loop filling part of gridA
	This for loop goes through each index of gridA up to where the row reaches the heightDiff /2
	calculated to center the temp array's values into gridA. It fills all the indices up to this
	point with ' '.

3) main - for loop filling rest of gridA
	Once the point has been reached by the row counter of the previous loop where the input
	should begin to be inserted into the array, the outer for loop changes to loop through the 
	rest of the rows in the array. It is then followed by three for loops. In case the input 
	files has less columns than those specified by the user, this first loop fills the first 
	widthDiff / 2 indices with ' ' to pad the left hand side of the input with the right amount
	of balnk spaces. Once this for loop terminates, the next one uses the previous column
	index, c, to continue filling in the current row of the array, but with the appropriate
 	values from the temp array. The correct values are acquired from temp array by keeping
	separate counter variables, which change according to the changes in the rows and columns
	in the for loop. The third for loop fills in the rest of the row's indices after the input
	is read in from each corresponding row of the temp array. These spaces are filled with ' '.

4) main - for loop to play for the given number of generations
	This loop runs for the input number of generations if the game does not reach a termination
	condition. If a termination condition is reached before the desired gens are run, the for
	loop terminates and exits to display a final message. For each iteration, one more
	generation is generated, and it is checked to see if it's reached a terminated condition.

5) blankArray - for loop 
	This nested for loop iterates through the given array and initializes each one of itss
	indices to a ' '. This is called from the main method. It is used to prefill an array from
	main.

6) playOne - nested for loop
	This nested for loop iterates through each of the given array's indices. This is done so
	that the neighbors of each cell in the array are counted. This number is then used to update
	the new array passed in to reflect the status of the next generation. As each index of 
	the old array is checked, the index of the new array is updated. 

7) checkArrays - newsted for loop
	This nested for loop also iterates through every index of the passed in array. This is done
	to compare whether the value of each index in both arrays are the same. If a value is found
	to be different in either array, the function returns as the arrays are found to be 
	different.

8) checkDeathTermination - nested for loop
	This nested for loop iterates through the entire given array and checks whether there is 
	a living organism in any of the indices. If an organism is found, the function returns as 
	death has not been achieved. 

9) printArray - nested for loop
	This nested for loop iterates through the given array and prints every single value stored
	within it. This is done to show the status of the given array. 
