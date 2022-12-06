#include "mainFrame.h"
#include <wx/wx.h>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <stack>
#include <chrono>
using namespace std;
using namespace std::chrono;

mainFrame::mainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxPanel* panel = new wxPanel(this);
	panel->SetBackgroundColour(wxColour(*wxLIGHT_GREY));

	//Search bar display
	wxStaticText* searchDis = new wxStaticText(panel, wxID_ANY, "Enter the License Number of the Driver", wxPoint(50, 40));
	wxTextCtrl* searchBar = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(50, 65), wxSize(200, -1), wxTE_RICH2 | wxTE_PROCESS_ENTER | wxTE_LEFT);
	
	//THis is what happens when user presses enter key
	searchBar->Bind(wxEVT_TEXT_ENTER, &mainFrame::pressedEnt, this);

	//These are the different output boxes for driver info
	wxStaticText* name = new wxStaticText(panel, wxID_ANY, "Name", wxPoint(400, 40));
	wxStaticText* licensePlate = new wxStaticText(panel, wxID_ANY, "License Plate", wxPoint(400, 90));
	wxStaticText* vinNum = new wxStaticText(panel, wxID_ANY, "VIN #", wxPoint(400, 140));
	wxStaticText* vehicleYear = new wxStaticText(panel, wxID_ANY, "Vehicle Year", wxPoint(400, 190));
	wxStaticText* companyNum = new wxStaticText(panel, wxID_ANY, "Company #", wxPoint(400, 240));
	wxStaticText* companyName = new wxStaticText(panel, wxID_ANY, "Company Name", wxPoint(400, 290));
	wxStaticText* carType = new wxStaticText(panel, wxID_ANY, "Car Type", wxPoint(400, 340));
	wxStaticText* phoneNum = new wxStaticText(panel, wxID_ANY, "Phone Number", wxPoint(400, 390));
	wxStaticText* address = new wxStaticText(panel, wxID_ANY, "Address", wxPoint(400, 440));

	//These are the different output boxes for execution time
	wxStaticText* dfs = new wxStaticText(panel, wxID_ANY, "DFS Execution Time (ms)", wxPoint(50, 240));
	wxStaticText* bfs = new wxStaticText(panel, wxID_ANY, "BFS Execution Time (ms)", wxPoint(50, 290));

	CreateStatusBar();
}

//This is called when the  user presses enter key
void mainFrame::pressedEnt(wxCommandEvent& event)
{
	//Struct where all the driver information is stored
	struct driver
	{
		driver* left;
		driver* right;
		string licenseNum;
		string driverName;
		string plateNum;
		string vinNum;
		string year;
		string companyNum;
		string comanyName;
		string carType;
		string phoneNum;
		string address;

		//Default constructor
		driver() {
			left = nullptr;
			right = nullptr;
			string licenseNum = "";
			string driverName = "";
			string plateNum = "";
			string vinNum = "";
			string year = "";
			string companyNum = "";
			string comanyName = "";
			string carType = "";
			string phoneNum = "";
			string address = "";
		}
		//Constructer with data from the csv file
		driver(string licenseNum, string driverName, string plateNum, string vinNum, string year, string companyNum, string comanyName, string carType, string phoneNum, string address) {
			left = nullptr;
			right = nullptr;
			this->licenseNum = licenseNum;
			this->driverName = driverName;
			this->plateNum = plateNum;
			this->vinNum = vinNum;
			this->year = year;
			this->companyNum = companyNum;
			this->comanyName = comanyName;
			this->carType = carType;
			this->phoneNum = phoneNum;
			this->address = address;
		}
	};

	class Tree {
		driver* root;
	public:
		Tree() {
			root = nullptr;
		}
		void setRoot(driver* root) {
			this->root = root;
		}
		driver* getRoot() {
			return root;
		}
		//Function used to insert data into the AVL tree
		driver* insert(driver* node, string licenseNum, string driverName, string plateNum, string vinNum, string year, string companyNum, string comanyName, string carType, string phoneNum, string address) {
			if (node == nullptr) {
				driver* node = new driver(licenseNum, driverName, plateNum, vinNum, year, companyNum, comanyName, carType, phoneNum, address);
				return node;
			}
			if (node->licenseNum > licenseNum) {
				node->left = insert(node->left, licenseNum, driverName, plateNum, vinNum, year, companyNum, comanyName, carType, phoneNum, address);
			}
			else if (node->licenseNum < licenseNum) {
				node->right = insert(node->right, licenseNum, driverName, plateNum, vinNum, year, companyNum, comanyName, carType, phoneNum, address);
			}
			else {
				return node;
			}
			node = balance(node);
			return node;
		}
		//Balances the tree
		driver* balance(driver* node) {
			int balanceFactor = findBalanceFactor(node);
			if (balanceFactor <= -2) {
				if (findBalanceFactor(node->right) >= 1) {
					node->right = rotateRight(node->right);
					node = rotateLeft(node);
				}
				else {
					node = rotateLeft(node);
				}
			}
			else if (balanceFactor >= 2) {
				if (findBalanceFactor(node->left) <= -1) {
					node->left = rotateLeft(node->left);
					node = rotateRight(node);
				}
				else {
					node = rotateRight(node);
				}
			}
			return node;
		}
		//Finds the height of the tree
		int height(driver* root) {
			if (root == nullptr) {
				return 0;
			}
			else {
				return 1 + max(height(root->left), height(root->right));
			}
		}
		//Find the balance factor of the tree
		int findBalanceFactor(driver* node) {
			if (node == nullptr) {
				return 0;
			}
			else {
				return height(node->left) - height(node->right);
			}
		}
		//Right rotations for the tree
		driver* rotateLeft(driver* node) {
			driver* rightChild = node->right;
			node->right = rightChild->left;
			rightChild->left = node;
			return rightChild;
		}
		//Left rotations for the tree
		driver* rotateRight(driver* node) {
			driver* leftChild = node->left;
			node->left = leftChild->right;
			leftChild->right = node;
			return leftChild;
		}
		//This is where the depth first search occurs
		//Inspiration taken from GeeksforGeeks depth first search and modified to fit our code
		string depthFirst(driver* root, string target) {
			stack<driver*> s;
			driver* node = root;
			while (node != nullptr || !s.empty()) {
				while (node != nullptr) {
					s.push(node);
					node = node->left;
				}
				node = s.top();
				s.pop();
				if (node->licenseNum == target) {
					string temp = node->driverName + ", " + node->plateNum + ", " + node->vinNum + ", " + node->year + ", " + node->companyNum + ", " + node->comanyName + ", " + node->carType + ", " + node->phoneNum + ", " + node->address;
					return temp;
				}
				node = node->right;
			}
		}
		//This is where the breadth first search occurs
		//Inspiration taken from GeeksforGeeks breadth first search and modified to fit our code
		string breadthFirst(driver* root, string target) {
			queue<driver*> q;
			q.push(root);
			while (q.empty() == false) {
				if (q.front()->licenseNum == target) {
					string temp = q.front()->driverName + ", " + q.front()->plateNum + ", " + q.front()->vinNum + ", " + q.front()->year + ", " + q.front()->companyNum + ", " + q.front()->comanyName + ", " + q.front()->carType + ", " + q.front()->phoneNum + ", " + q.front()->address;
					return temp;
				}
				driver* node = q.front();
				q.pop();
				if (node->left != nullptr) {
					q.push(node->left);
				}
				if (node->right != nullptr) {
					q.push(node->right);
				}
			}
		}
	};

	Tree tree;
	ifstream file("fhv.csv");
	string currentLine;

	getline(file, currentLine);
	
	//Goes through the csv files, parses the data, and stores it in the driver struct
	if (file.is_open()) {
		for (int i = 0; i < 100000; i++) {
			getline(file, currentLine, ',');
			string temp1 = currentLine;

			getline(file, currentLine, ',');
			string temp2 = currentLine;

			getline(file, currentLine, ',');
			string temp3 = currentLine;

			getline(file, currentLine, ',');
			string temp4 = currentLine;

			getline(file, currentLine, ',');
			string temp5 = currentLine;

			getline(file, currentLine, ',');
			string temp6 = currentLine;

			getline(file, currentLine, ',');
			string temp7 = currentLine;

			getline(file, currentLine, ',');
			string temp8 = currentLine;

			getline(file, currentLine, ',');
			string temp9 = currentLine;

			getline(file, currentLine);
			string temp10 = currentLine;

			driver* root = tree.insert(tree.getRoot(), temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8, temp9, temp10);
			tree.setRoot(root);
		}
	}
	
	wxString str = wxString::Format(event.GetString());
	string vln = std::string(str);
	string temp;

	//Finds the execution times for bfs and dfs
	//Syntax for timer taken from GeeksforGeeks and modified to print out milliseconds
	auto start = high_resolution_clock::now();
	temp = tree.depthFirst(tree.getRoot(), vln);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	double dfs = (double)duration.count() / 1000;
	string dfsTime = to_string(dfs);

	start = high_resolution_clock::now();
	temp = tree.breadthFirst(tree.getRoot(), vln);
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	double bfs = (double)duration.count() / 1000;
	string bfsTime = to_string(bfs);

	//Seperates the returned string of driver data from the search so it can be printed out properly
	int x = temp.find(',');
	string driverName = temp.substr(0, x);
	temp = temp.substr(x + 2, temp.length() - x);
	x = temp.find(',');
	string plateNum = temp.substr(0, x);
	temp = temp.substr(x + 2, temp.length() - x);
	x = temp.find(',');
	string vinNum = temp.substr(0, x);
	temp = temp.substr(x + 2, temp.length() - x);
	x = temp.find(',');
	string year = temp.substr(0, x);
	temp = temp.substr(x + 2, temp.length() - x);
	x = temp.find(',');
	string companyNum = temp.substr(0, x);
	temp = temp.substr(x + 2, temp.length() - x);
	x = temp.find(',');
	string comanyName = temp.substr(0, x);
	temp = temp.substr(x + 2, temp.length() - x);
	x = temp.find(',');
	string carType = temp.substr(0, x);
	temp = temp.substr(x + 2, temp.length() - x);
	x = temp.find(',');
	string phoneNum = temp.substr(0, x);
	temp = temp.substr(x + 2, temp.length() - x);
	x = temp.find(',');
	string address = temp.substr(0, x);

	//Prints out driver data in the gui
	wxTextCtrl* nameOut = new wxTextCtrl(this, wxID_ANY, driverName, wxPoint(400, 60), wxSize(350, -1));
	wxTextCtrl* licensePlateOut = new wxTextCtrl(this, wxID_ANY, plateNum, wxPoint(400, 110), wxSize(350, -1));
	wxTextCtrl* vinNumOut = new wxTextCtrl(this, wxID_ANY, vinNum, wxPoint(400, 160), wxSize(350, -1));
	wxTextCtrl* vehicleYearOut = new wxTextCtrl(this, wxID_ANY, year, wxPoint(400, 210), wxSize(350, -1));
	wxTextCtrl* companyNumOut = new wxTextCtrl(this, wxID_ANY, companyNum, wxPoint(400, 260), wxSize(350, -1));
	wxTextCtrl* companyNameOut = new wxTextCtrl(this, wxID_ANY, comanyName, wxPoint(400, 310), wxSize(350, -1));
	wxTextCtrl* carTypeOut = new wxTextCtrl(this, wxID_ANY, carType, wxPoint(400, 360), wxSize(350, -1));
	wxTextCtrl* phoneNumOut = new wxTextCtrl(this, wxID_ANY, phoneNum, wxPoint(400, 410), wxSize(350, -1));
	wxTextCtrl* addressOut = new wxTextCtrl(this, wxID_ANY, address, wxPoint(400, 460), wxSize(350, -1));

	//Prints out execution times in the gui
	wxTextCtrl* dfsOut = new wxTextCtrl(this, wxID_ANY, dfsTime, wxPoint(50, 260), wxSize(250, -1));
	wxTextCtrl* bfsOut = new wxTextCtrl(this, wxID_ANY, bfsTime, wxPoint(50, 310), wxSize(250, -1));
}