#ifndef Hierarchy_CPP
#define Hierarchy_CPP

#include "../headers/Hierarchy.h"
#include <sstream>
#include <fstream> //used by file handling
#include <algorithm> //used by ::distance
#include <regex> // used for regular expression
#include <cctype> // used to uppercase the first Char
#include <iterator> //used by std::istream_iterator
#include <iostream>

employeeInfo empInfoStruct;

bool Hierarchy::createEmpoyeeData(std::string fileName)
{
	/* Using regular expression, "[ \\t]*\\|[ \\t]*([0-9]+)[ \\t]*\\|[ \\t]*([a-zA-Z ]+[a-zA-Z]+)[ \\t]*\\|[ \\t]*([0-9]+|[ ])[ \\t]*\\|[ \\t]*"
	First group is ([0-9]+) - 1 or more digits from 0-9, Second group is ([a-zA-Z ]+[a-zA-Z]+) - Any character or space followed by a character. Should
	trim any trailing spaces, Third group is ([0-9]+|[ ]) - any numeric digits or a space (for the top level manager)*/
	employeeInfo empTmpInfo;

	std::regex regex("[ \\t]*\\|[ \\t]*([0-9]+)[ \\t]*\\|[ \\t]*([a-zA-Z ]+[a-zA-Z]+)[ \\t]*\\|[ \\t]*([0-9]+|[ ])[ \\t]*\\|[ \\t]*");
	std::string line;
	bool done = false;
	std::ifstream infile("files/" + fileName);

	/*Going through the file and extracting the data if matches the regular expression. The data is then used to populate the intergers and struct
	data before being mapped on EmployeeID to a new record on the map employeeData_*/
	if (!infile)
	{
		done = false;
	}
	else
	{
		while (std::getline(infile, line))
		{
			int empID;
			std::string empName;
			int manID;
			std::smatch match;
			if (std::regex_search(line, match, regex))
			{
				std::istringstream bufferID(match[1]);
				bufferID >> empID;
				empName = split(match[2]);

				if (match[3] == ' ')
				{
					manID = NULL;
				}
				else
				{
					std::istringstream bufferID(match[3]);
					bufferID >> manID;
				}
				empTmpInfo.employeeName = empName;
				empTmpInfo.manID = manID;
				employeeData_.insert(std::pair<int, employeeInfo>(empID, empTmpInfo));
			}
		}
		done = true;
	}

	return done;
}

void Hierarchy::manAdd(int id, std::string username, int manID)
{
	/*In order to manually add a new record. Used to test the functions before the file reading was completed*/
	empInfoStruct.employeeName = split (username);
	empInfoStruct.manID = manID;
	employeeData_.insert(std::pair<int, employeeInfo>(id, empInfoStruct));
}

std::string Hierarchy::returnData(std::string fileName, std::string employeeOne, std::string employeeTwo)
{
	/*This is the function that interacts between the Main.CPP and the functions in the Hierarchy.CPP. 
	1) It call the read file function to add the employee data to the employeeData_  Map.
	2) If the file has been read, then it calls the function split that capitalises the words held in the employeeOne & Two strings and gets rid of 
	unwanted spaces.
	3) Initiales the creation of the Hierarchy from that name to it's top level manger, then afterwards takes a copy of the results set.
	4) The results set is cleared ready for the search on employeeTwo.
	5) Initiales the creation of the Hierarchy from that employeeTwo to it's top level manger. 
	6) copies the results set for EmployeeTwo and then calls the function to compare those two vectors.
	7) The Hierarchy between the two employees is then passed back to the main ready to be std::cout.*/
	employeeInfo empInfoStruct;
	bool fileRead = createEmpoyeeData(fileName);
	std::string answer; //forward declaration;

	if (fileRead == true)
	{
		std::string formatCheckEmployeeOne = split(employeeOne);
		std::string formatCheckEmployeeTwo = split(employeeTwo);

		//get hierarchy for Employee One
		findName(formatCheckEmployeeOne);
		//Take copy of 
		std::vector<std::string> employeeOneData = resultSet_;

		//clear resultSetOne for Employee Two
		resultSet_.clear();

		//get hierarchy for Employee Two
		findName(formatCheckEmployeeTwo);
		std::vector<std::string> employeeTwoData = resultSet_;

		answer = Compare(employeeOneData, employeeTwoData);
	}
	else
	{
		answer = "Error reading file, check if exists";
	}

	return answer; // this is then passed back to the main and std::cout
}

std::string Hierarchy::split(std::string input)
{
	/*Takes in the Employee's name(s) and firstly get's ride of any spaces and appends then to the back of a vector. The words in the vector are then
	iterated through and each word has it's first character capitalised. Then the words are appened to a string with the appropiate spacing before
	that string is returned back to the function that called it. Used by Hierarchy::returnData() &  Hierarchy::createEmpoyeeData()*/
	std::transform(input.begin(), input.end(), input.begin(), ::tolower); //first convert string to lowercase.
	std::istringstream iss(input);
    std::vector <std::string> colWords;
    std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(colWords)); //Divides words in a string to individual words. Getting rid if spaces.
    // From http://stackoverflow.com/questions/236129/split-a-string-in-c/236803#236803

	std::string returnName;
	std::vector<std::string>::iterator itTemp;
	int counter = 0;
	for (itTemp = colWords.begin(); itTemp != colWords.end(); itTemp++)
	{
		(*itTemp)[0] = std::toupper((*itTemp)[0]); //Capitalise the first character
		if (counter != colWords.size() - 1)
		{
			returnName += *itTemp;
			returnName += ' ';
		}
		else
		{
			returnName += *itTemp;
		}
		++counter;
	}

	return returnName;
}

void Hierarchy::findName(std::string empName)
{
	/*Finds that employee's name within the employeeData_  map by iterating through it and looking in the value stuct under the employeeName
	(ie ->second.employeeName). Once found it will check if there is a manager ID for that employee and if there is an ID for that employee it
	will call the findAdd(id) function using that Manager ID. If there is none then it assumes that the employee has no manager ID. That employee's
	name and ID is then appended to the front of the Hierarchy class resultSet_ vector.*/
	std::vector < std::string > tmpResult;
	employeeInfo empInfoStruct;
	empInfoStruct.employeeName = empName;

	typedef std::map<int, employeeInfo>::iterator it_type;
	for (it_type iterator = employeeData_.begin(); iterator != employeeData_.end(); iterator++)
	{
		if (iterator->second.employeeName == empName)
		{
			if (iterator->second.manID != NULL)
			{
				//find next value
				findAdd(iterator->second.manID);
			}
			std::string combinedNameID = iterator->second.employeeName;
			combinedNameID += " (" + std::to_string(iterator->first) + ')';
			resultSet_.insert(resultSet_.begin(), combinedNameID);
		}
	}
}

void Hierarchy::findAdd(int ID)
{
	/*This function uses recursion and is also called by the function Hierarchy::findName(std::string empName). It uses thr Employee ID passed
	to it and uses the STL Map .find() function to find that employee's ID. If found then that employee's name and ID gets appended to the back
	of the employeeData_ vector. It then check to see if the employee has a Manager ID, if it does then the function calls itself with that 
	Manager ID.*/
	employeeInfo empInfoStruct;
	it_ = employeeData_.find(ID);
	std::string combinedNameID = it_->second.employeeName;
	combinedNameID += " (" + std::to_string(it_->first) + ')';
	resultSet_.push_back(combinedNameID);
	if (it_->second.manID != NULL)
	{
		//find next value
		int nxtID = it_->second.manID;
		findAdd(nxtID);
	}
}

std::string Hierarchy::Compare(std::vector<std::string> employeeOneData, std::vector<std::string> employeeTwoData)
{
	/*Compares the two vectors which contains the searched employee's name and ID up to the top level manager (which has no ID). The vectors
	contain each manager for that employee in order from bottom to top. IE: the first element in the vector will be the employee that was entered
	into the programme at run time.Then the last element will be that top level manager for that chain.
	EmployeeOne vector is compared between EmployeeTwo vector from first element to last element until it finds the first common employee name with ID
	that they both have in common.
	Once it has found the first common manager it works out where those employees were found in the vectors using the ::distance function. This then
	enables the unwanted employees after the first found common employee to be removed. The EmployeeTwo vector also removes the first common employee.
	The EmployeeTwo vector is then reversed in it's order and then the vectors are passed to the toString() function which creates a string of the
	path between the two employees. The string is then returned. */
	bool foundCommon = false;
	int empOnePos = 0;
	std::string copiedAnswer; // answer is out of scope, forward declaration
	for (std::vector<std::string>::iterator empOneIt = employeeOneData.begin(); empOneIt != employeeOneData.end(); ++empOneIt)
	{
			vecIt_ = std::find(employeeTwoData.begin(), employeeTwoData.end(), *empOneIt);
			if (vecIt_ != employeeTwoData.end() && foundCommon == false)
			{
				foundCommon = true;
				int pos = std::distance(employeeTwoData.begin(), vecIt_);  //to find position.
				if (employeeTwoData.size() > 1)
				{
					employeeTwoData.erase(employeeTwoData.begin() + pos, employeeTwoData.end()); //remove unwanted data ie from common name to end.
				}
				std::reverse(employeeTwoData.begin(), employeeTwoData.end()); //reverse order as reached first common person in Hierarchy
				empOnePos = std::distance(employeeOneData.begin(), empOneIt);
			}
		
	}

	if ((employeeOneData.size() > 1) && (foundCommon == true))
	{
		employeeOneData.erase(employeeOneData.begin() + empOnePos +1, employeeOneData.end());
	}

	if (employeeTwoData.size() > 0)
	{
		std::string answer = toString(employeeOneData, true);
		answer += toString(employeeTwoData, false);
		copiedAnswer = answer;
	}
	else
	{
		std::string answer = toString(employeeOneData, true);
		copiedAnswer = answer;
	}

	return copiedAnswer;
}

std::string Hierarchy::toString(std::vector<std::string> data, bool empOneData)
{
	/* Goes through the vectors and creates a string between the employee and it's manager adding the arrows showing the direction of the Hierarchy.*/
	std::string result;

	if (empOneData == true)
	{
		
		for (int it = 0; it < data.size(); it++)
		{
			if (it == data.size() - 1)
			{
				result += data[it];
			}
			else
			{
				result += data[it];
				result += "->";
			}
		}
	}
	else
	{
		for (int it = 0; it < data.size(); it++)
		{
			if ((it == 0) || (it == data.size() -1))
			{
				result += "<-";
				result += data[it];
			}
			else
			{
				result += data[it];
				result += "<-";
			}
		}
	}

	return result;

}

#endif