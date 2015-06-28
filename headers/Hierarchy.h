#ifndef Hierarchy_H
#define Hierarchy_H

#include <vector>
#include <map>
#include <string>

struct employeeInfo
{
	std::string employeeName;
	int manID;
};

class Hierarchy
{
public:
	std::string returnData(std::string fileName, std::string employeeOne, std::string employeeTwo); //Interacts between the functions and the Main.cpp
	bool createEmpoyeeData(std::string fileName); // Creates the data from the file
	void findName(std::string empName); //finds an employee by name.
	void findAdd(int ID); //finds an employee by ID
	std::string Compare(std::vector<std::string> employeeOneData, std::vector<std::string> employeeTwoData); //Compares the two vectors and returns the path between then.
	std::string toString(std::vector<std::string> data, bool empOneData); // Creates a string of the path for that vector and returns it.
	std::string split(std::string input); // Get's ride of unwanted spaces and capitalises the first char of each word the users input and infile data uses.


	//Used to test functions
	void manAdd(int id, std::string username, int manID); //To manually add a employee

private:
	std::map <int, employeeInfo> employeeData_; //Stores all the employee data ready to be searched.
	std::map <int, employeeInfo>::iterator it_; //Iterator for the employeeData_
	//std::map<std::string, int> result_; //
	//std::map<std::string, int>::iterator resultIt_;
	
	std::vector<std::string> resultSet_; //resultSet vector for the path for that one Employee
	std::vector<std::string>::iterator vecIt_; //Iterator for the resultSet_
};

#endif