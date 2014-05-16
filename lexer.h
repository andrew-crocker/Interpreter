#include <sstream>

#ifndef _Lexer_
#define _Lexer_

#pragma once

// All tokens must derive from this token type

class base_token {
public:
	typedef enum {
		t_invalid_token=0, t_symbol,
		t_integer, t_keyword, t_literal, 
		t_const_literal, t_punctuation,
		t_whitespace, t_eol, t_eof
	} type_of_token;
private:
	type_of_token token_type;
public:
	base_token(type_of_token token) : token_type(token) { };
	virtual ~base_token() { };
	type_of_token get_type() { return token_type; };
	virtual int parse_token(stringstream& stream, int input_char) = 0;
	virtual void print_token() = 0;
};

// A token that may contain a symbol

class symbol_token : public base_token {
	string symbol;
public:
	symbol_token() : base_token(t_symbol) { };
	int parse_token(stringstream& stream, int input_char);
	const char * get_symbol() { return symbol.c_str(); };
	void print_token();
};

// A token that represents a reserved word

class keyword_token : public base_token {
	typedef enum { 
		t_and=0, t_and_eq, t_asm, t_auto, t_bitand, t_bitor,
		t_bool, t_break, t_case, t_catch, t_char, t_class, t_const,
		t_const_cast, t_continue, t_default, t_delete, t_do,
		t_double, t_dynamic_cast, t_else, t_enum, t_explicit,
		t_export, t_extern, t_false, t_float, t_for, t_friend,
		t_goto, t_if, t_inline, t_int, t_long, t_mutable,
		t_namespace, t_new, t_not, t_not_eq, t_operator, t_or,
		t_or_eq, t_private, t_protected, t_public, t_register,
		t_reinterpret_cast, t_return, t_short, t_signed,
		t_sizeof, t_static, t_static_cast, t_struct, t_switch,
		t_template, t_this, t_throw, t_true, t_try, t_typedef,
		t_typeid, t_typename, t_union, t_unsigned, t_using,
		t_virtual, t_void, t_volatile, t_wchar_t, t_while,
		t_xor, t_xor_eq
	} type_of_keyword;
	static char const * keywords[];
	type_of_keyword keyword;
public:
	keyword_token() : base_token(t_keyword) { };
	int parse_token(stringstream& stream, int input_char);
	void print_token();
};

// A token that represents an integer

class integer_token : public base_token {
	string integer_string;
public:
	integer_token() : base_token(t_integer) { };
	int parse_token(stringstream& stream, int input_char);
	void print_token();
};

// A token that represents a literal

class literal_token : public base_token {
	string literal_string;
public:
	literal_token() : base_token(t_literal) { };
	int parse_token(stringstream& stream, int input_char);
	void print_token();
};

// A token that represents a constant literal (e.g. 'A')

class const_literal_token : public base_token {
	string const_literal_string;
public:
	const_literal_token() : base_token(t_const_literal) { };
	int parse_token(stringstream& stream, int input_char);
	void print_token();
};

// A token that represents a punctuation or separator

class punctuation_token : public base_token {
	string punctuation_string;
public:
	punctuation_token() : base_token(t_punctuation) { };
	int parse_token(stringstream& stream, int input_char);
	void print_token();
};

// A token that represents whitespace

class whitespace_token : public base_token {
public:
	whitespace_token() : base_token(t_whitespace) { };
	int parse_token(stringstream& stream, int input_char);
	void print_token();
};

// A token that represents an eol

class eol_token : public base_token {
public:
	eol_token() : base_token(t_eol) { };
	int parse_token(stringstream& stream, int input_char);
	void print_token();
};

// A token that represents an eof

class eof_token : public base_token {
public:
	eof_token() : base_token(t_eof) { };
	int parse_token(stringstream& stream, int input_char);
	void print_token();
};

// A token that represents an illegal character

class invalid_token : public base_token {
	int invalid_character;
public:
	invalid_token() : base_token(t_invalid_token) { };
	int parse_token(stringstream& stream, int input_char);
	void print_token();
};

// The C++ token lexer

class token_lexer {
	friend class token_parser;
	stringstream& source_stream;
	list<base_token *> token_list;
public:
	token_lexer(stringstream& stream) : source_stream(stream) { };
	bool parse_tokens();
	void print_tokens();
	bool complete_parse();
};

#endif
