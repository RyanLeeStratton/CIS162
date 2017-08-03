/********************************************************
*Ryan Stratton
*CS162
*HW2
*10/10/2012
*********************************************************/

//I wasn't able to get my pointer to work with the for loop to fill the values 
//and I also wasn't sure how to go about the stringstream assignment.


#include <iostream>
#include <string>
#include <vector>

#define SIZE 100

int main(int argc, char **argv){

	//sets up strings
	std::string string1 = "This is a string" ;
	std::string string2 = "This is another string";
	std::string string3 = "This is a string";
	std::string string4;
 
	//vector of 0 length
	std::vector <int> Array;

	//outputting the strings
	std::cout << string1 << std::endl;
	std::cout << string2 << std::endl;
	std::cout << string3 << std::endl;

	//requesting an input for string and processing the input
	std::cout <<"Please enter a string, and hit return: " << std::endl;
	std::cin >> string4;
    
	//output of string
	std::cout <<"string4 = " << string4 << std::endl;
    
	//string comparison for same
	if (string1 == string2){
		
		std::cout << "string1 = string2" << std::endl;
	}
	else{

		std::cout << "string1 "; 
			
			//comparison for greater than and less than of strings
			if(string1 > string2){
					std::cout << ">";
				}
			else{
					std::cout << "<";
				}
		std::cout << " string2" << std::endl;
					
		}


	
	if (string1 == string3){
		
		std::cout <<"string1 = string3" << std::endl;
	}
	else{
		std::cout << "string1 ";
				if(string1 > string3){
					std::cout << ">";
				}
				else{
					std::cout << "<";
				}
		std::cout << " string3" << std::endl;


	}
    


	if (string3 == string2) {
		
		std::cout << "string == string2" << std::endl; 
	}
	else{
		std::cout << "string3 ";
				if(string3 > string2){
					std::cout << ">";
				}
				else{
					std::cout << "<";
				}
		std::cout << " string2" << std::endl;
	}

	
	//sets a value for the vector named Array then moves to the next element
	for (int i = 0; i < SIZE; ++i){
		Array.push_back(i); 
		
	}
	
	//prints out the values for each element of the vector Array
	for (int i = 0; i < SIZE; ++i){

		std::cout << Array.at(i) << std::endl;
	}
    
	return 0;
}

