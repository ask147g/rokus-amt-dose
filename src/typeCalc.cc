#include "typeCalc.hh"

TypeCalculations::TypeCalculations() {};

TypeCalculations::~TypeCalculations() {};

void TypeCalculations::TypeMessenger() {
	std::cout << "Print type of calculatoins" << std::endl;
	std::cout << "0 for simlpe calculations of 1 fantom in different distance from constructData.xml" << std::endl;
	std::cout << "1 for simlpe calculations of 1 fantom in different distance from constructData.xml with visualization" << std::endl;	
	std::cout << "2 for calculation from text file in macro (a set of distancies)" << std::endl;
	std::cout << "3 for calculation of a plane distribution dose from constructData.xml" << std::endl;
	std::cout << "4 for calculation of a plane distribution dose from constructData.xml with visualization" << std::endl;
	std::cout << "5 for calculation of a plane distribution dose using bash" << std::endl;
	std::cout << "6 for calculation of closet from constructData.xml" << std::endl;
	std::cout << "7 for calculation of closet with visualization" << std::endl;
	std::cout << "8 for calculation of closet using bash" << std::endl;
}