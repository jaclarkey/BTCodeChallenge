How to compile:

From Terminal (Linux) g++ -o CodeChallenge src/Main.cpp src/Hierarchy.cpp

 
Assumptions:
— The data file contains the data in the correct columns.

How it works:

-- The file is read, with the employee names being converted into a standard format, and the data is added to a map<int, employeeInfo> employeeData_ with the employee ID as the key.

--The user's input of two employee's names are checked for any leading, trailing spaces or runs of multiple spaces. Beginning characters for each work is capitalised.

--The first employee's name is then searched for within the employeeData_  map. If found and that employee has a manger ID then the function calls the function to find it's managers until it reaches the top managers. These get appended to the private resultSet_ vector.

It then returns to the findName function and appends the first searched employee to the beginning of the resultSet_ vector.

-- The results are copied and it does the same for the second employee's name.

-- Both vectors are then compared from first element to last until it finds the first common employee. Both vectors then have any elements after the found common employee removed (Employee two vector also has the common found employee removed).

-- EmployeeTwoData vector then gets reversed and a string containing the path between the two employees is created.
 

Not Completed:

-- Checking if there is more than one occurrence of an Employee Name but with a different Employee ID.

Bugs:
—— If a top level employee is entered as one of the parameters then that employee may be duplicated in the chain. e.g. ./CodeChallenge TestData.txt "Super Ted" Dangermouse
Super Ted (15)->Invisible Woman (3)->Dangermouse (1)<-Dangermouse (1)