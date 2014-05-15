#include <vector>
#include <string>
using namespace std;

#ifndef _Editor_
#define _Editor_

#pragma once

class line {
	public:
		string input_line;                 // input from user
		list<base_token *> token_list;     // A list of tokens from the input line
};

class Editor {
		bool empty;
		long current_line;
		vector<line *> line_table;
		vector<line *>::iterator iterator;
		line_parser * basic_parser;
	public:
		Editor();
		bool command(string command);
};

#endif

