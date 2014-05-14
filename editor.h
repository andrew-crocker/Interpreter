#include <vector>
#include <string>
using namespace std;

#ifndef _Editor_
#define _Editor_

#pragma once

class line {
	public:
		string source_line;                // Source line as input by user
		list<base_token *> token_list;     // A list of tokens that are found in the source line
};

class line_editor {
	private:
		vector<line *> line_table;
		vector<line *>::iterator iterator;
		line_parser * basic_parser;
		long current_line;
		bool empty;
	public:
		line_editor();
		bool command(string command);
};

#endif

