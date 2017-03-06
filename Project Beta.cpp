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
#include <fstream>

using namespace std;

#define SmallRand (double)rand()/RAND_MAX*.01
#define Direction (double)rand()/RAND_MAX*3
#define RAND (double)rand()/RAND_MAX


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
	int Startx;
	int Starty;
	int positionx;  //Setting the agent's x and y position
	int positiony;
	int xin;  //Agent's positions based on user input
	int yin;
	int StartingState;
	int State;
	int moves;
	double epsilon = .1; //Greedy variable
	double alpha = .1; //Learning varaible
	double gamma = .9;  //Q-Learning variable

	vector<double> actions;
	vector<vector<double>> StartQTable;
	vector<vector<double>> QTable;

	void init(grid Init) {
		//initializing the agent's position
		positionx = -10;
		positiony = -10;
		State = -1;
		moves = 0;
		for (int i = 0; i < Init.States.size(); i++) {
			for (int j = 0; j < 4; j++) {
				double random = 2+SmallRand;
				actions.push_back(random);
			}
			StartQTable.push_back(actions);
			actions.clear();
		}

		QTable = StartQTable;
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

		Startx = positionx;
		Starty = positiony;

		StartingState = positionx + positiony*x;
		State = StartingState;
	};

	void bumpercheck(grid Check, int agentxpos, int agentypos, int Movetaken) {
		if (agentxpos < 0) {
			QTable[State][Movetaken] = -100;
			positionx = 0;
		}
		else if (agentxpos > Check.xmax-1){
			QTable[State][Movetaken] = -100;
			positionx = Check.xmax-1;
		}
		else {
			positionx = agentxpos;
		}
		if (agentypos < 0) {
			QTable[State][Movetaken] = -100;
			positiony = 0;
		}
		else if (agentypos > Check.ymax-1) {
			QTable[State][Movetaken] = -100;
			positiony = Check.ymax-1;
		}
		else {
			positiony = agentypos;
		}
	};

	void agent_moves(int xp, grid fboard) {
		int Greed = RAND;
		int Move = 0;  //Initialize a random move up

		State = positionx + positiony*xp;

		if (Greed < epsilon) {
			//Random movement in one of the four directions
			Move = rand() % 4;
		}
		else {
			//Take the best action in the direction dictated by the Q-table
			int BestQ = *max_element(QTable[State].begin(), QTable[State].end());
			for (int i = 0; i <= 3; i++) {
				if (QTable[State][i] = BestQ) {
					Move = i;
				}
			}
		}

		if (Move == 0) {
			//Move Up
			positiony++;
		}
		else if(Move == 1) {
			//Move Right
			positionx++;
		}
		else if(Move == 2) {
			//Move Down
			positiony--;
		}
		else {
			//Move Left
			positionx--;
		}

		//Adds one to the number of moves
		moves++;

		//Resets the agent if it hits a wall on the outer lines of the grid
		bumpercheck(fboard, positionx, positiony,Move);

		//Updates the agent's position in order to update the QTable of its move
		int NextState = positionx + positiony*xp;

		//Now the QTable is updated with what the agent just did
		updateQ(Move, fboard.Reward, NextState);
	};

	void updateQ(double action, vector<double> reward, int NewestState) {
		//Updates the old position of the agent with new Q data after moving one state 
		double maxQ = *max_element(QTable[NewestState].begin(), QTable[NewestState].end());
		QTable[State][action] = QTable[State][action] + alpha*(reward.at(NewestState) + gamma*maxQ - QTable[State][action]);
		State = NewestState;
	}

};


//Declared Tests and functions
void Runlearner(agent Smith, grid Matrix, int max_x, int Runs, int Episodes, ofstream &statout);

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
	
	ofstream stat;
	stat.clear();
	stat.open("LearningCurve.txt");

	stat << "Run:" << '\t' << "Episode:" << '\t' << "Moves:" << endl;

	//Makes the main grid
	grid board;
	//Sets up the grid boundaries
	board.init();
	board.set_xmax(x);
	board.set_ymax(y);
	board.create_grid();


	//Creates the autonmous agent
	agent Main;
	Main.init(board);
	Main.place(x, y);
	
	cout << "Agent's state: " << Main.State << endl;
	cout << "Goal State: " << board.GoalState << endl;

	//Running the Q-learner 30 statistical times over a certain amount of episodes 

	cout << Main.Startx << " " << Main.Starty << endl;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 10; j++) {
			Runlearner(Main, board, x, i, j, stat);
			cout << "Goal was at: " << board.goalx << ", " << board.goaly << endl;
			cout << "Agent ended at: " << Main.positionx << ", " << Main.positiony << endl;
			cout << "Number of moves: " << Main.moves << endl;
		}
	}

	stat.close();

	cout << Main.Startx << " " << Main.Starty << endl;


	cout << "Goal was at: " << board.goalx << ", " << board.goaly << endl;
	cout << "Agent ended at: " << Main.positionx << ", " << Main.positiony << endl;
	cout << "Number of moves: " << Main.moves << endl;




	system("pause");  //Contains a system stop which can be used to keep the program running
	return 0;
}

//Tests and function parameters for the rest of the program 

void Runlearner(agent Smith, grid Matrix, int max_x, int Runs, int Episodes, ofstream &statout) {

	while (Smith.State != Matrix.GoalState) {
		Smith.agent_moves(max_x, Matrix);
	}

	statout << Runs << '\t' << Episodes << '\t' << Smith.moves << endl; 

	//Sets the agent back to its starting position
	Smith.positionx = Smith.Startx;
	Smith.positiony = Smith.Starty;
	Smith.State = Smith.StartingState;
	//Sets the moves counter back to zero
	Smith.moves = 0;
}

