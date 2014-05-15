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
		bool has_filename;
		bool interpreting;
		bool pending_save;
		long current_line;
		string filename;
		vector<line *> line_table;
		vector<line *>::iterator iterator;
		token_parser * parser;
	public:
		Editor();
		bool command(string command);
		friend void load_file(int, char **, Editor &);
};

#endif

