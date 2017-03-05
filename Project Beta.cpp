// Project Beta.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "stdafx.h"
#include <random>
#include <vector>
#include <assert.h>
#include <conio.h>
#include <time.h>
#include <algorithm>

using namespace std;

#define MHRand (double)rand()/RAND_MAX*.01


//Creating a grid class that standardizes a board space 
class grid {
public:

	int xmin; //Setting x and y min at 0
	int ymin;
	int xmax; //x and y max established by user
	int ymax;
	int goalx;  //determined goal based on the grid size
	int goaly;
	int GoalState;
	
	vector<int> States;  //Number of state representations
	vector<double> Reward; //Sets the reward table for the grid 
	vector <int> rows;  //making a vector for x coor
	vector <int> columns;  //making a vecotr for y coor

	void init() {
		//initializing the grid class
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
		goalx = rand() % x;
	};

	void set_ymax(int y) {
		//setting the grid size and goal location for y
		ymax = y;
		goaly = rand() % y;
	};

	void create_grid() {
		//Creating the seperate vectors for the x and y coordinates 
		GoalState = goalx + goaly*xmax;


		for (int i = xmin; i < xmax; i++) {
			rows.push_back(i);
		}
		for (int j = ymin; j < ymax; j++) {
			columns.push_back(j);
		}
		for (int k = 0; k < ymax; k++) {
			for (int l = 0; l < xmax; l++) {
				States.push_back(l + k*(xmax));
			}
		}
		for (int i = 0; i < States.size(); i++) {
			Reward.push_back(-1);
		}
		Reward.at(GoalState) = 100;
	}

};

//Creating the agent class that decides how to move within the grid based on a goal
class agent {
public:
	int positionx;  //Setting the agent's x and y position
	int positiony;
	int xin;  //Agent's positions based on user input
	int yin;
	int Directions = 4;
	int Stateposition;
	int moves;
	double epsilon = .1; //Greedy variable
	double alpha = .1; //Learning varaible
	double gamma = .9;  //Q-Learning variable

	vector<double> actions;
	vector<vector<double>> Q;

	void init(grid Init) {
		//initializing the agent's position
		positionx = -10;
		positiony = -10;
		Stateposition = -1;
		moves = 0;
		for (int i = 0; i < Init.States.size(); i++) {
			for (int j = 0; j < Directions; j++) {
				double random = 0 + MHRand;
				actions.push_back(random);
			}
			Q.push_back(actions);
			actions.clear();
		}
	};

	void place(int x, int y) {
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
		if (yin < 0) {
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

	void bumpercheck(grid Check, int agentxpos, int agentypos) {
		if (agentxpos < 0) {
			positionx = 0;
		}
		else if (agentxpos > Check.xmax){
			positionx = Check.xmax;
		}
		else {
			positionx = agentxpos;
		}
		if (agentypos < 0) {
			positiony = 0;
		}
		else if (agentypos > Check.ymax) {
			positiony = Check.ymax;
		}
		else {
			positiony = agentypos;
		}
	};



	void updateQ(double oldaction, vector<double> newreward) {
		//Updates the old position of the agent with new Q data after moving one state 



	}


	void find_goal(agent Smith, int xp, int yp, grid fboard) {

		

		cout << "Agent's position is now: (" << Smith.positionx << "," << Smith.positiony << ")" << endl;
	};
};


//Declared Tests and functions
void TestA(grid A, int xcoor, int ycoor);

void TestB(agent Barry, grid Barrygrid);

void TestC(agent Carl, grid Carlgrid);

int main() {
	//Declaring the important variables to be used within the main program
	srand(time(NULL));
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
	for (int i = 0; i < board.States.size(); i++) {
		cout << board.States.at(i) << '\t' << board.Reward.at(i) << '\t' << endl;
	}


	
	//Identifies the grid coordinates for the user
	cout << "Grid is set between 0 and " << board.xmax - 1 << " in the x-direction and between 0 and ";
	cout << board.ymax - 1 << " in the y-direction." << endl;


	//Creates the autonmous agent
	agent Main;
	Main.init(board);

	//Placement for the autonomous agent
	//Main.place(x, y);

	system("pause");  //Contains a system stop which can be used to keep the program running
	return 0;
}


//Test A: When an agent is placed off the gridworld, it will be bumped back onto the gridworld
void TestA(grid A, int xcoor, int ycoor) {
	//Creates a new agent 
	agent Jerry;
	Jerry.init(A);

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

