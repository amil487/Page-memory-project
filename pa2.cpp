#include <iostream>
#include <string>
#include <cstring>
#include "pa2.h"
#include <math.h>

using namespace std;

int main(int argc, char ** argv)
{
	if (argc >= 3 || argc == 1)
		return 0;

	string fit;

	if (strcmp(argv[1], "worst") == 0)
		fit = "worst";
	if (strcmp(argv[1], "best") == 0)
		fit = "best";

	LinkedList memory;		//creates linked list object

	for (int i = 0; i < 32; i++)		//initializes linked list with 32 free pages
	{
		memory.createList("free");
	}

	int memoryFree = 32;		//variable to keep track of free pages

	if (fit == "worst")
	{
		cout << "using worst fit algorithm\n\n";
	}

	if (fit == "best")
	{
		cout << "using best fit algorithm\n\n";
	}

		bool exit = true;		//exit boolean
		while (exit)			//loop for main program
		{
			cout << "	1. Add program\n";		//Menu
			cout << "	2. Kill program\n";
			cout << "	3. Fragmentation\n";
			cout << "	4. Print memory\n";
			cout << "	5. Exit\n\n";

			int choice;					//choice value
			cout << "choice - ";
			cin >> choice;

			if (choice > 5 || choice < 1 || cin.fail())		//Invalid choice checker
			{
				cin.clear();
				cin.ignore(256, '\n');
				cout << "Choice must be an integer between 1 and 5, try again\n\n";
				continue;
			}

			if (choice == 1)		//option 1 (add program)
			{
				string programName;
				cout << "Program name - ";
				cin >> programName;

				while (programName == "free")		//check to make sure program isn't named free
				{
					cout << "Sorry, but you can't name a program free, try another name.";
					cin >> programName;
				}

				if (memory.programExister(programName) == false)
				{
					cout << "Error, Program " << programName << " already running.\n\n";
					continue;
				}

				cout << "Program size (KB) - ";
				double programSize;					//Program size input
				cin >> programSize;
				cout << endl;

				double numOfPages = ceil(programSize / 4);		//function used to determine # of pages needed

				if (numOfPages > memoryFree)		//Checker for avaiable pages
				{
					cout << "Error, Not enough memory for Program " << programName << endl << endl;
					continue;
				}
				else
				{
					if (fit == "worst")									//program adding into list for worst and best fits
						memory.addWorst(numOfPages, programName);
					if (fit == "best")
						memory.addBest(numOfPages, programName);

					memoryFree = memoryFree - numOfPages;		//Free memory updater

					cout << "Program " << programName << " added successfully: " << numOfPages << " page(s) used.\n\n";
				}

			}

			if (choice == 2)		//option 2 (kill program)
			{
				memory.pageTracker = 0;		//tracker to see how many pages are reclaimed
				string programName;
				cout << "Program name - ";
				cin >> programName;

				if (memory.programExister(programName) == true)
				{
					cout << "Program " << programName << " not found.\n\n";
					continue;
				}

				for (int i = 0; i < 32; i++)		//Loop to make sure all memory is checked
				{
					memory.remove(programName);
				}

				memoryFree = memoryFree + memory.pageTracker;		//Free memory updater

				cout << "\nProgram " << programName << " successfully killed, " << memory.pageTracker << " page(s) reclaimed.\n\n" ;
			}

			if (choice == 3)		//option 3 (fragmentation checker)
			{
				memory.fragment();
				cout << "\nThere are " << memory.numOfFrees << " fragment(s).\n\n";
			}

			if (choice == 4)		//option 4 (print memory)
				memory.printList();

			if (choice == 5)		//option 5 (exit)
				exit = false; 
		}
	return 0;
}