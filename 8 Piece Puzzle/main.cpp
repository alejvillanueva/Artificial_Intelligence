/*Alejandra Villanueva
A* Search for 8 piece puzzle
Written in C++ in Visual Studio
Nov 6th 2018
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <stdlib.h>

using namespace std;

//A class to represent the board
//Keep tracks of the important info for each state
class State {
public:
	State() {}
	State(int depth) :
		depth(depth), board(3, vector<int>()) {
	}
	int depth;
	int manDis = 0;
	int aValue = manDis + depth;
	vector<vector<int>> board;
	vector<char> actions;
};

//Overloads the == operator so when two States are compared
//It compares their board
bool operator==(const State& lhs, const State& rhs)
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (lhs.board[i][j] != rhs.board[i][j]) {
				return false;
			}
		}
	}

	return true;
}

//Overloads the < operator to enable the use of a priority queue
//That is filled with the class State. Compares the a value
bool operator<(const State& state1, const State& state2)
{
	return  state2.aValue < state1.aValue;
}

//Goal State
State goalState(0);

//Prioty queue for nodes in frontier.
priority_queue <State> frontier;

//A map of the explored nodes so that no state can be repeated
map<vector<vector<int>>, int> explored;

//Calculates the manhattanDistance of a state
//Called in aValue()
void manhattanDistance(State &currState) {
	int distance = 0;
	int currX = 0, currY = 0, goalX = 0, goalY = 0;

	for (int i = 1; i < 9; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				if (currState.board[j][k] == i) {
					currX = k;
					currY = j;
				}

				if (goalState.board[j][k] == i) {
					goalX = k;
					goalY = j;

				}
			}
		}
		distance += (abs(currX - goalX) + abs(currY - goalY));
	}
	currState.manDis = distance;
}

//Calls manhattan distance function and uses the value 
//to calculate the aValue. Sets state avalue here
void aValue(State &currState) {
	manhattanDistance(currState);
	currState.aValue = currState.depth + currState.manDis;
}

//returns a vector with the possible actions that the
//current state can do. called in createChildStates
vector<char> possibleActions(const State &currState) {
	int x = 0;
	int y = 0;
	vector<char> actions;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (currState.board[i][j] == 0) {
				x = j;
				y = i;
			}
		}
	}
	

	//0 is not on top edge
	if (y != 0) {
		actions.push_back('U');
	}

	//0 is not on bottom edge
	if (y != 2) {
		actions.push_back('D');
	}

	//0 is not on right edge
	if (x != 2) {
		actions.push_back('R');
	}

	//0 is not on left edge
	if (x != 0) {
		actions.push_back('L');
	}

	return actions;
};

//After goal is found, this writes the output file
void writeOutput(State &goal) {
	ofstream myfile;
	myfile.open("Output7.txt");
	vector<vector<int>> board;

	for (auto elem : explored) //Find initial state in the map of explored
	{
		if (elem.second == 0) {
			board = elem.first;
		}
	}
	//Outputs inital board
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			myfile << board[i][j] << " ";
		}

		myfile << "\n";
	}

	myfile << "\n";

	//outputs goal board
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			myfile << goal.board[i][j] << " ";
		}

		myfile << "\n";
	}

	//outputs depth and nodes generated and actions
	myfile << "\n" << goal.depth << "\n" << frontier.size() + explored.size() + 1 << endl;
	for (char act : goal.actions) {
		myfile << act << " ";
	}
	myfile.close();

}

bool checkFrontier(vector<vector<int>> &curr) {
	priority_queue <State> frontierCheck = frontier;
	map<vector<vector<int>>, int> frontierMap;
	while (frontierCheck.size() != 0) {
		State toCheck = frontierCheck.top();
		frontierCheck.pop();
		(frontierMap[toCheck.board] = 1);
	}

	if (frontierMap.find(curr) == frontierMap.end()) {
		return true;
	}
	else {
		return false;
	}
}

//Given a list of actions, nodes are generated
void makeMovement(State &parent, char action) {
	int x = 0;
	int y = 0;
	vector<vector<int>> newBoard = parent.board;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {

			if (newBoard[i][j] == 0) { //Finds empty space
				x = j;
				y = i;
			}
		}
	}

	//Swaps empty space with other number depending on the action
	if (action == 'L') {
		swap(newBoard[y][x], newBoard[y][x - 1]);
	};

	if (action == 'U') {
		swap(newBoard[y - 1][x], newBoard[y][x]);
	};

	if (action == 'R') {

		swap(newBoard[y][x], newBoard[y][x + 1]);
	};
	
	if (action == 'D') {
		swap(newBoard[y + 1][x], newBoard[y][x]);
	};

	//If board has not been seen before, it creates a new state
	if (explored.find(newBoard) == explored.end()) {
		cout << "NOT FOUND" << endl;
		int newDepth = parent.depth + 1;
		State newState = State(newDepth);

		newState.board = newBoard;
		aValue(newState);

		//Helps keep track of actions
		newState.actions = parent.actions;
		newState.actions.push_back(action);
		frontier.push(newState);
	}



}

//gets possible actions for current state
//call make movement for each action
void createChildStates(State &parent) {

	vector<char> actions = possibleActions(parent);

	for (char a : actions) {
		makeMovement(parent, a);
	}
}
int currA = 0;

//Does the A* Search!
void doSearch() {
	while (!frontier.empty()) {
		State toCheck = frontier.top();
		if (currA < toCheck.aValue) {
			frontier.pop(); //removes node from frontier
		}

		else {
			State toCheck = frontier.top();
			frontier.pop();
			if (toCheck == goalState) { //checks if goal state
				writeOutput(toCheck);
				return;
			}

			else { //adds node to explored
				explored[toCheck.board] = toCheck.depth;
				createChildStates(toCheck);
			}
		}
	}
};

//Reads file and creates goal state and initial state
void readInput(string filename) {
	State initialState(0);
	int first, second, third;

	ifstream ifs(filename);
	if (!ifs) {
		cerr << "Could not open the file.\n";
		exit(1);
	}

	for (int i = 0; i < 3; i++) {
		ifs >> first >> second >> third;
		initialState.board[i] = { first, second, third };
	}



	for (int i = 0; i < 3; i++) {
		ifs >> first >> second >> third;
		goalState.board[i] = { first, second, third };
	}

	//Calculates avalue for initial state and add its to frontier
	aValue(initialState);
	currA = initialState.aValue;
	frontier.push(initialState);
};

int main() {
	string fileName; // filename
	cin >> fileName; // asks user for filename they want to use
	readInput(fileName); //parses info from file.
	doSearch(); //calls search

	cout << frontier.size() << endl;
	cout << explored.size() << endl;
	system("pause");
}
