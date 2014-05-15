#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <vector>

using namespace std;

#include <cstdio>
#include "parser.h"
#include "editor.h"

Editor::Editor() {
	current_line = -1;
	empty = true;
	basic_parser = new(nothrow) line_parser;
}

bool Editor::command(string command_line) {
	char line_buffer[32];
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
	
	
	
	// If the command is not insert and the list is empty
	// then inform the user of such and return to caller
	if ( current_line == -1 && command_char != 'E' && command_char != 'Q' && command_char != 'H' && empty) {
		cout << "The file is empty" << endl;
		return false;
	}

	// Process the command based upon the letter entered
	// We simply ignore commands that we do not recognize
	// for this version
	switch ( command_char ) {
	case 'Q': // Quit
		{
			return true;
		}
	case 'H': // Help
		{
			cout << "This mini text editor also serves as an interpreter for your BASIC programs." << endl;
			cout << "The following commands will help you navigate your way around the editor:" << endl << endl;
			cout << "E: Enter Edit mode at current line number. <Esc> to exit Edit mode." << endl;
			cout << "U: Move up one line, or number of lines specified in argument." << endl;
			cout << "D: Move down one line, or number of lines specified in argument." << endl;
			cout << "J: Jump to line specified in argument." << endl;
			cout << "T: Move to top line." << endl;
			cout << "B: Move to bottom line." << endl;
			cout << "R: Remove current line, or line specified in argument." << endl;
			cout << "C: Show current line." << endl;
			cout << "S: Show all lines." << endl;
			cout << "I: Begin interpreter." << endl; 
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
			sprintf(line_buffer, "%8ld* ", current_line + 1);
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
			sprintf(line_buffer, "%8ld* ", current_line + 1);
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
	case 'E':  // Insert lines
		{
			string insertion_string;
			line * insertion_line;
			int line_number = current_line+1;
			int start_number = line_number;
			
			while ( true ) {
				current_line++;
				sprintf(line_buffer, "%8d  ", line_number + 1);
				cout << line_buffer;
				getline(cin, insertion_string);
				if ( insertion_string[0] == 27 || insertion_string == "Esc" || insertion_string == "<Esc>" ) {
					current_line--;
					break;
				}
				empty = false;
				insertion_line = new(nothrow) line;
				insertion_line->input_line = insertion_string;
				if ( basic_parser->parse_string(insertion_line) == true ) {
					line_table.insert(line_table.begin()+current_line, insertion_line);
					line_number++;
				}
				else {
					current_line--;
				}
			}
			cin.clear();
			// current_line = start_number;
			return false;
		}
	case 'R': // Remove one line at a time (count is ignored for now)
		{
			if ( current_line == -1 && line_table.size() == 0 ) {
				line_table.erase(line_table.begin()+current_line);
				empty = true;
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
					empty = true;
				}
			}
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
					sprintf(line_buffer, "%8d* ", line_number++);
				else
					sprintf(line_buffer, "%8d  ", line_number++);
    	  		cout << line_buffer << list_string << endl;
    	  		++iterator;
			}
			return false;
		}
	case 'C': // Show current line
		{
			sprintf(line_buffer, "%8ld  ", current_line + 1);
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
    case 'I': // Interpreter
    	{
    		cout << "Beep boop bop.. I am a robot." << endl;
    		// At some point, I will need to write this code.		
    	}
    }
	return false;
}

