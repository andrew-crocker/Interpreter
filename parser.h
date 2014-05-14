#ifndef _Parser_
#define _Parser_

class line;

// All tokens must derive from this token type

class base_token {
public:
	typedef enum { 	t_invalid_token=0, t_symbol, t_integer, t_literal, 
					t_punctuation, t_keyword
	             } type_of_token;
private:
	type_of_token token_type;
public:
	base_token(type_of_token token) : token_type(token) { };
	virtual ~base_token() { };
	type_of_token get_type() { return token_type; };
	virtual const char *  parse_token(const char * s) = 0;
};

// A token that may contain a symbol

class symbol_token : public base_token {
	string symbol;
public:
	symbol_token() : base_token(t_symbol) { };
	const char * parse_token(const char * s);
	const char * get_symbol() { return symbol.c_str(); };
};

// A token that will contain a keyword

class keyword_token : public base_token {
	typedef enum { 	t_let=0, t_for, t_to, t_step, t_next, t_dim,
					t_if, t_then, t_else, t_endif, t_read,
					t_data, t_end, t_function, t_return, t_program,
					t_input, t_print, t_int, t_left_dollar,
					t_right_dollar, t_mid_dollar, t_chr, t_asc, 
					t_procedure, t_end_procedure, t_randomize, t_rem,
					t_rnd 
	     		 }  type_of_keyword;
	static char const * keywords[];
	type_of_keyword keyword;
public:
	keyword_token() : base_token(t_keyword) { };
	const char * parse_token(const char * s);
};

// A token that represents an integer

class integer_token : public base_token {
	string integer_string;
public:
	integer_token() : base_token(t_integer) { };
	const char * parse_token(const char * s);
};

// A token that represents a literal

class literal_token : public base_token {
	string literal_string;
public:
	literal_token() : base_token(t_literal) { };
	const char * parse_token(const char * s);
};

// A token that represents a punctuation or separator

class punctuation_token : public base_token {
	string punctuation_string;
public:
	punctuation_token() : base_token(t_punctuation) { };
	const char * parse_token(const char * s);
};

// The BASIC token line

class line_parser {
public:
	bool parse_string(line * source_line);
};

#endif

