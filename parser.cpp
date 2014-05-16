#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cctype>

using namespace std;

#include "lexer.h"
#include "parser.h"

token_parser::token_parser(token_lexer * tl) {
	base_tokens = tl->token_list;
	iterator = base_tokens.begin();
	// print_tokens();
}

base_token * token_parser::current_token() {
	return *(iterator++);
}

base_token * token_parser::next_token() {
	return *(++iterator);
}

base_token * token_parser::previous_token() {
	return *(--iterator);
}

void token_parser::print_tokens() {
	list<base_token *>::iterator iterator;
	iterator = base_tokens.begin();
	while( iterator != base_tokens.end() ) {
		(*iterator)->print_token();
		++iterator;
	}
}

bool token_parser::complete_parse() {
	while (iterator != base_tokens.end()) {
		switch(current_token()->get_type()) {
			case 0: { // t_invalid_token
				// cout << "Invalid token" << endl;
				
				break;
			}
			case 1: { // t_symbol
				// cout << "Symbol" << endl;
				
				break;
			}
			case 2: { // t_integer
				// cout << "Integer" << endl;
				
				break;
			}
			case 3: { // t_keyword
				// cout << "Keyword" << endl;
				
				break;
			}
			case 4: { // t_literal
				// cout << "Literal" << endl;
				
				break;
			}
			case 5: { // t_const_literal
				// cout << "Const Literal" << endl;
				
				break;
			}
			case 6: { // t_punctuation
				// cout << "Punctuation" << endl;
				
				break;
			}
			case 7: { // t_whitespace
				// cout << "Whitespace" << endl;
				
				break;
			}
			case 8: { // t_eol
				// cout << "EOL" << endl;
				
				break;
			}
			case 9: { // t_eof
				// cout << "EOF" << endl;
				
				break;
			}
			default: {
				// cout << "What the heck is this?" << endl;
				
				break;
			}
		}
	}
	// current_token()->print_token();
	// current_token()->print_token();
	// current_token()->print_token();
	return true;
}