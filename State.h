#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <random> 
#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <set>

using namespace std;

//helper template for printing map
template<typename K, typename V>
void print_map(std::unordered_map<K, V> const& m) {
	for (auto const& pair : m) {
		std::cout << "{" << pair.first << ": " << pair.second << "}\n";
	}
}

//store information about a state
typedef struct MoveHistory {
	int x1;                       //initial source location at row
	int y1;                       //initial source location at colum
	int x2;                       //location after move at row
	int y2;                       //location after move at y
	vector<vector<char>> state;   //the state after a move
	string move;                  //move ex: el1, xd2..
}MoveHistory;
//bool MatchMember(MoveHistory m1, MoveHistory m2){
//	return m1.state == m2.state;
//}
class State {

	static string Problem;                               //ex: J1 ,A20 ,B2 ,En...
	static MoveHistory MH;                               //info about each move
	static bool goal;                                    //to check if a child is a goal state
	static unordered_map<char, int> _toInt;              //char to int version 2
	static unordered_map<int, char> _toChar;             //int to char version 2                      
	static unordered_map<char, pair<int, int>> location; //location of each piece at board
	//---------------------------------[ HASH ]-------------------------------------------
	static unsigned long long int current_hash;                         //current state hash
	static vector<vector<vector<unsigned long long int>>> ZobristTable; // hash value table
	//------------------------------------------------------------------------------------

public:
	static const string twoPieces;                        //the 2x1 pieces
	static const string threePieces;                      //the 3x1 pieces
	static const int SIZE = 7;                            //dimention of board
	static const int MAX_SIZE = 15;                       //maximum size of table;                     
	static vector<vector<char>> board;                    //game board version 1
	static vector<MoveHistory> history;                   //game history v1 (store path states)
	//static unordered_map<int,tuple<int,int,int, int,string>>H;//to store move info (original & updated location of node)

public:
	State();

	//initialize
	void initialize();

	//get the board buzzle from user 
	void setBoard(const string& b);

	//print the game board
	void PrintBoard()const;

	//save the name of the problem (id)
	void setProblem(string problem);

	//return the problem id
	string getProblem();

	//get location of a piece on board
	pair<int, int> getLocation(char a);

	//whether is on board(debug use)
	bool isOnBoard(int row, int col)const;

	//find all next valid states of a board
	vector<vector<vector<char>>> getNextState(vector<vector<char>> state);

	//check if the current played move is horizontal
	bool isHorizontal(char s, int row, int col);

	//calculate cost //get h(s)
	unsigned int GoalDistEstimate();

	//calculate the cost c(n,m) the distance between two nodes manhatten distance
	unsigned int Cost(int x1, int y1, int x2, int y2);

	//find if game is solved (rover facing exit & path between rover and exit is empty)
	bool isGoal(vector<vector<char>> state);

	//update the location of the rover at the current state
	void updateRoverLoc(vector<vector<char>> s);

	//get child information
	tuple<int, int, int, int, string> getChildInfo(int i, vector<vector<vector<char>>> child);

	//---------------------------------[ HASH ]-------------------------------------------
	//generate random int for table
	unsigned long long int randomInt()const;

	//initialize ZobristTable
	void initialTable()const;

	// Computes the hash value of a given board
	unsigned long long int ComputeHash();

	//get current board hash
	unsigned long long int getHash()const;

	//get the hash of the current move made
	bool moveHash(char a, int x2, int y2, unsigned long long int& moved_hash)const;

	//------------------------------------------------------------------------------------
};
