/**********************************
Ryan Stratton
Homework3, sieve
CS162
10/30/2012
**********************************/

#include <iostream>
#include <vector>
#include <string>

int main(int argc, char **argv){

	//variables for computing the prime numbers
	int maximum_value = 0;
	double square_root = 0;
	int integer_root = 0;
	int primes_length = 0;

	//vector for holding a list of numbers
	std::vector <int> numbers;

	//vector for holding prime numbers
	std::vector <int> prime_values;

	//gets maximum/last number in the list
	std::cout << "Input a maximum value \"n\" for a list of integers." << std::endl;
	std::cout << "All prime numbers in the list will be determined." << std::endl;
	std::cout << "n:   ";

	std::cin >> maximum_value;
	std::cout << std::endl;
	
	//sets guess number for sqaure root function 
	square_root = maximum_value/2;

	//computes sqaure root using babylonian method
	for(int j = 0; j < 25; ++j){

		square_root = (0.5*(square_root + (maximum_value/square_root)));
	}

	//the square root
	integer_root = (int) square_root;


	//fills the list of numbers
	for(int i = 0; i < maximum_value; ++i){
		
		numbers.push_back(i+1);
	}



	//computes prime numbers in the list
	for(int k = 2; k <= square_root; ++k){
		for(int m = 1; m < maximum_value; ++m){

			double k_replace;
			double number_replace;
			double value_2 = 0;
			double value = 0;
		
			//turns int value of k into double
			k_replace = k;

			//turns numbers int value into double
			number_replace = numbers.at(m);

			//computes int value checking for composite 
			value = ((numbers.at(m))/k);

			//computes double value checking for composite
			value_2 = (number_replace/k_replace);
	

				//if both values match, number is a composite, changes to a 1.
				if((value == value_2) && ((value_2 != 1))){

					numbers.at(m) = 1;
				}
			}
		}
	
	//fills vector with primes
	for(int g = 0; g < maximum_value; ++g){

		//if value in numbers is not a one then place in prime values vector
		if(numbers.at(g) != 1){
			int number_holder =0;

			number_holder = numbers.at(g);
			prime_values.push_back(number_holder);
		}
	}

	//sets a variable to the size of the prime vector (how many primes)
	primes_length = prime_values.size();

	//lists each prime number
	for(int w = 0; w < primes_length; ++w){

		std::cout << prime_values.at(w) << "   ";

	}

	//outputs how many prime numbers there are in the prime number vector
	std::cout << std::endl << std::endl << "There are " << primes_length << " prime numbers in the list" << std::endl << std::endl;
}

	




	



