#ifndef Main_CPP
#define Main_CPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cassert>
#include "../headers/Hierarchy.h"

Hierarchy employeeHierarchy;

int main(int argc, const char * argv[])
{
	bool correctPars = false;
	if (argc < 4)
	{
		std::cerr << "Expecting 3 arguments: the input file, Employee One's name, Employee Two's name";
	}
	else if (argc == 4)
	{
		correctPars = true;
	}
	if (correctPars == true)
	{
		/*employeeHierarchy.manAdd(1, "Dangermouse", NULL);
		employeeHierarchy.manAdd(2, "Gonzo the great    ", 1);
		employeeHierarchy.manAdd(3, "Invisible Woman", 1);
		employeeHierarchy.manAdd(6, "Black Widow", 2);
		employeeHierarchy.manAdd(12, "Hit Girl", 3);
		employeeHierarchy.manAdd(15, "Super Ted", 3);
		employeeHierarchy.manAdd(16, "Batman", 12);
		employeeHierarchy.manAdd(17, "Catwoman", 6);*/

		//std::string answer = employeeHierarchy.returnData("Testdata.txt", " Catwoman     ", "Super Ted   ");
		std::string answer = employeeHierarchy.returnData(argv[1], argv[2], argv[3]);
		std::cout << answer << std::endl;
	}
}


#endif