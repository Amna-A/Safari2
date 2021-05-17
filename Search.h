#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <deque>
#include <unordered_map>
#include <set>
#include <chrono>
#include <numeric>

#include"State.h"



using namespace std;

//helper template for creating a fixed size queue
//to avoid memory over 200MB from queue
template <typename T, int MaxLen, typename Container = std::deque<T>>
class FixedQueue : public std::queue<T, Container> {
public:
	void push(const T& value) {
		if (this->size() == MaxLen) {
			this->c.pop_front();
		}
		std::queue<T, Container>::push(value);
	}
};

// the entries to store in TT;
typedef struct TTEntry {
	unsigned long long int hash=0;   // 64 bit hash;
	unsigned int bound;              
}TTEntry;

class Search {

	//--------------------------------[ SEARCH vars ]---------------------------------------
	State game;                                             
	double searchTime;                                      //fixed search time needed to solve a puzzle   
	bool isOverTime;                                        // whether the fixed time is over;
	vector<TTEntry> TT;                                     // transposition table;
	int h_time;                                             // time of h evaluated
	bool Solved;                                            //flag to tell if puzzle is solved
	int TT_size;                                            // size of TT
	unsigned int node_count ;                               // total states
	double total_h;                                         // sum of all estimated h 
	unsigned int minCut;                                    // min depth when cut
	unsigned int maxCut;                                    // max depth when cut
	unsigned int timeCut;                                   // times of cut
	unsigned int f_min;                                     // lower f value bound
	unsigned long long int sumCut;                          // sum of depth of all cut
	unordered_map<int, int> HH;                             // for ordering
	deque<string> path;                                     // to save solution moves: el1,xu3,...
	vector<vector<char>> finalBoard;                        //solved board
	std::chrono::duration<double> time;                     // time of search end
	std::chrono::time_point<std::chrono::system_clock> start_time;// the time when a new search start;
	//------------------------------------------------------------------------------------
public:
	Search();
	//--------------------------------[ SEARCH Functions ]---------------------------------------

	//set the fixed time for solving entire input boards
	void setTime(const double& time);

	//get the time (debug use)
	double getTime()const;

	// initialize before IDS
	void initialize_search();

	// Read gameboard to local vars;
	void importGame(State& game);

	// check whether searching is over time;
	bool OverTime();

	//store info in TT
	void StoreEntry(char a, int x2, int y2, unsigned int& bound);

	//check TT
	bool checkTT(char a, int x2, int y2, unsigned int& tt_bound);

	//Breadth First search (for testing)
	vector<vector<vector<char>>> BFS();

	//Game Driver for BFS
	void Play();

	//printing queue (debug use)
	void PrintQueue(queue<pair<int, vector<vector<char>>>> q);

	//print solutions information
	void print_sol();

	//IDA* with TT version 3
	//unsigned int IDA_TT(unsigned int depth, unsigned int t);

	//IDA* with TT 
	unsigned int IDA_TT(vector<vector<char>> s,unsigned int depth, unsigned int bound, vector<vector<char>> &finalboard, deque<string>& path);

	//Star iterative deapning for IDA* with TT
	void IDsearch();

};

