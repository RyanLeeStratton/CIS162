/**************************
Ryan Stratton
Homework3, beer
CS162
10/30/2012
**************************/


#include <iostream>
#include <vector>
#include <string>

int main(int argc, char **argv){

std::vector <std::string> Ones_digits;

//vector for ones position name
Ones_digits.push_back (" nine");
Ones_digits.push_back (" eight");
Ones_digits.push_back (" seven");
Ones_digits.push_back (" six");
Ones_digits.push_back (" five");
Ones_digits.push_back (" four");
Ones_digits.push_back (" three");
Ones_digits.push_back (" two");
Ones_digits.push_back (" one");
Ones_digits.push_back ("");


std::vector <std::string> Tens_digits;

//vector for tens position name, minus 10-19
Tens_digits.push_back (" ninety");
Tens_digits.push_back (" eighty");
Tens_digits.push_back (" seventy");
Tens_digits.push_back (" sixty");
Tens_digits.push_back (" fifty");
Tens_digits.push_back (" forty");
Tens_digits.push_back (" thirty");
Tens_digits.push_back (" twenty");
Tens_digits.push_back ("teen");
Tens_digits.push_back ("");

std::vector <std::string> Teens_digits;

//vector for teen position name
Teens_digits.push_back (" nineteen");
Teens_digits.push_back (" eighteen");
Teens_digits.push_back (" seventeen");
Teens_digits.push_back (" sixteen");
Teens_digits.push_back (" fifteen");
Teens_digits.push_back (" fourteen");
Teens_digits.push_back (" thirteen");
Teens_digits.push_back (" twelve");
Teens_digits.push_back (" eleven");
Teens_digits.push_back (" ten");



//loops 100 times
for (int i = 0; i < 10; ++i){
	for (int j = 0; j < 10; ++j){
		
		//if there are zero bottles or less left 
		if((i >= 9) && (j >= 9)){
		}

		else{

			//if bottles left is in 10-19
			 if(i ==8){

				std::cout << Teens_digits.at(j) <<" bottles of beer on the wall," << std::endl;
				std::cout << Teens_digits.at(j) <<" bottles of beer," << std::endl;

			}

			 // if one bottle of beer left
			 else if(i == 9 && j == 8){

				std::cout << Tens_digits.at(i) << Ones_digits.at(j) <<" bottle of beer on the wall," << std::endl;
				std::cout << Tens_digits.at(i) << Ones_digits.at(j) <<" bottle of beer," << std::endl;
			 }
		
			//if more than one bottle of beer is left
			else if(!(i == 9 && j == 8)){

				std::cout << Tens_digits.at(i) << Ones_digits.at(j) <<" bottles of beer on the wall," << std::endl;
				std::cout << Tens_digits.at(i) << Ones_digits.at(j) <<" bottles of beer," << std::endl;
			
			}

			std::cout << " take one down, pass it around," << std::endl;


			//zero bottles left
			if(i >= 9 && j >= 8){

				std::cout << " zero bottles of beer on the wall." << std::endl;
			}

			//nineteen bottles left
			if(i == 7 && j == 9){

				std::cout << Teens_digits.at(j-9) << " bottles of beer on the wall." << std::endl;
			}

			//if 9,29,39,49 ect bottles left, excludes 19
			if((j == 9 && (i < 9)) && !(i == 7 && j == 9)){

				std::cout << Tens_digits.at(i+1) << Ones_digits.at(j-9) << " bottles of beer on the wall." << std::endl;
			}

			//if 10-18 bottles left
			else if(i == 8 && j < 9){

				std::cout << Teens_digits.at(j+1) << " bottles of beer on the wall." << std::endl;
			}

			//one bottle left
			else if(i == 9 && j == 7){

				std::cout << Tens_digits.at(i) << Ones_digits.at(j+1) << " bottle of beer on the wall." << std::endl;
			}

			//more than 1 bottle of beer left
			else if((i <= 9 && j < 9) && !(i == 9 && j == 8) && !(i == 9 && j == 7)){

				std::cout << Tens_digits.at(i) << Ones_digits.at(j+1) << " bottles of beer on the wall." << std::endl;
			
			}

			std::cout << "" << std::endl;
		}

		


		}
	}	
}
