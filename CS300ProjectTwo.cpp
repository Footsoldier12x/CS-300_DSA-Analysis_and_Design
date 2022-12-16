//============================================================================
// Name        : CS300ProjectTwo.cpp
// Author      : Juan Pablo Izquierdo
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>
#include <algorithm>
#include <string>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold course information
struct Course {
    string courseId; // unique identifier
    string courseTitle;
    vector<string> coursePrereqs;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
            Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node *parentNode ,Node *NodeToInsert);
    void displayInOrder(Node* node);
    void postOrder(Node* node);
    void preOrder(Node* node);

    Node* removeNode(Node* node, string courseId);


public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course bid);
    bool IsTreeEmpty();
    Course Search(string courseId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
	root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node

}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root
	displayInOrder(root);

}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // (5) Implement inserting a course into the tree

	// Make a new node for the new course to be inserted.
	Node* newNode = new Node;
	newNode->course = course;

    // if root equarl to null ptr
	if (root == nullptr){
		// root is equal to new node course
		root = newNode;
	}
    // else
	else {
		// add Node root and course
		addNode(root, newNode);
	}
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */

void BinarySearchTree::addNode(Node *parent, Node *nodeToInsert) {
    // (8) Implement inserting a course into the tree

	// if nodeToInsert is smaller than parent node, then insert on left branch
	if (nodeToInsert->course.courseId < parent->course.courseId){
		// if left child is null, insert new node there.
		if (parent->left == nullptr){
			parent->left = nodeToInsert;
		}
		// if left child is not null, then make left node the parent and run insertRecursive again.
		else {
			addNode(parent->left, nodeToInsert);
		}
	}
	// if node to insert is not less than parent, go to the right branch.
	else{
		// if right node is null, insert new node there
		if (parent->right == nullptr){
			parent->right = nodeToInsert;
		}
		// if right node is not null, make right node the parent node and run insertRecursive again.
		else {
			addNode(parent->right, nodeToInsert);
		}
	}
}

/**
 * Return the tree's root
 */
bool BinarySearchTree::IsTreeEmpty(){

	if (root == nullptr){
		return true;
	}
	else{
		return false;
	}
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseId) {
    // set current node equal to root
	Node* currentNode = root;

    // keep looping downwards until bottom reached or matching courseId found
	while (currentNode != nullptr){
        // if match found, return current course
		if (courseId == currentNode->course.courseId){
			return currentNode->course;
		}
        // if course is smaller than current node then traverse left
		else if (courseId < currentNode->course.courseId){
			currentNode = currentNode->left;
		}
        // else larger so traverse right
		else{
			currentNode = currentNode->right;
		}
	}

    Course course;
    return course;
}

// display 
void BinarySearchTree::displayInOrder(Node* node) {
      //if node is not equal to null ptr
	if(node != nullptr){
      //InOrder not left
		displayInOrder(node->left);
      //output courseID, course title, and prereqs if any
	    cout << " - " << node->course.courseId << ": " << node->course.courseTitle << endl;

	    // displays all prereqs found in the course
	    /**
	    for (unsigned int i = 0; i < node->course.coursePrereqs.size(); ++i){
	    	cout << "    Prerequisite " << (i + 1) << ": " << node->course.coursePrereqs.at(i) << endl;
	    }
		*/

      //InOder right
	    displayInOrder(node->right);
	}
}


//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {

	cout << endl;
    cout << course.courseId << ": " << course.courseTitle << endl;

    // displays all prereqs found in the course
    for (unsigned int i = 0; i < course.coursePrereqs.size(); ++i){
    	cout << " Prerequisite " << (i + 1) << ": " << course.coursePrereqs.at(i) << endl;
    }
    cout << endl;
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of courses
            Course course;
            course.courseId = file[i][0];
            course.courseTitle = file[i][1];


            // if the input file has an item at the first index, add it to the coursePrereqs vector
            if (!file[i][2].empty()){
            	//cout << "prereq one found!: " << file[i][2] << endl;
                course.coursePrereqs.push_back(file [i][2]);

            }

            // if the input file has an item at the second index, add it to the couserPrereqs vector
            if (!file[i][3].empty()){
            	//cout << "prereq two found!: " << file[i][3] << endl;
                course.coursePrereqs.push_back(file[i][3]);
            }

            // if course prereqs are not found, empty out coursePrereqs vector.
            if (course.coursePrereqs.at(0) == "no prereq found"){
            	//cout << "no prereqs found, clearing coursePrereqs vector" << endl;
            	course.coursePrereqs.clear();
            }
            // if only the second prereq is not found, print only the first prereq
            else if (course.coursePrereqs.at(1) == "no prereq found"){
        		//cout << " | Prereq 1: " << course.coursePrereqs.at(0);
            	//cout << "no second prereq found, clearing coursePrereqs #2" << endl;
            	course.coursePrereqs.pop_back();
            }
            // both prereqs are found, print both
            else{
        		//cout << " | Prereq 1: " << course.coursePrereqs.at(0);
				//cout << " | Prereq 2: " << course.coursePrereqs.at(1) << endl;
            }

            // push this course to the end
            bst->Insert(course);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        courseKey = "";
        break;
    case 3:
        csvPath = argv[1];
        courseKey = argv[2];
        break;
    default:
        csvPath = "";
        courseKey = "";
    }

    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    int choice = 0;
    string userInput = "";
    string courseCode = "";
    string courseNumber = "";

    // welcome prompt
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Welcome to ABCU's Computer Science department course planner tool." << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;

    while (choice != 9) {

    	cout << "*************************" << endl;
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit." << endl;
    	cout << "*************************" << endl;

        cout << "What would you like to do? ";

        // read user input
		userInput = "";
        cin >> userInput;

        // if userInput is an expected case, it is passed into choice to run the case
        if (userInput == "1" || userInput == "2" || userInput == "3" ||userInput == "9"){
        	choice = stoi(userInput);
        }
        else{
        	choice = 0;

			// ignore the rest of the input in the userInput line and clear the input in the buffer.
			cin.ignore(INT_MAX, '\n');
			cin.clear();
        }

		switch (choice) {

		case 1:

			cout << endl;
			cout << "Enter the name of the list you would like to load from the files below: " << endl;
			cout << " - ABCU_CS_Course_List.csv" << endl << endl;

			cin >> userInput;

			if (userInput == "ABCU_CS_Course_List.csv"){
				csvPath = userInput;

				// Complete the method call to load the courses
				loadCourses(csvPath, bst);

				cout << "Load successful." << endl << endl;

				break;
			}
			else{
				cout << "Invalid file input, please try again." << endl << endl;
				// ignore the rest of the input in the userInput line and clear the input in the buffer.
				cin.ignore(INT_MAX, '\n');
				cin.clear();
				break;
			}

		case 2:
			cout << endl;

			// if the root is empty, then the tree is empty
			if (bst->IsTreeEmpty()){
				cout << "Database is empty, please load data structure first." << endl << endl;
			}
			// print the courses stored in the tree
			else{
				cout << "Here is a sample schedule: " << endl << endl;

				// displays courses stored in binary tree in order
				bst->InOrder();

			    cout << endl;
			}
			break;

		case 3:

			cout << endl;
			cout << "Enter the course code you would like learn about from the list below: " << endl;

			// displays courses stored in binary tree in order
			bst->InOrder();

			cout << "For example: CSCI100" << endl << endl;
			cout << "Enter course code: ";

			// user enters a course code
			userInput = "";
			cin >> userInput;

			// a course code must me 7 in length
			if (userInput.length() == 7){
				// split userInput to make all letters in the courseCode capitalized for the search function
				courseCode = userInput.substr(0, 4);
				std::transform(courseCode.begin(), courseCode.end(), courseCode.begin(), ::toupper);

				// split userInput into course number to concatenate for search function.
				courseNumber = userInput.substr(4,7);

				// rejoind capitalized courseCode and courseNumber for search function
				userInput = courseCode + courseNumber;

				// search for the course in the binary tree
				course = bst->Search(userInput);
			}

			if (!course.courseId.empty()) {
				displayCourse(course);
			} else {
				cout << "Course Id " << userInput << " not found." << endl << endl;

				// ignore the rest of the input in the userInput line and clear the input in the buffer.
				cin.ignore(INT_MAX, '\n');
				cin.clear();
			}

			break;

		case 9:

			cout << endl;
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		    cout << "Thank you for visiting and using the course planner!" << endl;
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
			break;

		default:
				cout << endl;
	        	cout << userInput << " is not a valid option, please try again." << endl << endl;
	        	break;
		}

    }

    cout << "Good bye." << endl;

	return 0;
}
