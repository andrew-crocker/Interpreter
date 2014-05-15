#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

#include <cstdio>
#include "parser.h"
#include "editor.h"

void load_file(int argc, char ** argv, Editor &editor) {
	fstream source;
	// open the source file
	source.open(argv[1], ios_base::in);
	int attempt_num = 1;
	while (source.fail()) {
		if (attempt_num == argc-1) {
			cout << "Error opening " << argv[attempt_num] << "." << endl;
			exit(0);
		}
		source.open(argv[++attempt_num], ios_base::in);
	}
	editor.filename = argv[attempt_num];
	editor.has_filename = true;
	editor.line_table.clear();
	string insertion_string;
	int line_number = -1;
	while (getline(source, insertion_string)) {
		line * insertion_line = new(nothrow) line;
		insertion_line->input_line = insertion_string;
		line_number++;
		editor.line_table.insert(editor.line_table.begin()+line_number, insertion_line);
	}
	editor.current_line = line_number;
}

int main(int argc, char ** argv) {
	string command_line;
	Editor editor;
	if (argc > 1) {
		load_file(argc, argv, editor);
	}
	cout << "Welcome to ACE, the Awesome C++ Editor." << endl;
	cout << "ACE is the last editor you'll ever need." << endl;
	cout << "Type H for Help." << endl << endl;
	while ( true ) {
		cout << "ACE%  ";
		getline(cin, command_line);
		if (editor.command(command_line)) {
			break;
		}
	}
}

