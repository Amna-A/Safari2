#include<iostream>
#include <string.h>

#include "State.h"
#include "Search.h"

//support the input in the following format: safari.exe -t 200 < a2-orig.data 

int main(int argc, char* argv[]) {
	std::cout.setf(std::ios_base::unitbuf); //auto flush
	State s;
	Search sr;
	sr.importGame(s);
	//check & set time entered
	if (argc == 3) {
		if (strcmp(argv[1], "-t") == 0) {//compare 'actual' content
			int time = stoi(argv[2]); //change sstream to stoi-> faster
			sr.setTime(time);
			//test: 
			cout << "[Fixed Time: " << time <<" seconds]"<< endl;
		}
		else { cout << "[Invalid Entry]" << endl; }
	}
	//now accept all game puzzles 
	string CMD;
	while (getline(cin, CMD)) {
		string problem;
		if (CMD[0] == 'P') {//entering board
			if (CMD.size() > 3) {
				problem = CMD.substr(2, CMD.size());  //ex: J1, E20, etc...
				s.setProblem(problem);//for printing output
			}
			//start getting board from file < a2-orig.data 
			string temp_board,line;
			temp_board = "";
			bool complete = false;
			for (int i = 0; i < s.SIZE; i++) {
				getline(cin, line);
				if (line.size() < unsigned(s.SIZE)) {break; }
				else {
					line = line.substr(0, s.SIZE);
					temp_board = temp_board + line;
				}
				if (i == s.SIZE - 1) { complete = true; break; }
			}
			if (complete) {
				s.setBoard(temp_board);
				s.PrintBoard();

				sr.IDsearch();//start IDA* with TT
				
				//sr.Play(); //test BFS
			}
			continue;
		}
		if (CMD.empty()) { continue; } //skip empty lines between problems

		if (CMD[0] == 'q' && CMD.size()==1) { break; }

	}
	return 0;
}


