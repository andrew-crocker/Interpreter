#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

#include <cstdio>
#include "parser.h"
#include "editor.h"

int main(int argc, char *argv[]) {
	string command_line;
	Editor editor;
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

