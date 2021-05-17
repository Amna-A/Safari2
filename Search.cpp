#include <iomanip>
#include "Search.h"

Search::Search() {
	HH;
	path;
	finalBoard;
	searchTime = 990;
	isOverTime = false;
	TT_size = 11000 * 1000;
	TT = vector<TTEntry>(TT_size);
	h_time = 0;
	Solved = false;
	node_count = 0;
	total_h = 0;
	maxCut = 0;
	timeCut = 0;
	sumCut = 0;
	minCut = 990;
	f_min = 999999;
	initialize_search();
}
// initialize parameters before every search
void Search::initialize_search() {
	HH.clear();
	path.clear();
	finalBoard.clear();
	node_count  = 0;
	h_time = 0;
	total_h = 0;
	isOverTime = false;
	Solved = false;
	maxCut = 0;
	timeCut = 0;
	sumCut = 0;
	minCut = 16463370;
	//f_min = 999999999;
	start_time = std::chrono::system_clock::now();
	//to avoid memory usage over 200MB;
	for (int i = 0; i < TT_size; ++i) {
		TT[i] = { };//zero all fields 
	}
}
//import the game from state file (make vars/funcs available)
void Search::importGame(State& s) { game = s; }

//set fixed search time
void Search::setTime(const double& time) {
	searchTime = time;
}
//get search time (debug)
double Search::getTime()const{
	return searchTime;
}
//check if we are over time
bool Search::OverTime() {
	if (isOverTime) { return true; }
	std::chrono::duration<double> d = (std::chrono::system_clock::now() - start_time);
	if (d.count() >= searchTime) { isOverTime = true; return true; }
	return false;
}

//store info on TT
void Search::StoreEntry(char a, int x2, int y2, unsigned int& bound) {
	unsigned long long int _hash = 0;
	//get the hash of the current move
	game.moveHash(a, x2, y2, _hash);

	int index = _hash % TT_size;
	if (TT[index].hash == _hash) {
		TT[index].bound = bound;
		return;
	}
	// round robin window size =1
	else if (index < TT_size - 1) {
		if (TT[index + 1].hash == _hash || TT[index + 1].hash == 0) {
			TT[index + 1].hash = _hash;
			TT[index + 1].bound = bound;
			return;
		}
	}
	TT[index].hash = _hash;
	TT[index].bound = bound;
}
//Check TT
bool Search::checkTT(char a, int x2,int y2, unsigned int& tt_bound) {
	unsigned long long int _hash = 0;
	//get the hash of the current move
	game.moveHash(a,x2,y2,_hash);

	int index = _hash % TT_size;
	if (TT[index].hash == _hash) {
		tt_bound = TT[index].bound;
		return true;
	}
	else if (index < TT_size - 1) {// round robin window size =1
		if (TT[index + 1].hash == _hash) {
			tt_bound = TT[index + 1].bound;
			return true;
		}
	}
	return false;
}

//Breadth First Search (for testing )
vector<vector<vector<char>>> Search::BFS() {
	vector<vector<vector<char>>> Path;
	//Initialize a fixed size queue containing the start state.
	FixedQueue<pair<int, vector<vector<char>>>, 11000 * 1000> my_queue;

	//fixedCapQueue(boost::circular_buffer<int>(3));
	my_queue.push(make_pair(0, game.board));
	set<vector<vector<char>>> seen_states;

	while (!my_queue.empty()) {
		//Dequeue the next state from the front of the stack.
		int key = my_queue.front().first;
		Path.emplace_back(my_queue.front().second);
		my_queue.pop();
		vector<vector<char>> last_path = Path.back();
		//check if this is the goal state -> return true if so
		if (game.isGoal(last_path)) { Solved = true; return Path; }

		if (HH.find(key) == HH.end()) { HH[key] = 1; }
		else { HH[key] += 1; }

		vector<vector<vector<char>>> next_st = game.getNextState(last_path);
		//otherwise get all possible next states and enqueue them at the back of the queue.
		for (vector<vector<vector<char>>>::const_iterator i = next_st.begin(); i != next_st.end(); ++i) {
			if (seen_states.find(*i) == seen_states.end()) {
				seen_states.insert(*i);
				my_queue.push(make_pair(key + 1, *i));
			}
		}
	}
	//PrintQueue(my_queue);
	//If the queue is exhausted, all states were examined and the goal was never reached
	Solved = false; return Path;
}


//Game Driver for BFS
void Search::Play() {
	initialize_search();
	while (true) {
		cout << "\nStarting Search..." << endl;
		vector<vector<vector<char>>> path = BFS();
		cout << "\n[Solved Length: " << path.size() << "]" << endl;
		print_map(HH);
		cout << "\n--------------" << endl;
		cout << "-NEXT PATH-" << endl;
		cout << "--------------" << endl;
		if (path.size() >= 15) {
			for (vector<vector<vector<char>>>::const_iterator i = path.begin(); i != path.end(); ++i) {
				for (vector<vector<char>>::const_iterator j = i->begin(); j != i->end(); ++j) {
					for (vector<char>::const_iterator k = j->begin(); k != j->end(); ++k) {
						cout << *k << ' ';
					}
					cout << endl;
				}
				cout << endl;
				cout << "--------------" << endl;
				break;
			}
		}
	}
}

//printing queue (debug use)
void Search::PrintQueue(queue<pair<int, vector<vector<char>>>> q) {
	if (q.empty()) {
		cout << "\n[Queue is empty!]" << endl;
		return;
	}
	vector<vector<char>> p = q.front().second;
	int count = q.front().first;
	q.pop();
	cout << "\n[Count : " << count << "]" << endl;
	for (auto& row : p) {
		for (auto& s : row) {
			cout << s << ' ';
		}
		cout << endl;
	}
	cout << "\n--------------" << endl;
	PrintQueue(q);
	q.push(make_pair(count, p));
}

//print solutions information
void Search::print_sol() {

	cout << "solution " << game.getProblem() << "  " << fixed << setprecision(3) << time.count() << "  "
		<< path.size() << " ";

	for (unsigned int i = 0; i < path.size(); i++) {
		cout << path.at(i) << "  ";
	}
	cout << endl;
	cout << "[Searched Nodes: " << node_count  << "]  -  "
		 << "[Average h-value: " << (total_h / h_time) << "]   -  "
		 << "[min/max/average depth:" << minCut << " / " << maxCut << " / " << double(sumCut) / double(timeCut) << "]" << endl;
}
unsigned int Search::IDA_TT(vector<vector<char>> s,unsigned int depth, unsigned int bound, vector<vector<char>>&finalboard, deque<string> &path) {
	node_count++;
	cout << "[NODE: " << node_count << "]" << endl;
	int x1, y1, x2, y2;
	int xx1, yy1, xx2, yy2;
	unsigned int distance;
	string move, move2;
	char a;

	if (OverTime()) { 
		return 9999990;
	}
	if (game.isGoal(s)) {
		Solved = true;
		return 0;
	}
	vector<vector<vector<char>>> child = game.getNextState(s);
	for (unsigned int i = 0; i < child.size(); ++i) {
		//check if there is an goal state in children
		
		if (game.isGoal(child[i])) {
			finalboard = child[i];
			/*finalboard[2][5] = '.'; finalboard[2][6] = '.';
			finalboard[3][5] = '.'; finalboard[3][6] = '.';*/
			path.emplace_back("xr1");//since the final state is considered at the exit
			Solved = true;
			return 0;
		}
	}
	
	vector<size_t> index_vec; //for sorting
	vector<unsigned int> b; //cost bound of successor
	/*
	* cost function parameters
		* first pair : x1,y1: position of animal at the original (start state)
		* second pair: x2,y2: position of animal after move is made
	*/
	unsigned int tt_bound = 999999;
	for (unsigned int i = 0; i < child.size(); i++) {
		//get the distance between s and current child (m) c(s, s.child[i])
		if (i == 0) {//at first iteration the distance is from original state to child 0
			tie(xx1, yy1, xx2, yy2, move2) = game.getChildInfo(i,child);
			distance = game.Cost(xx1, yy1, xx2, yy2);
			a = move2.at(0);
		}
		else { //otherwise the distance is between current node and previous node
			tie(x1, y1, x2, y2, move) = game.getChildInfo(i-1, child);
			tie(xx1, yy1, xx2, yy2, move2) = game.getChildInfo(i, child);
			distance = game.Cost(xx2, yy2, x2, y2);
			a = move2.at(0);
		}
		if (checkTT(a, xx2, yy2, tt_bound)) {// m found in tt: value stored in tt_bound
			b.insert(b.begin() + i, distance + tt_bound);
		}
		else {// not found, use heuristic
			game.updateRoverLoc(child[i]);//update rover location at current child h(n)
			tt_bound = game.GoalDistEstimate();
			b.insert(b.begin() + i ,distance + tt_bound);
			h_time++;
			total_h += tt_bound;
		}
	}
	//sort children increasing b[] score
	vector<unsigned int> index(b.size());
	iota(index.begin(), index.end(), 0);
	sort(index.begin(), index.end(), [&](int n1, int n2) { return b[n1] < b[n2]; });

	//---------------------------search---------------------------------------

	unsigned int new_bound = 9999990;
	unsigned int t = 9999990;
	for (unsigned int i = 0; i < child.size(); i++) {
		//get coordinates of the current node to store in table
		tie(xx1, yy1, xx2, yy2, move2) = game.getChildInfo(i, child);
		path.emplace_back(move2);
		child[i] = child[index[i]]; 
		if (b[i] <= bound) { //move ok
			if (!path.empty()) { path.pop_front(); }
			t = 1 + IDA_TT(child[i], depth + 1, bound - 1, finalboard, path);
		}
		else {
			t = b[i];
			if (depth < minCut) { minCut = depth; }
			if (depth > maxCut) { maxCut = depth; }
			sumCut += depth;
			timeCut++;
		}
		if (Solved) {
			path[depth] = move2;
			return t;
		}
		if (t < new_bound) {
			new_bound = t;// better bound
		}
	}
	StoreEntry(a, xx2, yy2, new_bound);
	return new_bound;
}

void Search::IDsearch(){

	start_time = std::chrono::system_clock::now();
	initialize_search();
	cout << "\n[starting IDA* for Problem "<<game.getProblem()<<"]" << endl;

	unsigned int bound = game.GoalDistEstimate();
	bool done = false;
	while (!Solved && bound < 9999990 && !isOverTime) {
		bound = IDA_TT(game.board, 0, bound, finalBoard, path);
	}
	time = (std::chrono::system_clock::now() - start_time);

	if (Solved) { 
		cout << "-Solved Board-" << endl;
		for (auto& r : finalBoard) {
			for (auto& c : r) {
				cout << c << " ";
			}cout << endl;
		}cout << "--------------" << endl;

		print_sol(); 
	}
	else {
		if (!isOverTime) { cout << "\n[Not Solvable!]" << endl; }
		else { cout << "\n[timeout !]" << endl; print_sol(); }
	}

}
