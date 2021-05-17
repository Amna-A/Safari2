#include "State.h"

State::State() { initialTable(); initialize(); }

string State::Problem;
unordered_map<char, pair<int, int>> State::location;
MoveHistory State::MH;
vector<vector<char>> State::board;
//unordered_map<int, tuple<int,int,int, int,string>> State::H;
vector<MoveHistory> State::history;
bool State::goal = false;
const string State::twoPieces = "afdgjbhikce";      //all animals with 2 pieces (2x1 or 1x2)
const string State::threePieces = "proqs";          //all animals with 3 pieces (3x1 or 1x3)


//transfer char into int 
unordered_map<char, int> State::_toInt = {
	{ '.',0  },     //empty
	{ 'x',1  },     //safari rover
	//dogs
	{ 'u',2  },
	{ 'v',3  },
	//lions
	{ 'a',4  },
	{ 'f',5  },
	//lionesses
	{ 'd',6  },
	{ 'g',7  },
	{ 'j',8  },
	//impalas
	{ 'b',9  },
	{ 'h',10 },
	{ 'i',11 },
	{ 'k',12 },
	//zebras
	{ 'c',13 },
	{ 'e',14 },
	//rhinos
	{ 'p',15 },
	{ 'r',16 },
	//elephants
	{ 'o',17 },
	{ 'q',18 },
	{ 's',19 },
};
//convert int to char colors 
unordered_map<int, char> State::_toChar = {
    {0, '.'},     //empty
	{1, 'x'},     //safari rover
	//dogs
	{2, 'u'},
	{3, 'v'},
	//lions
	{4, 'a'},
	{5, 'f'},
	//lionesses
	{6, 'd'},
	{7, 'g'},
	{8, 'j'},
	//impalas
	{9, 'b'},
	{10,'h'},
	{11,'i'},
	{12,'k'},
	//zebras
	{13,'c'},
	{14,'e'},
	//rhinos
	{15,'p'},
	{16,'r'},
	//elephants
	{17,'o'},
	{18,'q'},
	{19,'s'},
};


//-----------------------------------------------------------------

void State::initialize(){
	goal = false;
	MH.state.clear();
	MH.move = "xx1";
	history.clear();
	//H.clear();
}
//set problem id
void State::setProblem(string p) {
	Problem = p;
}
//get id
string State::getProblem() {
	return Problem;
}

//setting the board puzzle from user input
void State::setBoard(const string& b) {
	location.clear(); 
	board = vector<vector<char>>(SIZE, vector<char>(SIZE, ' '));//initialize board
	for (unsigned int i = 0; i < SIZE; ++i) {
		for (unsigned int j = 0; j < SIZE; ++j) {
			board[i][j] = b.at(i * SIZE + j);
		}
	}
	ComputeHash();//update hash

	//save location of each animal
	for (unsigned int i = 0; i < SIZE; ++i) {
		for (unsigned int j = 0; j < SIZE; ++j) {
			char a = board[i][j];
			if (a != '.') {
				location.emplace(a, make_pair(i, j));
			}
		}
	}
	//for (auto const& pair : location) {
	//	std::cout << "{" << pair.first << ": " << pair.second.first <<","<< pair.second.second << "}\n";
	//}
}

//printing the board
void State::PrintBoard()const {
	cout << "\n--Game Board--" << endl;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
	cout << "--------------" << endl;
}

//-----------------------------------[Hash Vars and Func]--------------------------------------
mt19937 mt(1646337);
unsigned long long int State::current_hash;
vector<vector<vector<unsigned long long int>>> State::ZobristTable;

//generate random int for table
unsigned long long int State::randomInt()const {
	uniform_int_distribution<unsigned long long int> dist(0, UINT64_MAX);
	return dist(mt);
}

//initialize ZobristTable
void State::initialTable()const {
	// board max size MAX_SIZE * MAX_SIZE , with 19 types of animals.
	ZobristTable = vector<vector<vector<unsigned long long int>>>(MAX_SIZE, vector<vector<unsigned long long int>>(MAX_SIZE, vector<unsigned long long int>(20, 0)));
	for (int i = 0; i < MAX_SIZE; i++) {
		for (int j = 0; j < MAX_SIZE; j++) {
			for (int k = 0; k < 20; k++) {
				ZobristTable[i][j][k] = randomInt();
			}
		}
	}
}
// Computes the hash value of a given board
unsigned long long int State::ComputeHash() {
	// Computes the hash value of the board  
	unsigned long long int current_hash = 0;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (board[i][j] != '.') {
				int piece = _toInt[board[i][j]];
				current_hash ^= ZobristTable[i][j][piece];
			}
		}
	}
	return current_hash;
}
//get current board hash
unsigned long long int State::getHash()const { return current_hash; }

//---------------------------------------[Game Funcs]---------------------------------------------

//find the next valid states of the board from a given node s
vector<vector<vector<char>>> State::getNextState(vector<vector<char>> state) {
	
	vector<vector<vector<char>>> state_history; 
	//H.clear();
	set<char> processed_pieces = { '.' };
	vector<vector<char>> next_state;
	char s;
	int key = 0; int i,j,step; 
	string move; 

	//Iterate through every animal on the board and move it
	for (unsigned int r = 0; r < state.size(); r++) {
		for (unsigned int c = 0; c < state[r].size() ; c++) {
			i = r; j = c;
			s = state[i][j]; string animal(1, s);
			if (processed_pieces.find(s) == processed_pieces.end()) {
				processed_pieces.insert(s);//add animal to list if not processed
				if (s == 'u' || s == 'v' || s == 'x') {//to move 2*2 pieces
					//move  up
					next_state = state;
					i = r; j = c;
					step = 1;
					while (isOnBoard(i + 1, j+1)&& isOnBoard(i - 1, j) && state[i - 1][j] == '.' && state[i - 1][j + 1] == '.') {
						next_state[i - 1][j] = s;
						current_hash ^= ZobristTable[i - 1][j][_toInt[s]];
						next_state[i - 1][j + 1] = s;
						current_hash ^= ZobristTable[i - 1][j + 1][_toInt[s]];
						next_state[i + 1][j] = '.';
						current_hash ^= ZobristTable[i + 1][j][_toInt['.']];
						next_state[i + 1][j + 1] = '.';
						current_hash ^= ZobristTable[i + 1][j + 1][_toInt['.']];
						string count = std::to_string(step);
						move = animal + "u" + count;
						//if (H.find(key) == H.end()) { H.emplace(key, make_tuple(i,j,i-1, j,move)); key += 1; }
						state_history.emplace_back(next_state);
						
						MH.x1 = i; MH.y1 = j; MH.x2 = i - 1; MH.y2 = j;
						MH.move = move;
						MH.state = next_state;
						history.push_back(MH);

						i -= 1; step += 1;
					}
					//move down
					next_state = state;
					i = r; j = c;
					step = 1;
					while (isOnBoard(i + 2, j+1) && state[i + 2][j] == '.' && state[i + 2][j + 1] == '.') {
						next_state[i + 2][j] = s;
						current_hash ^= ZobristTable[i + 2][j][_toInt[s]];
						next_state[i + 2][j + 1] = s;
						current_hash ^= ZobristTable[i + 2][j + 1][_toInt[s]];
						next_state[i][j] = '.';
						current_hash ^= ZobristTable[i][j][_toInt['.']];
						next_state[i][j + 1] = '.';
						current_hash ^= ZobristTable[i][j + 1][_toInt['.']];
						string count = std::to_string(step); 
						move = animal + "d" + count;
						//if (H.find(key) == H.end()) { H.emplace(key, make_tuple(i,j,i+1, j,move)); key += 1; }
						state_history.emplace_back(next_state);

						MH.x1 = i; MH.y1 = j; MH.x2 = i + 1; MH.y2 = j;
						MH.move = move;
						MH.state = next_state;
						history.push_back(MH);

						i += 1; step += 1;
					}
					//move right
					next_state = state;
					i = r; j = c;
					step = 1;
					while (isOnBoard(i+1, j + 2) && state[i][j + 2] == '.' && state[i + 1][j + 2] == '.') {
						next_state[i][j + 2] = s;
						current_hash ^= ZobristTable[i][j + 2][_toInt[s]];
						next_state[i + 1][j + 2] = s;
						current_hash ^= ZobristTable[i + 1][j + 2][_toInt[s]];
						next_state[i][j] = '.';
						current_hash ^= ZobristTable[i][j][_toInt['.']];
						next_state[i + 1][j] = '.';
						current_hash ^= ZobristTable[i + 1][j][_toInt['.']];
						string count = std::to_string(step); 
						move = animal + "r" + count;
						//if (H.find(key) == H.end()) { H.emplace(key, make_tuple(i,j,i, j+1, move)); key += 1; }
						state_history.emplace_back(next_state);

						MH.x1 = i; MH.y1 = j; MH.x2 = i; MH.y2 = j+1; MH.move = move;
						MH.state = next_state;
						history.push_back(MH);

						j += 1; step += 1;
					}
					//move left
					next_state = state;
					i = r; j = c;
					step = 1;
					while(isOnBoard(i +1, j + 1) && isOnBoard(i, j - 1) && state[i][j - 1] == '.' && state[i + 1][j - 1] == '.') {
						next_state[i][j - 1] = s;
						current_hash ^= ZobristTable[i][j - 1][_toInt[s]];
						next_state[i + 1][j - 1] = s;
						current_hash ^= ZobristTable[i + 1][j - 1][_toInt[s]];
						next_state[i][j + 1] = '.';
						current_hash ^= ZobristTable[i][j + 1][_toInt['.']];
						next_state[i + 1][j + 1] = '.';
						current_hash ^= ZobristTable[i + 1][j + 1][_toInt['.']];
						string count = std::to_string(step);
						move = animal + "l" + count;
						//if (H.find(key) == H.end()) { H.emplace(key, make_tuple(i,j,i, j - 1, move)); key += 1; }
						state_history.emplace_back(next_state);	
						
						MH.x1 = i; MH.y1 = j; MH.x2 = i; MH.y2 = j - 1;
						MH.move = move;
						MH.state = next_state;
						history.push_back(MH);

						j -= 1; step += 1;
					}
				}
				else {
					int d; //check what type of piece 3*1/1*3 or 2*1/1*2
					if (twoPieces.find(s) != std::string::npos) { d = 2; }
					else if (threePieces.find(s) != std::string::npos) { d = 3; }
					if (isHorizontal(s, i, j)) {//move left and right
						next_state = state;
						i = r; j = c;
						step = 1;
						while(isOnBoard(i , j + d) && state[i][j+d]=='.'){ //right
							next_state[i][j + d] = s;
							current_hash ^= ZobristTable[i][j + d][_toInt[s]]; //update hash
							next_state[i][j] = '.';
							current_hash ^= ZobristTable[i][j][_toInt['.']];//update hash
							string count = std::to_string(step);
							move = animal + "r" + count;
							//if it's not stored, make new key
							//if (H.find(key) == H.end()) { H.emplace(key, make_tuple(r,c,i, j+1,move)); key += 1; }
							state_history.emplace_back(next_state);//save new state in history

							MH.x1 = r; MH.y1 = c; MH.x2 = i; MH.y2 = j + 1; MH.move = move;
							MH.state = next_state;
							history.push_back(MH);

							j += 1; step += 1;
						}
						next_state = state;//copy the board
						i = r; j = c;
						step = 1;
						while(isOnBoard(i, j + d - 1)) {//left
							if (isOnBoard(i, j - 1) && state[i][j - 1] == '.') {
								next_state[i][j - 1] = s;
								current_hash ^= ZobristTable[i][j - 1][_toInt[s]]; //update hash
								next_state[i][j + d - 1] = '.';
								current_hash ^= ZobristTable[i][j + d - 1][_toInt['.']];//update hash
								string count = std::to_string(step);
								move = animal + "l" + count;
								//if (H.find(key) == H.end()) { H.emplace(key, make_tuple(r, c, i, j - 1, move)); key += 1; }
								state_history.emplace_back(next_state);

								MH.x1 = r; MH.y1 = c; MH.x2 = i; MH.y2 = j - 1;
								MH.move = move;
								MH.state = next_state;
								history.push_back(MH);
							}
							j -= 1; step += 1;
						}
					}
					else {//move up and down
						next_state = state;
						i = r; j = c; step = 1;
						while (isOnBoard(i + d - 1, j)) {//up
							if (isOnBoard(i - 1, j) && state[i - 1][j] == '.') {
								next_state[i - 1][j] = s;
								current_hash ^= ZobristTable[i - 1][j][_toInt[s]];
								next_state[i + d - 1][j] = '.';
								current_hash ^= ZobristTable[i + d - 1][j][_toInt['.']];
								string count = std::to_string(step);
								move = animal + "u" + count;
								//if (H.find(key) == H.end()) { H.emplace(key, make_tuple(r, c, i - 1, j, move)); key += 1; }
								state_history.emplace_back(next_state);
								
								MH.x1 = r; MH.y1 = c; MH.x2 = i - 1; MH.y2 = j;
								MH.move = move;
								MH.state = next_state;
								history.push_back(MH);
							}
							i -= 1; step += 1;
						}
						next_state = state;
						i = r; j = c; step = 1;
						while (isOnBoard(i + d, j) && state[i + d][j] == '.') {//down
							next_state[i + d][j] = s;
							current_hash ^= ZobristTable[i + d][j][_toInt[s]];
							next_state[i][j] = '.';
							current_hash ^= ZobristTable[i][j][_toInt['.']];
							string count = std::to_string(step);
							move = animal + "d" + count;
							//if (H.find(key) == H.end()) { H.emplace(key, make_tuple(r,c,i+1,j,move)); key += 1; }
							state_history.emplace_back(next_state);

							MH.x1 = r; MH.y1 = c; MH.x2 = i + 1; MH.y2 = j;
							MH.move = move;
							MH.state = next_state;
							history.push_back(MH);

							i += 1; step += 1;
						}
					}
				}
			}
		}
	}
	return state_history;
}

//check if a row and col are on board
bool State::isOnBoard(int row, int col)const {
	if (col < SIZE && row < SIZE && col >= 0 && row >= 0) { return true; }
	else { return false; }
}

//check if a move made is horizontal or not
bool State::isHorizontal(char s, int row, int col) {
	if (col == 0) {//piece at lest corner
		if (board[row][col + 1] == s) { return true; }
	}
	else if (col == SIZE - 1) {//piece at right corner
		if (board[row][col - 1] == s) { return true; }
	}
	else { if (board[row][col + 1] == s || board[row][col - 1] == s) { return true; } }
	return false;
}

//find if goal is goal state (rover facing exit & path between rover and exit is empty)
bool State::isGoal(vector<vector<char>> state) {
	//check if safari rover is facing the exit at row 2 and row 3 or have exited
	
	if (state[2][6] == 'x' && state[3][6] == 'x') { return true; }
	return false; 
}

//get location of a piece on board
pair<int, int> State::getLocation(char a) {
	return location[a];
}

//approximation heuristics to calculate estimated cost of the cheapest path from the state at node n
//to a goal state (Euclidean distance)
unsigned int State::GoalDistEstimate() {
	int x = location.at('x').first;
	int y = location.at('x').second;
	return unsigned(sqrt(pow(abs(x - 2), 2) + pow(abs(y - 6), 2) * 1.0));
}

//calculate the cost c(n,m) the distance between two nodes (manhatten distance)
unsigned int State::Cost(int x1, int y1, int x2, int y2) {
	return abs(x1 - x2) + abs(y1 - y2);
}
//update the location of the rover(x)
void State::updateRoverLoc(vector<vector<char>> s) {
	
	for (unsigned int i = 0; i < s.size(); i++) {
		for (unsigned int j = 0; j < s[i].size(); j++) {
			if (s[i][j] == 'x') {
				location.at('x').first = i;
				location.at('x').second = j;
				return;
			}
		}
	}
}
//get child coordinates and move info 
tuple<int, int, int, int, string> State::getChildInfo(int i,vector<vector<vector<char>>> child) {
	
	int x1, y1, x2, y2,key; string move;

	for (unsigned int it = 0; it < history.size(); it++) {
		if (child[i] == history[it].state) {
			move = history[it].move; 
			x1 = history[it].x1; y1 = history[it].y1;
			x2 = history[it].x2; y2 = history[it].y2;
		}
	}
	return make_tuple(x1, y1, x2, y2, move);
}
//get the hash of the current move made
bool State::moveHash(char a, int x2,int y2, unsigned long long int& moved_hash)const {
	moved_hash = current_hash ^ ZobristTable[x2][y2][_toInt[a]];
	return true;
};







