// HW 2.cpp : Defines the entry point for the console application.
//Max Pullman
//Homework #2
//February 28, 2017
//ME 493: Autonomy

#include <iostream>
#include "stdafx.h"
#include <random>
#include <vector>
#include <assert.h>
#include <conio.h>

using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

//Creating a grid class that standardizes a board space 
class grid {
public:

	int xmin; //Setting x and y min at 0
	int ymin;
	int xmax; //x and y max established by user
	int ymax;
	int goalx;  //determined goal based on the grid size
	int goaly;

	vector <int> rows;  //making a vector for x coor
	vector <int> columns;  //making a vecotr for y coor

	void init() {
		//initializing the agent class
		xmin = 0;
		ymin = 0;
		xmax = 0;
		ymax = 0;
		goalx = 0;
		goaly = 0;
	};

	void set_xmax(int x) {
		//setting the grid size and goal location for x
		xmax = x;
		goalx = xmax - 2; 
	};

	void set_ymax(int y) {
		//setting the grid size and goal location for y
		ymax = y;
		goaly = ymax - 2;
	};
	
	void create_grid() {
		//Creating the seperate vectors for the x and y coordinates 
		for (int i = xmin; i < xmax; i++) {
			rows.push_back(i);
		}
		for (int j = ymin; j < ymax; j++) {
			columns.push_back(j);
		}
	}

};

//Creating the agent class that decides how to move within the grid based on a goal
class agent {
public:
	int positionx;  //Setting the agent's x and y position
	int positiony;
	int xin;  //Agent's positions based on user input
	int yin;

	void init() {
		//initializing the agent's position
		positionx = -10;
		positiony = -10;
	};

	void place(int x,int y) {
		cout << "Indicate the agent's x position (Positive integer): ";
		cin >> xin;
		cout << "Indicate the agent's y position (Positive integer): ";
		cin >> yin;

		//Making sure the agent is placed within the boundaries of the user inputted grid
		if (xin < 0) {
			positionx = 0;
		}
		else if (xin > x - 1) {
			positionx = x - 1;
		}
		else {
			positionx = xin;
		}
		if (yin < 0 ) {
			positiony = 0;
		}
		else if (yin > y - 1) {
			positiony = y - 1;
		}
		else {
			positiony = yin;
		}
		cout << "Agent has been placed at: (" << positionx << "," << positiony << ")" << endl;
	};

	//Manual control for the agent to get to the predetermined goal
	void manual_mode(char input,grid Manual) {
		//updating position for the manual controls 
		//makes sure that no moves can go past the grid boundaries 
		//Classic computer game controls for directions
		if (input == 'w' || input == 'W'){
			positiony++;
			if (positiony > Manual.ymax - 1){
				positiony = Manual.ymax - 1;
			}
		}
		else if (input == 's' || input == 'S'){
			positiony--;
			if (positiony < 0)	{
				positiony = 0;
			}
		}
		else if (input == 'a' || input == 'A'){
			positionx--;
			if (positionx < 0) {
				positionx = 0;
			}
		}
		else if (input == 'd' || input == 'D'){
			positionx++;
			if (positionx > Manual.xmax - 1) {
				positionx = Manual.xmax - 1;
			}
		}
		//Makes user input a correct character for movement
		else {
			cout << "Please input a correct direction (w for up, d for right, s for down, a for left)";
		}

	};

	void find_goal(agent Smith, int xp, int yp,grid fboard) {
	
		//Calculates the distance between the goal's x and y position and the agents coordinates
		int movex = abs(fboard.goalx - Smith.positionx);
		int movey = abs(fboard.goaly - Smith.positiony);

		//Will always move the agent to the goal's position based on the goal's coordinates
		if (fboard.goalx > Smith.positionx) {
			for (int i = 0; i < movex; i++) {
				Smith.positionx++;
		}
			}
		else if (fboard.goalx <= Smith.positionx) {
			for (int i = 0; i < movex; i++) {
				Smith.positionx--;
			}
		}

		if (fboard.goaly > Smith.positiony) {
			for (int i = 0; i < movey; i++) {
				Smith.positiony++;
			}
		}
		else if (fboard.goaly <= Smith.positiony) {
			for (int i = 0; i < movey; i++) {
				Smith.positiony--;
			}
		}
		
		//Updates the final position of the agent in the function to the main agent
		positionx = Smith.positionx;
		positiony = Smith.positiony;

		cout << "Agent's position is now: (" << Smith.positionx << "," << Smith.positiony << ")" << endl;
	};
};


//Declared Tests and functions
void TestA(grid A, int xcoor, int ycoor);

void TestB(agent Barry, grid Barrygrid);

void TestC(agent Carl, grid Carlgrid);

void Controller(grid Human, int xlimit, int ylimit);

int main(){
	//Declaring the important variables to be used within the main program
	int x; //Xmax
	int y; //Ymax

	//Asking for x and y dimensions to create the grid 
	cout << "Indicate the grid size in the x-direction (positive integer only): "; 
	cin >> x;
	cout << "Indicate the grid size in the y-direction (positive integer only): ";
	cin >> y;

	//Makes the main grid
	grid board;
	//Sets up the grid boundaries
	board.init();
	board.set_xmax(x);
	board.set_ymax(y);
	board.create_grid();
	
	//Identifies the grid coordinates for the user
	cout << "Grid is set between 0 and " << board.xmax - 1 << " in the x-direction and between 0 and ";
	cout << board.ymax - 1 << " in the y-direction." << endl;
	//Creates the autonmous agent
	agent Main;
	Main.init();

	cout << "For Test A, please choose integer values outside of the inputted grid range." << endl;
	
	TestA(board, x, y);
	cout << "Test A has successfully passed." << endl;

	cout << "Goal's position is set at coordinates: (" << board.goalx << "," << board.goaly << ")" << endl;

	cout << "Now place the agent on the board for real this time." << endl;
	
	//Human controlled
	Controller(board, x, y);

	cout << "You thought you were done placing agents?" << endl;
	cout << "Not really, but I swear this is the last time." << endl;
	cout << "Place the autonomous agent on the board." << endl;

	//Placement for the autonomous agent
	Main.place(x, y);
	//Agent will move based on the movement goals 
	Main.find_goal(Main,x, y, board);

	//TestC
	TestC(Main, board);
	cout << "Test C has passed. Agent has reached the goal." << endl;

	system("pause");  //Contains a system stop which can be used to keep the program running
    return 0;
}


//Functions 
//Human Controller Function
void Controller(grid Human,int xlimit,int ylimit) {
	char input;

	//create agent for the main controller
	agent New;
	New.init();

	//Place controller agent
	New.place(xlimit, ylimit);

	while (New.positionx != Human.goalx || New.positiony != Human.goaly) {
		//Need some input from user
		cout << "Choose a move for the agent (w for up, d for right, s for down, a for left): " << endl;
		cin >> input;

		//Takes the individual move indicated
		New.manual_mode(input, Human);

		cout <<  "Agent's position: (" << New.positionx << "," << New.positiony << ")" << endl;
	}

	//Tests the controller location against the goal
	TestB(New, Human);
	cout << "Test B has passed. You have reached the goal!" << endl;

}



//Test A: When an agent is placed off the gridworld, it will be bumped back onto the gridworld
void TestA(grid A, int xcoor, int ycoor) {
	//Creates a new agent 
	agent Jerry;
	Jerry.init();

	Jerry.place(xcoor, ycoor);
	//Ensures that the agent, no matter where it's placed, gets put back on the grid
	assert(Jerry.positionx >= 0 && Jerry.positionx <= xcoor - 1 && Jerry.positiony >= 0 && Jerry.positiony <= ycoor - 1);
}

//Test B: The agent can be hand guided to the pre-determined goal 
//The Manual input has reached the goal
void TestB(agent Barry, grid Barrygrid) {
	assert(Barry.positionx == Barrygrid.goalx && Barry.positiony == Barrygrid.goaly);
}

//Test C: The agent's heuristic rule-of-thumb is capable of moving it to a goal state
//The agent has reached the goal
void TestC(agent Carl, grid Carlgrid) {
	assert(Carl.positionx == Carlgrid.goalx && Carl.positiony == Carlgrid.goaly);
}