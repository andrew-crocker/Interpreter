#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <cctype>
#include <sstream>

using namespace std;
#include "parser.h"
#include "lexer.h"

string possible_keyword;

#define keyword_count sizeof(keywords)/sizeof(char *)

char const * keyword_token::keywords[] = { 
	"and", "and_eq", "asm", "auto", "bitand", "bitor",
	"bool", "break", "case", "catch", "char", "class", "const",
	"const_cast", "continue", "default", "delete", "do",
	"double", "dynamic_cast", "else", "enum", "explicit",
	"export", "extern", "false", "float", "for", "friend",
	"goto", "if", "inline", "int", "long", "mutable",
	"namespace", "new", "not", "not_eq", "operator", "or",
	"or_eq", "private", "protected", "public", "register",
	"reinterpret_cast", "return", "short", "signed",
	"sizeof", "static", "static_cast", "struct", "switch",
	"template", "this", "throw", "true", "try", "typedef",
	"typeid", "typename", "union", "unsigned", "using",
	"virtual", "void", "volatile", "wchar_t", "while",
	"xor", "xor_eq"
};

// parse the rest of a symbol
int symbol_token::parse_token(stringstream& stream, int input_char) {
	symbol = input_char;
	while ( true ) {
		input_char = stream.get();
		if ( isalpha(input_char) || isdigit(input_char) || input_char == '_' ) {
			symbol += input_char;
			continue;
		}
		return input_char;
	}
}

// print the token to cout
void symbol_token::print_token() {
	cout << "[\"symbol\" , \"" << symbol << "\"]" << endl;
}

// compare the parameter s for a keyword
int keyword_token::parse_token(stringstream& stream, int input_char) {
	int keyword_index;
	for ( keyword_index = 0; keyword_index < keyword_count; keyword_index++ ) {
		if ( strcmp(possible_keyword.c_str(), keywords[keyword_index]) == 0 ) {
			keyword = (type_of_keyword)keyword_index;
			return 0;
		}
	}
	return 1;
}

// print the token to cout
void keyword_token::print_token() {
	cout << "[\"keyword\" , \"" << keywords[keyword] << "\"]" << endl;
}

// parse the rest of an integer
int integer_token::parse_token(stringstream& stream, int input_char) {
	integer_string = input_char;
	if ( input_char == '0' )
	{
		input_char = stream.peek();
		if ( input_char == 'X' || input_char == 'x' ) {
			integer_string += input_char;
			input_char = stream.get();
			while ( true ) {
				input_char = stream.get();
				if ( isxdigit(input_char) ) {
					integer_string += input_char;
					continue;
				}
				return input_char;
			}
		}
	}
	while ( true ) {
		input_char = stream.get();
		if ( isdigit(input_char) ) {
			integer_string += input_char;
			continue;
		}
		return input_char;
	}
}

// print the token to cout
void integer_token::print_token() {
	cout << "[\"integer\" , " << integer_string << "]" << endl;
}

// parse the rest of a literal
int literal_token::parse_token(stringstream& stream, int input_char) {
	literal_string.clear();
	while ( true ) {
		input_char = stream.get();
		if ( input_char == '\\' ) {
			input_char = stream.peek();
			if ( input_char == '\"' || input_char == '\\' ) {
				literal_string += '\\';
				input_char = stream.get();
				literal_string += input_char;
				continue;
			}
			if ( input_char == 0x0A ) {
				cout << "error: EOL encountered before closing literal quotes" << endl;
				exit(0);
			}
			if ( input_char == -1 ) {
				cout << "error: EOF encountered before closing literal quotes" << endl;
				exit(0);
			}
			literal_string += input_char;
			continue;
		}
		if ( input_char != '\"' && input_char != -1 ) {
			literal_string += input_char;
			continue;
		}
		if ( input_char == -1 ) {
			cout << "error: EOF encountered before closing literal quotes" << endl;
			exit(0);
		}
		input_char = stream.get();
		return input_char;
	}
}

// print the token to cout
void literal_token::print_token() {
	cout << "[\"literal\" , \"" << literal_string << "\"]" << endl;
}

// parse the rest of a literal
int const_literal_token::parse_token(stringstream& stream, int input_char) {
	const_literal_string.clear();
	while ( true ) {
		input_char = stream.get();
		if ( input_char == '\\' ) {
			input_char = stream.peek();
			if ( input_char == '\'' || input_char == '\\' ) {
				const_literal_string += '\\';
				input_char = stream.get();
				const_literal_string += input_char;
				continue;
			}
			const_literal_string += input_char;
			continue;
		}
		if ( input_char != '\'' ) {
			const_literal_string += input_char;
			continue;
		}
		input_char = stream.get();
		return input_char;
	}
}

// print the token to cout
void const_literal_token::print_token() {
	cout << "[\"constant literal\" , \"" << const_literal_string << "\"]" << endl;
}

// parse the rest of a punctuation sequence - this consists of
// ending up with either one, two or three characters in the
// punctuation string. NB: The sequence .. is accepted as a 
// punctuation token, but must be rejected by the compiler at
// some later stage.
int punctuation_token::parse_token(stringstream& stream, int input_char) {
	punctuation_string = input_char;
	switch ( input_char ) {
	case '!': // Looking for either ! or !=
		input_char = stream.peek();
		if ( input_char == '=' ) {
			input_char = stream.get();
			punctuation_string += input_char; // != token
		}
		break;
	case '#': // Looking for either # or ##
		input_char = stream.peek();
		if ( input_char == '#' ) {
			input_char = stream.get();
			punctuation_string += input_char; // ## token
		}
		break;
	case '%': // Looking for either % or %=
		input_char = stream.peek();
		if ( input_char == '=' ) {			
			input_char = stream.get();
			punctuation_string += input_char; // %= token
		}
		break;
	case '&': // Looking for either &, && or &=
		input_char = stream.peek();
		if ( input_char == '&' || input_char == '=' ) {
			input_char = stream.get();
			punctuation_string += input_char; // && token
		}
		break;
	case '*': // Looking for either * or *=
		input_char = stream.peek();
		if ( input_char == '=' ) {
			input_char = stream.get();
			punctuation_string += input_char; // *= token
		}
		break;
	case '+': // Looking for either +, ++, or +=
		input_char = stream.peek();
		if ( input_char == '+' || input_char == '=' ) {
			input_char = stream.get();
			punctuation_string += input_char; // ++ or += token
		}
		break;
	case '-': // Looking for either -, --, -=, ->, ->*
		input_char = stream.peek();
		if ( input_char == '-' || input_char == '=' ) {
			input_char = stream.get();
			punctuation_string += input_char; // -- or -= token
		}
		if ( input_char == '>' ) {
			input_char = stream.get();
			punctuation_string += input_char; // -> token
			input_char = stream.peek();
			if ( input_char == '*' ) {
				input_char = stream.get();
				punctuation_string += input_char; // ->* token
			}
		}
		break;
	case '.': // Looking for either ., .. or ...
		input_char = stream.peek();
		if ( input_char == '.' ) {
			input_char = stream.get();
			punctuation_string += input_char; // .. token (illegal!)
			input_char = stream.peek();
			if ( input_char == '.' ) {
				input_char = stream.get();
				punctuation_string += input_char; // ... token
			}
		}
		break;
	case '/': // Looking for either / or /=
		input_char = stream.peek();
		if ( input_char == '=' ) {
			input_char = stream.get();
			punctuation_string += input_char; // /= token
		}
		break;
	case ':': // Looking for either : or ::
		input_char = stream.peek();
		if ( input_char == ':' ) {
			input_char = stream.get();
			punctuation_string += input_char; // :: token
		}
		break;
	case '<': // Looking for either < or <=, <<, or <<=
		input_char = stream.peek();
		if ( input_char == '='  ) {
			input_char = stream.get();
			punctuation_string += input_char; // <= token
			break;
		}
		if ( input_char == '<'  ) {
			input_char = stream.get();
			punctuation_string += input_char; // << token
			input_char = stream.peek();
			if ( input_char == '=' ) {
				input_char = stream.get();
				punctuation_string += input_char; // <<= token
			}
		}
		break;
	case '=': // Looking for either = or ==
		input_char = stream.peek();
		if ( input_char == '=' ) {
			input_char = stream.get();
			punctuation_string += input_char; // == token
		}
		break;
	case '>': // Looking for either >, >=, >>, or >>=
		input_char = stream.peek();
		if ( input_char == '='  ) {
			input_char = stream.get();
			punctuation_string += input_char; // >= token
			break;
		}
		if ( input_char == '>'  ) {
			input_char = stream.get();
			punctuation_string += input_char; // >> token
			input_char = stream.peek();
			if ( input_char == '=' ) {
				input_char = stream.get();
				punctuation_string += input_char; // >>= token
			}
		}
		break;
	case '|': // Looking for either |, |=, or || 
		input_char = stream.peek();
		if ( input_char == '|' || input_char == '=' ) {
			input_char = stream.get();
			punctuation_string += input_char; // || or |= token
		}
		break;
	}
	input_char = stream.get();
	return input_char;
}

// print the token to cout
void punctuation_token::print_token() {
	cout << "[\"punctuation\" , \"" << punctuation_string << "\"]" << endl;
}

// parse the whitespace characters
int whitespace_token::parse_token(stringstream& stream, int input_char) {
	while ( true ) {
		input_char = stream.get();
		if ( input_char == ' ' || input_char == 0x09 || input_char == 0x0B || input_char == 0x0D ) {
			continue;
		}
		return input_char;
	}
}

// print the token to cout
void whitespace_token::print_token() {
	cout << "[\"whitespace\" , \" \"]" << endl;
}

// parse the eol character
int eol_token::parse_token(stringstream& stream, int input_char) {
	while ( true ) {
		input_char = stream.get();
		return input_char;
	}
}

// print the token to cout
void eol_token::print_token() {
	cout << "[\"EOL\"]" << endl;
}

// parse the eof character
int eof_token::parse_token(stringstream& stream, int input_char) {
	return 0;
}

// print the token to cout
void eof_token::print_token(void) {
	cout << "[\"EOF\"]" << endl;
}

// parse the invalid character
int invalid_token::parse_token(stringstream& stream, int input_char) {
	invalid_character = input_char;
	input_char = stream.get();
	return input_char;
}

// print the token to cout
void invalid_token::print_token(void) {
	cout << "[\"INVALID\"" << invalid_character << endl;
}

// parse the input source
bool token_lexer::parse_tokens() {
	base_token * token;

	while ( !source_stream.eof() ) {
		int input_char = source_stream.get();

		// Determine what the leading character is of the sequence,
		// create an appropriate token and get the actual token
		// class to parse the rest of it (if any)
		
		while ( !source_stream.eof() ) {
			// The following do loop is there only because I hate seeing
			// if () ... else if () ... else if () ... code!!!
			// Hence it's a do ... while ( false ) - single shot
			do
			{
				// Remove any comments from the source
				if ( input_char == '/' ) {
					int peek_character = source_stream.peek();
					if ( peek_character == '/' ) {
						// Remove the line comment
						while ( peek_character != 0x0A && !source_stream.eof() ) {
							peek_character = source_stream.get();
						}
						token = new(nothrow) eol_token;
						break;
					}
					if ( peek_character == '*' ) {
						// Remove a block comment
						while ( true ) {
							peek_character = source_stream.get();
							if ( peek_character == -1 ) {
								cout << "error: block comment not terminated before EOF" << endl;
								exit(0);
							}
							if ( peek_character == 0x0A ) {
								token = new(nothrow) eol_token;
								// Add the token to the end of the list
								token_list.push_back(token);
								continue;
							}
							if ( peek_character == '*' ) {
								peek_character = source_stream.get();
								if ( peek_character == -1 ) {
									cout << "error: block comment not terminated before EOF" << endl;
									exit(0);
								}
								if ( peek_character == '/' ) {
									// We need to ensure that a whitespace token
									// is created to ensure /* */ in the middle
									// of a source line is processed correctly.
									input_char = source_stream.get();
									input_char = ' ';
									token = new(nothrow) whitespace_token;
									break;
								}
							}
						}
					}
				}
				if ( isalpha(input_char) || input_char == '_' ) {
					// Start of a symbol sequence
					token = new(nothrow) symbol_token;
					break;
				}
				if ( input_char == 0x0A ) {
					// EOL
					token = new(nothrow) eol_token;
					break;
				}
				if ( isspace(input_char) ) {
					// Start of whitespace sequence
					token = new(nothrow) whitespace_token;
					break;
				}
				if ( input_char == '\"' ) {
					// Start of literal sequence
					token = new(nothrow) literal_token;
					break;
				}
				if ( input_char == '\'' ) {
					// Start of constant literal sequence
					token = new(nothrow) const_literal_token;
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
			}
			while ( false );
			if ( token == NULL ) return false;


			input_char = token->parse_token(source_stream, input_char);
			if ( token->get_type() == base_token::t_symbol ) {
				keyword_token * this_token = new(nothrow) keyword_token;
				possible_keyword = static_cast<symbol_token *>(token)->get_symbol();
				if ( this_token->parse_token(source_stream, input_char) == 0 ) {
					delete token;
					token = this_token;
				}
				else
					delete this_token;
			}
			// Add the token to the end of the list
			token_list.push_back(token);
			continue;
		}
	}
	// Add the EOF token to the end of the list
	token = new(nothrow) eof_token;
	token_list.push_back(token);
	return true;
}

// Simply iterate through the list of tokens and print them to cout
// Of course, get the token object to print itself :o)
void token_lexer::print_tokens() {
	list<base_token *>::iterator iterator;
	iterator = token_list.begin();
	while( iterator != token_list.end() ) {
		(*iterator)->print_token();
		++iterator;
	}
}

bool token_lexer::complete_parse() {
	parse_tokens();
	// parser.tokenize();
	return true;
}
