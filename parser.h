#include <vector>
#include <string>
#include <vector>
using namespace std;
#include "lexer.h"

#ifndef _Parser_
#define _Parser_

#pragma once

class token_parser {
	list<base_token *> base_tokens;
	list<base_token *>::iterator iterator;
	// list<complete_token *> tokens;
public:
	token_parser(token_lexer *);
	base_token * current_token();
	base_token * next_token();
	base_token * previous_token();
	void print_tokens();
	bool complete_parse();
};

#endif // _Parser_