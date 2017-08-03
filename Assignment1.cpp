/*****************************************
* Ryan Stratton
* CS 162
* Homework #1
*****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10
#define GRID 20

//Sets up the function that fills in the values for the first two arrays.
void ArrayRandomSet(int ArrayOne[SIZE][SIZE], int ArrayTwo[SIZE][SIZE]);

//Sets up the function that matrix multiplies the first two arrays and sets the output as the third array.
void MatrixMultiply(int ArrayOne[SIZE][SIZE], int ArrayTwo[SIZE][SIZE], int ArrayThree[SIZE][SIZE]);

//Sets up the function that adds any multiple of 3 or 5 below 1000.
int MultipleOfThreeOrFive();

//Sets up the function that figures out the largest sum of 4 adjacent numbers and then outputs that sum.
int GridMax(int ArrayFour[GRID][GRID]);

int main (int argc, char **argv){

	//Sets up variables.

	int Total = 0;
	int Max_value = 0;

	int ArrayOne[SIZE][SIZE];
	int ArrayTwo[SIZE][SIZE];
	int ArrayThree[SIZE][SIZE];
	int ArrayFour[GRID][GRID] = {8, 2, 22, 97, 38, 15, 0, 40, 0, 75, 4, 5, 7, 78, 52, 12, 50, 77, 91, 8,
								 49, 49, 99, 40, 17, 81, 18, 57, 60, 87, 17, 40, 98, 43, 69, 48, 4, 56, 62, 0,
								 81, 49, 31, 73, 55, 79, 14, 29, 93, 71, 40, 67, 53, 88, 30, 3, 49, 13, 36, 65,
								 52, 70, 95, 23, 4, 60, 11, 42, 69, 24, 68, 56, 1, 32, 56, 71, 37, 2, 36, 91,
								 22, 31, 16, 71, 51, 67, 63, 89, 41, 92, 36, 54, 22, 40, 40, 28, 66, 33, 13, 80,
								 24, 47, 32, 60, 99, 3, 45, 2, 44, 75, 33, 53, 78, 36, 84, 20, 35, 17, 12, 50,
								 32, 98, 81, 28, 64, 23, 67, 10, 26, 38, 40, 67, 59, 54, 70, 66, 18, 38, 64, 70,
								 67, 26, 20, 68, 2, 62, 12, 20, 95, 63, 94, 39, 63, 8, 40, 91, 66, 49, 94, 21,
								 24, 55, 58, 5, 66, 73, 99, 26, 97, 17, 78, 78, 96, 83, 14, 88, 34, 89, 63, 72,
								 21, 36, 23, 9, 75, 0, 76, 44, 20, 45, 35, 14, 0, 61, 33, 97, 34, 31, 33, 95,
								 78, 17, 53, 28, 22, 75, 31, 67, 15, 94, 3, 80, 4, 62, 16, 14, 9, 53, 56, 92,
								 16, 39, 5, 42, 96, 35, 31, 47, 55, 58, 88, 24, 0, 17, 54, 24, 36, 29, 85, 57,
								 86, 56, 0, 48, 35, 71, 89, 7, 5, 44, 44, 37, 44, 60, 21, 58, 51, 54, 17, 58,
								 19, 80, 81, 68, 5, 94, 47, 69, 28, 73, 92, 13, 86, 52, 17, 77, 4, 89, 55, 40,
								 4, 52, 8, 83, 97, 35, 99, 16, 7, 97, 57, 32, 16, 26, 26, 79, 33, 27, 98, 66,
								 88, 36, 68, 87, 57, 62, 20, 72, 3, 46, 33, 67, 46, 55, 12, 32, 63, 93, 53, 69,
								 4, 42, 16, 73, 38, 25, 39, 11, 24, 94, 72, 18, 8, 46, 29, 32, 40, 62, 76, 36,
								 20, 69, 36, 41, 72, 30, 23, 88, 34, 62, 99, 69, 82, 67, 59, 85, 74, 4, 36, 16,
								 20, 73, 35, 29, 78, 31, 90, 1, 74, 31, 49, 71, 48, 86, 81, 16, 23, 57, 5, 54,
								 1, 70, 54, 71, 83, 51, 54, 69, 16, 92, 33, 48, 61, 43, 52, 1, 89, 19, 67, 48};

	srand(time(NULL)); //seeds the random number generator. 

	//calls the various functions.
	ArrayRandomSet(ArrayOne, ArrayTwo);
	MatrixMultiply(ArrayOne, ArrayTwo, ArrayThree);
    Total = MultipleOfThreeOrFive();
	Max_value = GridMax(ArrayFour);

	printf("The sum of the numbers divisible by 3 or 5 below 1000 is: %d \n", Total);

	printf("The maximum value of 4 adjacent numbers is: %d \n", Max_value);
	
}

//The function that sets the random values.
void ArrayRandomSet(int ArrayOne[SIZE][SIZE], int ArrayTwo[SIZE][SIZE]){

	int i;
	int j;

	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){

			ArrayOne[i][j] = rand()%50; //sets the random numbers.
			ArrayTwo[i][j] = rand()%50;
		}
	}
}

//Multiples the two matrixes together and sets the output equal to the third.
void MatrixMultiply(int ArrayOne[SIZE][SIZE], int ArrayTwo[SIZE][SIZE], int ArrayThree[SIZE][SIZE]){

	int i;
	int j;

	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){

			ArrayThree[i][j] = (ArrayOne[i][j] * ArrayTwo[j][i]);
		}
	}
}

//Finds the sum of the numbers that are divisible by 3 or 5 that are below 1000.
int MultipleOfThreeOrFive(){

	int i;
	int j;

	int number = 1;
	int Total = 0;

	for(i = 1; i < 1000; i++){

		if( (number%3 == 0 || number%5 == 0)){
			Total += number;
		}
		number++;
	}
	
	return Total;
}

//Determines the largest of 4 adjacent numbers multiplied together in the matrix.
int GridMax(int ArrayFour[GRID][GRID]){

	int i;
	int j;

	int Value_H = 0;
	int Value_V = 0;
	int Value_D1 = 0;
	int Value_D2 = 0;
	int max = 0;

	for(i = 0; i < 20; i++){
		for(j = 0; j < 20; j++){
			
			//If statements that check each of the 4 possible directions
			//horizontal, vertical and both diagonals.
			if (j < 17){
				Value_H = (ArrayFour[i][j]*ArrayFour[i][j+1]*ArrayFour[i][j+2]*ArrayFour[i][j+3]);
			}

			if (i < 17){
				Value_V = (ArrayFour[i][j]*ArrayFour[i+1][j]*ArrayFour[i+2][j]*ArrayFour[i+3][j]);
			}

			if ((i < 17) && (j < 17)){
				Value_D1 = (ArrayFour[i][j]*ArrayFour[i+1][j+1]*ArrayFour[i+3][j+3]*ArrayFour[i+3][j+3]);
			}

			if ((i < 17) &&  (j > 2)){
				Value_D2 = (ArrayFour[i][j]*ArrayFour[i+1][j-1]*ArrayFour[i+2][j-2]*ArrayFour[i+3][j-3]);
			}

			//If statements that assigns max to the highest output value.
			if (Value_H > max){
				max = Value_H;
			}

			if (Value_V > max){
				max = Value_V;
			}

			if (Value_D1 > max){
				max = Value_D1;
			}

			if (Value_D2 > max){
				max = Value_D2;
			}
		}
	}
	 return max;
}
