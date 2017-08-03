/*********************************
Ryan Stratton
Homework3, gravity
CS162
10/30/2012
*********************************/

#include <iostream>


int main(int argc, char **argv){

	//variables required for gravitational force
	double MassOne = 0;
	double MassTwo = 0;
	double Gravity = 0.00000006673;
	double Distance = 0;
	double Force = 0;

	//get first mass
	std::cout << "Input the first mass, measured in grams." << std::endl;
	std::cin >> MassOne;

	//get second mass
	std::cout << "Input the second mass, measured in grams." << std::endl;
	std::cin >> MassTwo;

	//get distance
	std::cout << "Input the distance between the masses, measured in cm." << std::endl;
	std::cin >> Distance;

	//compute force and output value
	Force = ((Gravity * MassOne * MassTwo)/(Distance * Distance));
	std::cout << "The gravitational force between the two masses is " << Force << " ((g*cm)/sec^2) " << std::endl;
	
	return 0;
}