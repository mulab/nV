#pragma once
#include "Var.h"

namespace mU {
//////////////////////////////////////
class parser
{
public:
	DLL parser();
	DLL parser(wistream&);
	DLL void start(wistream&);
	DLL void parse();
	DLL var result();
	DLL void clear();
	DLL bool has_more() { return lookahead != EOI; }

	// FIXME; wrap these two data members with getters
	size_t lineno;
	size_t column;

	DLL static const wchar *character_name(wchar x);

	enum token_t {
		IDENTIFIER, INTEGER, REAL, STRING, PTR_OP, INC_OP, DEC_OP, LEFT_OP, RIGHT_OP,
		RIGHT_GT, LE_OP, GE_OP, EQ_OP, NE_OP, BOOL_AND_OP, BOOL_OR_OP, ADD_ASSIGN, SUB_ASSIGN,
		MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,	LPAREN,	RPAREN,	LBRACKET, RBRACKET,	LBRACE,	RBRACE,
		PERIOD,	COMMA, COLON, SEMICOLON, QUESTIONMARK, PLUS, MINUS, STAR, SLASH, ASSIGN, AND_OP,
		OR_OP, BANG, TILDE, TILDE_TILDE, PERCENT, CIRCUMFLEX, GT_OP, LT_OP, COLON_ASSIGN, POW_ASSIGN,
		POW_COLON_ASSIGN, COLON_GT, SLASH_PERIOD, COLON_COLON, UNDERLINE, UNDERLINE_UNDERLINE,
		PERIOD_PERIOD, SLASH_SEMICOLON, POUND, AT, SLASH_SLASH, AT_AT, ASSIGN_EQ, ASSIGN_PERIOD,
		SLASH_SLASH_PERIOD, ASSIGN_NE, QUOTE, SLASH_AT, SLASH_SLASH_AT, SPACE, POUND_POUND,
		LT_GT, ASSIGN_GT, NEWLINE, EOI = -1
	};

	struct oper_t
	{
		token_t token;
		const wchar *name;
		const wchar *show;
		bool postfix;
		bool prefix;
		bool rassoc;
		size_t prec;
		Var symbol;
		oper_t(token_t _token,const wchar *_name,
			const wchar *_show, bool _postfix = false,
			bool _prefix = false,bool _rassoc = false) : 
		token(_token),name(_name),
			show(_show),postfix(_postfix),
			prefix(_prefix),rassoc(_rassoc),
			prec(s_prec),symbol(Sym(name, System)) {}
	};
	DLL static const oper_t *lookup_oper(Var);
	DLL static size_t max_prec() { return s_prec; }

private:
	enum tag_t {
		tag_expression, tag_suffix, tag_inequality, tag_sequence,
		tag_primary, tag_invalid = -1
	};
	enum instr_t {
		instr_expression, instr_symbol, instr_integer,
		instr_float, instr_string, instr_null = -1
	};
	struct char_t
	{
		wint_t unicode;
		const wchar *name;
		Var symbol;
		char_t(size_t _unicode,const wchar *_name) : 
		unicode(_unicode),name(_name),symbol(Sym(name, System)) {}
	};
	DLL static std::vector<char_t> s_char;
	DLL static stdext::hash_map<wint_t,size_t> s_unicode;
	DLL static stdext::hash_map<wstring,size_t> s_name;
	DLL static size_t s_prec;

	DLL static std::vector<oper_t> s_oper;
	DLL static std::map<token_t,size_t> s_postfix_token, s_prefix_token, s_infix_token;
	DLL static stdext::hash_map<Var,size_t> s_oper_symbol;
	DLL static std::set<token_t> s_end;
	DLL static size_t operPLUS, operSTAR;
	DLL static int precInequality;
	DLL static std::set<token_t> s_inequality;
	DLL static void init();

	wstring text;
	token_t lookahead;
	token_t lookback;
	bool skip;
	bool linebreak;
private:
	wistream *m_in;
	void error();
	void more();

	std::stack<wchar> char_stack;
	wchar read(bool in_string_literal=false);
	token_t scan();
		
	struct node_t
	{
		tag_t tag;
		int value;
	};
	std::vector<node_t> m_node;
	std::multimap<size_t,size_t> m_child;
	stdext::hash_set<wstring> m_entry;
	std::map<size_t,const wchar*> m_note;
	std::stack<size_t> oper_stack;
	std::stack<size_t> oprn_stack;
	enum mode_t {
		mode_start, mode_prefix, mode_infix, mode_primary,
		mode_postfix, mode_suffix, mode_end = -1
	};
	struct frame_t
	{
		size_t offset;
		mode_t mode;
		size_t op;
		int prec;

		frame_t(size_t o,mode_t m,int p = 0) :
		offset(o),mode(m),op(0),prec(p) {}
	};
	size_t node(tag_t t,int v);
	void insert(size_t i, size_t j);
	const wchar* entry(const wstring &s);
	void note(size_t i, const wstring &s);
	void shift(frame_t &s);
	void reduce(frame_t &s);
	bool compare(frame_t &s);
	void accept();
	void match(token_t t);
	size_t expression(int);
	size_t inequality();
	size_t expression();
	size_t suffix();
	size_t sequence();
	size_t primary();

	std::vector<var> m_code;
	std::stack<std::pair<Var,size_t> > code_stack;
	typedef std::multimap<size_t,size_t>::const_iterator iter_t;
	iter_t lower(size_t i);
	iter_t upper(size_t i);
	size_t count(size_t i);
	void emit(Var);
	void head(size_t n);
	void gen(size_t m);
};
//////////////////////////////////////
}