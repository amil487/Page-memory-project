#include <iostream>
#include <string>
using namespace std;

class LinkedList		//Declare Linked List class
{
private:
	struct Node		//Delcare Node structure
	{
		string data;		//Data within node
		Node* next;			//Pointer to next node
		Node(string data1, Node* next1 = NULL)		//Node constructor
		{
			data = data1;
			next = next1;
		}
	};
	Node* head;		//Head pointer
public:
	LinkedList()		//Linked List constructor
	{
		head = NULL;
	}
	~LinkedList();		//destructor

	int numOfFrees;		//trackers used in methods
	int tracker;
	int pageTracker;

	void createList(string data);				//method declarations
	void addWorst(int size, string data);
	void addBest(int size, string data);
	void remove(string data);
	void printList();
	void fragment();
	void freeCounterWorst();
	void freeCounterBest();
	bool programExister(string data);

};

void LinkedList::createList(string data)
{
	if (head == NULL)		//If list is empty
	{
		head = new Node(data);
	}
	else		//When list is not empty 
	{
		Node* nodePtr = head;			//nodePtr helps us traverse the list
		while (nodePtr->next != NULL)
		{
			nodePtr = nodePtr->next;
		}

		nodePtr->next = new Node(data);			//nodePtr->next is now null so nodePtr now points to the last node of the list
	}											//So a new node is created and placed after the last node
}

void LinkedList::addWorst(int size, string data)		//program insert for worst fit
{
	Node* nodeptr;				//Creation of a node pointer set to head
	nodeptr = head;
	int counter = 1;			//counter used to avoid infinite loops
	fragment();					//fragment and free counter used to figure out where to put program
	freeCounterWorst();

	while (nodeptr != NULL && nodeptr->next != NULL)		//This while block is used to find a fragment block large enough for the program
	{
		while (nodeptr != NULL && nodeptr->next != NULL && nodeptr->data != "free")
		{
			nodeptr = nodeptr->next;
		}
		if (counter == tracker)
		{
			break;
		}
		while (nodeptr != NULL && nodeptr->next != NULL && nodeptr->data == "free")
		{
			nodeptr = nodeptr->next;
		}
		counter++;
	}

	for (int i = 0; i < size; i++)		//Once a fragment is found, it is filled with the program
	{
		nodeptr->data = data;
		nodeptr = nodeptr->next;
	}

	tracker = 0;						//Once the process is over, our generic tracker is reset
}

void LinkedList::addBest(int size, string data)			//program insert for best fit
{
	Node* nodeptr;										//Creation of a node pointer set to head
	nodeptr = head;
	int counter = 1;									//counter used to avoid infinite loops
	fragment();											//fragment and free counter used to figure out where to put program
	freeCounterBest();

	while (nodeptr != NULL && nodeptr->next != NULL)		//This while block is used to find a fragment block large enough for the program
	{
		while (nodeptr != NULL && nodeptr->next != NULL && nodeptr->data != "free")
		{
			nodeptr = nodeptr->next;
		}
		if (counter == tracker)
		{
			break;
		}
		while (nodeptr != NULL && nodeptr->next != NULL && nodeptr->data != "free")
		{
			nodeptr = nodeptr->next;
		}
		counter++;
	}

	for (int i = 0; i < size; i++)					//Once a fragment is found, it is filled with the program
	{
		nodeptr->data = data;
		nodeptr = nodeptr->next;
	}

	tracker = 0;						//Once the process is over, our generic tracker is reset
}

void LinkedList::remove(string data)
{
	Node *nodePtr;

	if (!head)		//Abort if list is empty
	{
		return;
	}

	if (head->data == data)			//If the first node is part of program we want to kill
	{
		head->data = "free";		
		pageTracker++;				//increments page tracker in main
	}
	else
	{
		nodePtr = head;		//Make nodePtr head of list

		while (nodePtr != NULL && nodePtr->data != data)		//Omit all other nodes
		{
			nodePtr = nodePtr->next;
		}

		if (nodePtr)		//removes matching node
		{
			nodePtr->data = "free";
			pageTracker++;				//increments page tracker in main
		}
	}
}

void LinkedList::printList()		//Method for printing memory list
{
	Node* nodePtr = head;

	int i = 1;
	while (nodePtr)					//loop for printing
	{
		cout << nodePtr->data << "	";
		nodePtr = nodePtr->next;

		if (i % 8 == 0)				//line skip after eight pages printed
			cout << endl;
		i++;
	}
	cout << endl;
}

void LinkedList::fragment()		//Method for finding fragmentations
{
	Node* nodeptr;			//node pointer made that starts at head
	nodeptr = head;
	int fragments = 0;		//fragment variable

	while (nodeptr != NULL && nodeptr->next != NULL)		//within this loop we check for free blocks that are connected
	{
		if (nodeptr->data == "free")
		{
			while (nodeptr != NULL && nodeptr->next != NULL && nodeptr->data == "free")			//loop that keeps going while current node and next node is free
			{
				nodeptr = nodeptr->next;
			}
			fragments++;			//when interior loop exits, fragment variable increments
		}
		nodeptr = nodeptr->next;
	}

	numOfFrees = fragments;			//tracker used in free counters
}

void LinkedList::freeCounterWorst()		//method used in addWorst to help position programs
{
	Node* nodeptr;				//node pointer made that starts at head
	nodeptr = head;
	int counter = 0;
	int frags = 1;

	while (nodeptr != NULL && nodeptr->next != NULL)
	{
		while (nodeptr != NULL && nodeptr->next != NULL && nodeptr->data == "free")			//This loop tells us how many frees are contiguous
		{
			nodeptr = nodeptr->next;
			counter++;
		}
		if (frags == numOfFrees)			//Break parameter that also sets the tracker to the number of fragments
		{
			tracker = frags;
			break;
		}
		else if (nodeptr->data != "free")		//When a program is encountered 
		{
			while (nodeptr->next != NULL && nodeptr->data != "free")		//We jump til we hit a free
			{
				nodeptr = nodeptr->next;
			}
		}
		frags++;			//We increment frag and set counter to zero
		counter = 0;
	}
}

void LinkedList::freeCounterBest()		//method used in addBest to help position programs
{
	Node* nodeptr;			//node pointer made that starts at head
	nodeptr = head;
	int counter = 0;
	int frags = 1;
	int limit = 33;			//We set this limit to the total # of possible frees + 1

	while (nodeptr != NULL && nodeptr->next != NULL)
	{
		while (nodeptr != NULL && nodeptr->next != NULL && nodeptr->data == "free")		//This loop tells us how many frees are contiguous
		{
			nodeptr = nodeptr->next;
			counter++;
		}
		if (counter < limit)		//if counter is less than possible # of frees
		{
			limit = counter;		//We set the limit to the number of frees in the previous loop
			tracker = frags;		//We set our tracker equal to the frags found
		}
		if (frags == numOfFrees)	//breaker for when all frees are accounted for
		{
			break;
		}
		else if (nodeptr->data != "free")	//When a program is encountered
		{
			while (nodeptr->next != NULL && nodeptr->data != "free")		//we jump over program memory
			{
				nodeptr = nodeptr->next;
			}
		}
		frags++;		//increase # of frags and reset counter
		counter = 0;
	}
}

bool LinkedList::programExister(string data)		//Checks if program is already running
{
	Node *nodePtr;

	if (!head)		//Abort if list is empty
	{
		return true;
	}

	if (head->data == data)			//If the first node has same name
	{
		return false;
	}
	else
	{
		nodePtr = head;		//Make nodePtr head of list

		while (nodePtr != NULL && nodePtr->data != data)		//Omit all other nodes
		{
			nodePtr = nodePtr->next;
		}

		if (nodePtr)
		{
			return false;
		}
		else
			return true;
	}
}

LinkedList:: ~LinkedList()			//Destructor for linked list
{
	Node* nodePtr = head;
	while (nodePtr != NULL)
	{
		Node* garbage = nodePtr;
		nodePtr = nodePtr->next;
		delete garbage;
	}
}