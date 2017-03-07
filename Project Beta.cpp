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

	int Episodecounter;
	int Totallast10moves;
	int Averagelast10moves;

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
		Episodecounter = 0;
		Totallast10moves = 0;
		Averagelast10moves = 0;
		//Initializes the Q-Table for the agent
		for (int i = 0; i < Init.States.size(); i++) {
			for (int j = 0; j < 4; j++) {
				double random = SmallRand;
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
		//Same code was used in HW 2 to ensure the agent isnt placed off the grid 
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
		//Checks to see whether the agent's x and y coordinate position is not off the grid and if so
		//puts the agent back on the grid and gives the Q-Table a negative value of -100 for that state and direction movement
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
		double Greed = RAND;
		int Move = 0;  //Initialize a random move up
		

		State = positionx + positiony*xp;

		if (Greed < epsilon) {
			//Random movement in one of the four directions
			Move = rand() % 4;
		}
		else {
			//Take the best action in the direction dictated by the Q-table
			double BestQ =  *max_element(QTable[State].begin(), QTable[State].end());
			for (int i = 0; i <= 3; i++) {
				//cout << QTable[State][i] << endl;
				if (QTable[State][i] == BestQ) {
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

	void Runlearner(grid Matrix, int max_x, int Runs, int Episodes, ofstream &statout, int TotalEpisodes) {


		while (State != Matrix.GoalState) {
			agent_moves(max_x, Matrix);
		}

		/*cout << "Goal: " << Matrix.goalx << ", " << Matrix.goaly << endl;
		cout << "Agent: " << positionx << ", " << positiony << endl;
		cout << "Moves: " << moves << endl;*/
		//cout << Matrix.Reward.at(Matrix.GoalState) << '\t' << QTable[State - 1][1] << '\t' << QTable[State - 5][0] << '\t' << QTable[State + 1][3] << '\t' << QTable[State + 5][2] << endl;

		statout << Runs << '\t' << Episodes << '\t' << moves << endl;

		//Sets the agent back to its starting position
		positionx = Startx;
		positiony = Starty;
		State = StartingState;
		//Sets an Episode counter to determine the total moves of the agent on the last 10 steps
		Episodecounter++;
		//For the last 10 episodes calculates the toal moves and calculates the average
		if (Episodecounter > TotalEpisodes-10) {
			Totallast10moves = moves + Totallast10moves;
		}
		Averagelast10moves = Totallast10moves / 10;
		//Sets the moves counter back to zero
		moves = 0;
	}

	
};


//Declared Tests and functions
void TestD(agent Da, grid Dg);

void TestE(agent Ea);

void TestF(agent Fa, grid Fg);

void TestG();

int main() {
	//Declaring the important variables to be used within the main program
	srand(time(NULL));
	int x; //Xmax
	int y; //Ymax
	int Episodes = 250;

	//Asking for x and y dimensions to create the grid 
	cout << "Indicate the grid size in the x-direction (positive integer only): ";
	cin >> x;
	cout << "Indicate the grid size in the y-direction (positive integer only): ";
	cin >> y;
	
	//Opens a text file
	ofstream stat;
	stat.clear();
	stat.open("LearningCurve.txt");
	//Sets the header for the learning curve text file
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
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < Episodes; j++) {

			Main.Runlearner(board, x, i, j, stat,Episodes);
			//Checks to see if agent's position was reset and the Q-table was not 
			TestE(Main);
			if (j == Episodes - 1) {
				TestF(Main, board);
			}
		}
		//resets the parameters for Test E 
		Main.Episodecounter = 0;
		Main.Totallast10moves = 0;
		Main.Averagelast10moves = 0;
		Main.QTable = Main.StartQTable;
		//Test for each statistical run that the Q-table doesnt exceed the max reward of 100 at any point
		TestD(Main, board);
	}

	//Closes the learning curve text file
	stat.close();



	system("pause");  //Contains a system stop which can be used to keep the program running
	return 0;
}

//Tests and function parameters for the rest of the program 
//Test D: No Q-value in the Q-table ever exceeds the given reward of 100 by reaching the goal state
void TestD(agent Da, grid Dg) {
	int Directions = 4;

	//Runs through entire Q-table and checks the convergent reward
	for (int i = 0; i < size(Dg.States); i++) {
		for (int j = 0; j < Directions; j++) {
			assert(Da.QTable[i][j] <= 100);
		}
	}
}

//Test E: When the agent reaches the goal state, it is reset to the initial state and is identical to a freshly initialized agent, except for the Q-values
void TestE(agent Ea) {
	assert(Ea.State == Ea.StartingState);
	assert(Ea.QTable != Ea.StartQTable);
}

//Test F: The agent is capable of using Q-learning to get to the goal in a near-optimal number of steps
void TestF(agent Fa, grid Fg) {
	int movex = abs(Fg.goalx - Fa.Startx);
	int movey = abs(Fg.goaly - Fa.Starty);
	int total_moves = movex + movey;
	
	assert(total_moves <= Fa.Averagelast10moves && Fa.Averagelast10moves <= total_moves *2);
}

//Test G: The agent can use a different state representation than in TestD and get to the goal state 
void TestG()
{



}