#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <cctype>

using namespace std;
#include "parser.h"
#include "editor.h"

#define keyword_count sizeof(keywords)/sizeof(char *)

char const * keyword_token::keywords[] = { "LET", "FOR", "TO", "STEP", "NEXT", "DIM",
				     "IF", "THEN", "ELSE", "ENDIF", "READ",
				     "DATA", "END", "FUNCTION", "RETURN", "PROGRAM",
				     "INPUT", "PRINT", "INT", "LEFT$", "RIGHT$",
				     "MID$", "CHR", "ASC", "PROCEDURE", "RANDOMIZE",
				     "REM", "RND" };

// parse the rest of a symbol
const char * symbol_token::parse_token(const char * s) {
	symbol = *s++;
	while ( true ) {
		unsigned char input_char = *s++;
		if ( isalpha(input_char) || isdigit(input_char) ) {
			symbol += input_char;
			continue;
		}
		if (  input_char == '$' || input_char == '%' ) {
			symbol += input_char;
			return s++;
		}
		return s;
	}
}

// compare the parameter s for a keyword
const char * keyword_token::parse_token(const char * s) {
	int keyword_index;
	for ( keyword_index = 0; keyword_index < keyword_count; keyword_index++ ) {
		if ( strcmp(s, keywords[keyword_index]) == 0 ) {
			keyword = (type_of_keyword)keyword_index;
			return NULL;
		}
	}
	return s;
}

// parse the rest of an integer
const char * integer_token::parse_token(const char * s) {
	unsigned char input_char;

	integer_string = *s++;
	while ( true ) {
		input_char = *s++;
		if ( isdigit(input_char) ) {
			integer_string += input_char;
			continue;
		}
		return s;
	}
}

// parse the rest of a literal
const char * literal_token::parse_token(const char * s) {
	unsigned char input_char;

	literal_string.clear();
	s++;
	while ( true ) {
		input_char = *s++;
		if ( input_char == 0 ) {
			cout << "          error: missing literal quotes" << endl;
			return NULL;
		}
		if ( input_char != '\"' ) {
			literal_string += input_char;
			continue;
		}
		return s;
	}
}

// parse the rest of a punctuation sequence - this consists of
// ending up with either one, two or three characters in the
// punctuation string. NB: The sequence .. is accepted as a 
// punctuation token, but must be rejected by the compiler at
// some later stage.
const char * punctuation_token::parse_token(const char * s) {
	unsigned char input_char= *s++;

	punctuation_string = input_char;
	switch ( input_char ) {
	case '<': // Looking for either <, <> or <=
		input_char = *(s+1);
		if ( input_char == '>'  || input_char == '=' ) {
			input_char = *s++;
			punctuation_string += input_char; // <> or <= token
			break;
		}
		break;
	case '>': // Looking for either > or >=
		input_char = *(s+1);
		if ( input_char == '=' ) {
			input_char = *s++;
			punctuation_string += input_char; // >= token
			break;
		}
		break;
	}
	return s++;
}

// parse the input source
bool line_parser::parse_string(line * source_line) {
	base_token * token;
	
	const char * character_pointer = source_line->source_line.c_str();
	source_line->token_list.clear();
	int line_length = source_line->source_line.length();
	while ( true ) {
		int input_char;

		if ( *character_pointer == 0 ) break;
		while ( *character_pointer == ' ' || *character_pointer == 0x09 ) character_pointer++;
		input_char = *character_pointer;
		// Determine what the leading character is of the sequence,
		// create an appropriate token and get the actual token
		// class to parse the rest of it (if any)

		while ( true ) {
			if ( isalpha(input_char) ) {
				// Start of a symbol sequence
				token = new(nothrow) symbol_token;
				break;
			}
			if ( input_char == '\"' ) {
				// Start of literal sequence
				token = new(nothrow) literal_token;
				break;
			}
			if ( isdigit(input_char) ) {
				// Start of number sequence
				token = new(nothrow) integer_token;
				break;
			}
			if ( ispunct(input_char) ) {
				// Start of punctuation sequence
				token = new(nothrow) punctuation_token;
				break;
			}
			break;
		}
		if ( token == NULL ) return false;
		character_pointer = token->parse_token(character_pointer);
		if ( character_pointer == NULL ) return false;
		if ( token->get_type() == base_token::t_symbol ) {
			keyword_token * this_token = new(nothrow) keyword_token;
			if ( this_token->parse_token(static_cast<symbol_token *>(token)->get_symbol()) == NULL ) {
				delete token;
				token = this_token;
			}
			else
				delete this_token;
		}
		// Add the token to the end of the list
		source_line->token_list.push_back(token);
	}
	return true;
}

