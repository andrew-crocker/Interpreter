#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <vector>

using namespace std;

#include <cstdio>
#include "parser.h"
#include "editor.h"

#define MAX_BUFF_LENGTH 100

string get_filename();
string ask_user(string);

Editor::Editor() {
	current_line = -1;
	empty = true;
	has_filename = false;
	interpreting = false;
	pending_save = false;
	// basic_parser = new(nothrow) token_parser;
}

bool Editor::command(string command_line) {
	char line_buffer[32];
	char user_prompt[MAX_BUFF_LENGTH];
	unsigned int command_line_length;
	unsigned char command_char;
	int command_count = -1;

	// Get the command from the command line
	command_line_length = command_line.length();
	if ( command_line_length == 0 ) return false;
	
	// Command to upper case character
	command_char = (command_line[0]&0xDF);
	
	// Get the command count if there is one
	if ( command_line_length >= 1 ) {
		unsigned int char_index = 1;
		while ( char_index < command_line_length ) {
			unsigned char this_char = command_line[char_index++];
			if ( this_char >= '0' && this_char <= '9' ) {
				command_count = (this_char - '0');
			}
		}
	}
	
	// Check to see if our line table is empty
	empty = (line_table.size() == 0) ? true : false;
	
	// If the command is not insert and the list is empty
	// then inform the user of such and return to caller
	if ( current_line == -1 && command_char != 'I' && command_char != 'Q' && command_char != 'H' && command_char != 'L' && empty) {
		cout << "The file is empty" << endl;
		return false;
	}

	// Process the command based upon the letter entered
	// We simply ignore commands that we do not recognize
	// for this version
	switch ( command_char ) {
	case 'Q': // Quit
		{
			if (pending_save) {
				sprintf(user_prompt, "Would you like to save your work? [Y/N]: ");
				string response = ask_user(user_prompt);
				if (response[0] == 'N') {
					return true;
				}
				command("W");
			}
			return true;
		}
	case 'H': // Help
		{
			cout << "This mini text editor also serves as an interpreter for your BASIC programs." << endl;
			cout << "The following commands will help you navigate your way around the editor:" << endl << endl;
			cout << "I: Enter Edit mode at current line number. <Esc> to exit Edit mode." << endl;
			cout << "U: Move up one line, or number of lines specified in argument." << endl;
			cout << "D: Move down one line, or number of lines specified in argument." << endl;
			cout << "J: Jump to line specified in argument." << endl;
			cout << "T: Move to top line." << endl;
			cout << "B: Move to bottom line." << endl;
			cout << "R: Remove current line, or line specified in argument." << endl;
			cout << "C: Show current line." << endl;
			cout << "S: Show all lines." << endl;
			cout << "W: Write source to file." << endl;
			cout << "L: Load source from file." << endl;
			cout << "P: Print the parse tree." << endl;
			cout << "X: Execute source." << endl; 
			cout << "H: Help." << endl; 
			cout << "Q: Quit BASIC interpreter." << endl;
			cout << endl;	
			return false;
		}
	case 'U': // More up one or more lines
		{
			if ( current_line == 0 ) {
				cout << "At top of file" << endl;
				current_line = -1;
				return false;
			}
			// Default to a command count of one if the user
			// has not given us a count
			if ( command_count == -1 )
				command_count = 1;
			current_line -= command_count;
			if ( current_line < 0 )
				current_line = 0;
			sprintf(line_buffer, "%4ld* ", current_line + 1);
			cout << line_buffer << line_table[current_line]->input_line << endl;
			return false;
		}
	case 'D': // More down one or more lines
		{
			if ( (current_line+1) == line_table.size() ) {
				cout << "At end of file" << endl;
				return false;
			}
			// Default to a command count of one if the user
			// has not given us a count
			if ( command_count == -1 )
				command_count = 1;
			current_line += command_count;
			if ( (current_line+1) >= (long)line_table.size() )
				current_line = line_table.size()-1;
			sprintf(line_buffer, "%4ld* ", current_line + 1);
			cout << line_buffer << line_table[current_line]->input_line << endl;
			return false;
		}
	case 'J': // Jump to specified line
		{
			if (command_count == -1) {
				return false;
			}
			else if (command_count < 0 || command_count > line_table.size()) {
				return false;
			}
			else {
				current_line = command_count-1;
			}
			return false;
		}
	case 'I':  // Insert lines
		{
			string insertion_string;
			line * insertion_line;
			int line_number = current_line+1;
			int start_number = line_number;
			
			while ( true ) {
				current_line++;
				sprintf(line_buffer, "%4d  ", line_number + 1);
				cout << line_buffer;
				getline(cin, insertion_string);
				if ( insertion_string[0] == 27 || insertion_string == "Esc" || insertion_string == "<Esc>" ) {
					current_line--;
					break;
				}
				pending_save = true;
				insertion_line = new(nothrow) line;
				insertion_line->input_line = insertion_string;
				line_table.insert(line_table.begin()+current_line, insertion_line);
				line_number++;
			}
			cin.clear();
			// current_line = start_number;
			return false;
		}
	case 'R': // Remove one line at a time (count is ignored for now)
		{
			if ( current_line == -1 && line_table.size() == 0 ) {
				line_table.erase(line_table.begin()+current_line);
				return false;
			}
			if (command_count > 0) {
				if (command_count > line_table.size()) {
					return false;
				}
				else {
					line_table.erase(line_table.begin()+command_count-1);
					if (command_count < current_line+1 || current_line == line_table.size()) {
						current_line--;
					}
				}
			}
			else {
				if ( (current_line+1) == line_table.size() ) {
					current_line--;
					line_table.erase(line_table.begin()+current_line+1);
				}
				else {
					line_table.erase(line_table.begin()+current_line);
				}
				if (current_line == -1) {
				}
			}
			pending_save = true;
			return false;
		}
	case 'S': // Show lines (count is ignored for now)
		{
			string list_string;
			int line_number = 1;

			iterator = line_table.begin();
    		while( iterator != line_table.end() ) {
				list_string = (*iterator)->input_line;
				if ( iterator == line_table.begin()+current_line )
					sprintf(line_buffer, "%4d* ", line_number++);
				else
					sprintf(line_buffer, "%4d  ", line_number++);
    	  		cout << line_buffer << list_string << endl;
    	  		++iterator;
			}
			return false;
		}
	case 'C': // Show current line
		{
			sprintf(line_buffer, "%4ld  ", current_line + 1);
			cout << line_buffer << line_table[current_line]->input_line << endl;
			return false;
		}
	case 'T': // Move to top of source
		{
			current_line = -1;
			return false;
		}
	case 'B': // Move to bottom of source
		{
			current_line = line_table.size()-1;
			return false;
		}
    case 'X': // Interpreter
    	{

    		// At some point, I will need to write this code.
    		return false;
    	}
    case 'P': // Parser
    	{
    		interpreting = true;
    		command("W");
			fstream source;
			// open the source file
			source.open(filename.c_str(), ios_base::in);
			if ( source.fail() ) {
				cout << "Error opening " << filename << endl;
				return false;
			}
			// Create the token list
			parser = new token_parser(source);
			parser->parse_tokens();
    		parser->print_tokens();
    		interpreting = false;
    		return false;
    	}
    case 'W': // Write to file
    	{
			if (!has_filename) {
				filename = get_filename();
				has_filename = true;
			}
			else {
				if (!interpreting) {
					sprintf(user_prompt, "Write source to %s? [Y/N]: ", filename.c_str());
					string response = ask_user(user_prompt);
					if (response[0] == 'N') {
						filename = get_filename();
					}
				}
			}
			ofstream outfile(filename.c_str());
			string list_string;
			iterator = line_table.begin();
    		while( iterator != line_table.end() ) {
				list_string = (*iterator)->input_line;
    	  		outfile << list_string << endl;
    	  		++iterator;
			}
			outfile.close();
			if (!interpreting) {
				cout << "Source written to " << filename << endl;
			}
			pending_save = false;
    		return false;
    	}
    case 'L': // Load from file
    	{
    		if (pending_save) {
    			sprintf(user_prompt, "Would you like to save your work first? [Y/N]: ");
				string response = ask_user(user_prompt);
				if (response[0] == 'Y') {
					command("W");
				}
    		}
    		fstream source;
    		cout << "What file would you like to load? ";
    		getline(cin, filename);
			// open the source file
			source.open(filename.c_str(), ios_base::in);
			while (source.fail()) {
				cout << "Error opening " << filename << ".  Try again: ";
	    		getline(cin, filename);
				source.open(filename.c_str(), ios_base::in);
			}
			has_filename = true;
			line_table.clear();
			string insertion_string;
			int line_number = -1;
			while (getline(source, insertion_string)) {
				line * insertion_line = new(nothrow) line;
				insertion_line->input_line = insertion_string;
				line_number++;
				line_table.insert(line_table.begin()+line_number, insertion_line);
			}
			current_line = line_number;
    		return false;
    	}
    }
	return false;
}

string get_filename() {
	string filename;
	cout << "Enter filename to write source to: ";
	getline(cin, filename);
	return filename;
}

string ask_user(string prompt) {
	cout << prompt;
	string response = "";
	getline(cin, response);
	response[0] = (response[0]&0xDF);
	while (response[0] != 'Y' && response[0] != 'N') {
		cout << prompt;
		getline(cin, response);
		response[0] = (response[0]&0xDF);
	}
	return response;
}