#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

#include <cstdio>
#include "parser.h"
#include "editor.h"

void print_int(string s) {
	for (int i = 0; i < s.size(); ++i) {
		cout << (int)s[i] << " ";
	}
	cout << endl << endl;
}

int main(int argc, char *argv[]) {
	string command_line;
	line_editor editor;
	cout << "Welcome to the best text editor you've ever used for BASIC programming!" << endl;
	cout << "Type H for Help." << endl << endl;
	while ( true ) {
		cout << "[Basic]:  ";
		getline(cin, command_line);
		if (editor.command(command_line)) {
			break;
		}
	}
}

