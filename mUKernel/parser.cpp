#include "common.h"
#include <mU/Exceptions.h>
#include <mU/Parser.h>

namespace mU {
//////////////////////////////////////
DLL std::vector<parser::char_t> parser::s_char;
DLL stdext::hash_map<wint_t,size_t> parser::s_unicode;
DLL stdext::hash_map<wstring,size_t> parser::s_name;
DLL size_t parser::s_prec = 1;
DLL std::vector<parser::oper_t> parser::s_oper;
DLL std::map<parser::token_t,size_t> parser::s_postfix_token, parser::s_prefix_token, parser::s_infix_token;
DLL stdext::hash_map<Var,size_t> parser::s_postfix_symbol, parser::s_prefix_symbol, parser::s_infix_symbol;
DLL std::set<parser::token_t> parser::s_end;
DLL size_t parser::operPLUS, parser::operSTAR;
DLL int parser::precInequality;
DLL std::set<parser::token_t> parser::s_inequality;

#define DEF_OPER(...) s_oper.push_back(oper_t(__VA_ARGS__));

void parser::init()
{
	static bool inited = false;
	if(inited) return;

	DEF_OPER(SEMICOLON,_W("CompoundExpression"),_W(";"))
	++s_prec;
	DEF_OPER(QUESTIONMARK,L"Usage",L"?",false,true)
	DEF_OPER(QUESTIONMARK_QUESTIONMARK,L"Information",L"??",false,true)
	DEF_OPER(LEFT_OP,L"Get",L"<<",false,true)
	DEF_OPER(RIGHT_OP,L"Put",L">>")
	DEF_OPER(RIGHT_GT,L"PutAppend",L">>>")
	++s_prec;
	DEF_OPER(ASSIGN,L"Set",L"=")
	DEF_OPER(COLON_ASSIGN,L"SetDelayed",L":=")
	DEF_OPER(POW_ASSIGN,L"UpSet",L"^=")
	DEF_OPER(POW_COLON_ASSIGN,L"UpSetDelayed",L"^:=")
	DEF_OPER(ASSIGN_PERIOD,L"Unset",L"=.",true)
	++s_prec;
	DEF_OPER(SLASH_SLASH,L"Postfix",L"//")
	++s_prec;
	DEF_OPER(AND_OP,L"Function",L"&",true)
	++s_prec;
	DEF_OPER(ADD_ASSIGN,L"AddTo",L"+=")
	DEF_OPER(SUB_ASSIGN,L"SubtractFrom",L"-=")
	DEF_OPER(MUL_ASSIGN,L"TimesBy",L"*=")
	DEF_OPER(DIV_ASSIGN,L"DivideBy",L"/=")
	DEF_OPER(MOD_ASSIGN,L"ModBy",L"%=")
	++s_prec;
	DEF_OPER(ASSIGN_GT,L"Function",L"=>")
	++s_prec;
	DEF_OPER(SLASH_PERIOD,L"ReplaceAll",L"/.")
	DEF_OPER(SLASH_SLASH_PERIOD,L"ReplaceRepeated",L"//.")
	++s_prec;
	DEF_OPER(PTR_OP,L"Rule",L"->")
	DEF_OPER(COLON_GT,L"RuleDelayed",L":>")
	++s_prec;
	DEF_OPER(SLASH_SEMICOLON,L"Condition",L"/;")
	++s_prec;
	DEF_OPER(TILDE_TILDE,L"StringExpression",L"~~")
	++s_prec;
	DEF_OPER(COLON,L"Optional",L":")
	++s_prec;
	DEF_OPER(OR_OP,L"Alternatives",L"|")
	++s_prec;
	DEF_OPER(PERIOD_PERIOD,L"Repeated",L"..",true)
	++s_prec;
	DEF_OPER(BOOL_OR_OP,L"Or",L"||")
	++s_prec;
	DEF_OPER(BOOL_AND_OP,L"And",L"&&")
	++s_prec;
	DEF_OPER(QUOTE,L"Unevaluated",L"'",false,true)
	++s_prec;
	DEF_OPER(BANG,L"Not",L"!",false,true)
	++s_prec;
	DEF_OPER(ASSIGN_EQ,L"SameQ",L"===")
	DEF_OPER(ASSIGN_NE,L"UnsameQ",L"=!=")
	++s_prec;
	precInequality = s_prec;
	s_inequality.insert(EQ_OP);
	s_inequality.insert(LE_OP);
	s_inequality.insert(GE_OP);
	s_inequality.insert(NE_OP);
	s_inequality.insert(LT_OP);
	s_inequality.insert(GT_OP);
	DEF_OPER(EQ_OP,L"Equal",L"==")
	DEF_OPER(LE_OP,L"LessEqual",L"<=")
	DEF_OPER(GE_OP,L"GreaterEqual",L">=")
	DEF_OPER(NE_OP,L"Unequal",L"!=")
	DEF_OPER(LT_OP,L"Less",L"<")
	DEF_OPER(GT_OP,L"Greater",L">")
	++s_prec;
	operPLUS = s_oper.size();
	DEF_OPER(PLUS,L"Plus",L"+")
	++s_prec;
	operSTAR = s_oper.size();
	DEF_OPER(STAR,L"Times",L"*")
	++s_prec;
	DEF_OPER(SLASH,L"Divide",L"/",false,false,true)
	++s_prec;
	DEF_OPER(PERCENT,L"Mod",L"%")
	++s_prec;
	DEF_OPER(PLUS,L"Plus",L"+",false,true)
	DEF_OPER(MINUS,L"Minus",L"-",false,true)
	++s_prec;
	DEF_OPER(PERIOD,L"Dot",L".")
	++s_prec;
	DEF_OPER(CIRCUMFLEX,L"Power",L"^")
	++s_prec;
	DEF_OPER(LT_GT,L"StringJoin",L"<>")
	++s_prec;
	DEF_OPER(TILDE,L"Join",L"~")
	++s_prec;
	DEF_OPER(QUOTE,L"Differential",L"'",true)
	++s_prec;
	DEF_OPER(BANG,L"Factorial",L"!",true)
	++s_prec;
	DEF_OPER(SLASH_AT,L"Map",L"/@")
	DEF_OPER(SLASH_SLASH_AT,L"MapAll",L"//@")
	DEF_OPER(AT_AT,L"Apply",L"@@")
	++s_prec;
	DEF_OPER(AT,L"Prefix",L"@")
	++s_prec;
	DEF_OPER(INC_OP,L"PreIncrement",L"++",false,true)
	DEF_OPER(DEC_OP,L"PreDecrement",L"--",false,true)
	++s_prec;
	DEF_OPER(INC_OP,L"Increment",L"++",true)
	DEF_OPER(DEC_OP,L"Decrement",L"--",true)
	++s_prec;
	DEF_OPER(QUESTIONMARK,L"PatternTest",L"?")
	++s_prec;

	for(size_t i = 0; i < s_oper.size(); ++i)
	{
		if(s_oper[i].postfix)
		{
			s_postfix_token[s_oper[i].token] = i;
			s_postfix_symbol[s_oper[i].symbol] = i;
		}
		else if(s_oper[i].prefix)
		{
			s_prefix_token[s_oper[i].token] = i;
			s_prefix_symbol[s_oper[i].symbol] = i;
		}
		else
		{
			s_infix_token[s_oper[i].token] = i;
			s_infix_symbol[s_oper[i].symbol] = i;
		}
	}
	s_end.insert(EOI);
	s_end.insert(COMMA);
	s_end.insert(SEMICOLON);
	s_end.insert(RBRACKET);
	s_end.insert(RPAREN);
	s_end.insert(RBRACE);
#include "character.h"
	for(size_t i = 0; i < s_char.size(); ++i)
	{
		s_unicode[s_char[i].unicode] = i;
		s_name[s_char[i].name] = i;
	}
	inited = true;
}
parser::parser() { init(); }
parser::parser(wistream &x) { init(); start(x); }
void parser::start(wistream &x) { m_in = &x; clear(); }
void parser::parse()
{
	if(lookahead == EOI)
		error();
	gen(expression());
	if(!linebreak && lookahead != EOI)
		error();
}
var parser::result() { 
	if (m_code.empty())
		throw mU::LogicError(_W("no parse result\n"));
	return m_code.back(); 
}
void parser::clear()
{
	m_child.clear();
	m_node.clear();
	m_note.clear();
	m_code.clear();
	lineno = 1;
	column = 1;
	node(tag_invalid,0);
	accept();
}
const wchar *parser::character_name(wchar x)
{
	stdext::hash_map<wint_t,size_t>::const_iterator
		iter = s_unicode.find(static_cast<wint_t>(x));
	if(iter != s_unicode.end())
		return s_char[iter->second].name;
	else
		return 0;
}

const parser::oper_t *parser::lookup_postfix_oper(Var s)
{
	stdext::hash_map<Var,size_t>::const_iterator iter = s_postfix_symbol.find(s);
	if (iter != s_postfix_symbol.end()) {
		return &s_oper[iter->second];
	}

	return 0;
}

const parser::oper_t *parser::lookup_prefix_oper(Var s)
{
	stdext::hash_map<Var,size_t>::const_iterator iter = s_prefix_symbol.find(s);
	if (iter != s_prefix_symbol.end()) {
		return &s_oper[iter->second];
	}

	return 0;
}

const parser::oper_t *parser::lookup_infix_oper(Var s)
{
	stdext::hash_map<Var,size_t>::const_iterator iter = s_infix_symbol.find(s);
	if (iter != s_infix_symbol.end()) {
		return &s_oper[iter->second];
	}

	return 0;
}

size_t parser::node(tag_t t,int v)
{
	node_t r;
	r.tag = t; r.value = v;
	m_node.push_back(r);
	return m_node.size() - 1;
}
void parser::insert(size_t i, size_t j)
{
	m_child.insert(std::make_pair(i,j));
}
const wchar* parser::entry(const wstring &s)
{
	return m_entry.insert(s).first->c_str();
}
void parser::note(size_t i, const wstring &s)
{
	m_note.insert(std::make_pair(i,entry(s)));
}
void parser::error()
{
	throw mU::UnexpectedTokenException();
}
void parser::more()
{
	throw mU::MoreInputNeededException();
}
void parser::accept()
{
	lookback = lookahead;
	lookahead = scan();
	if(lookahead == NEWLINE)
	{
		linebreak = skip = true;
		lookahead = scan();
	}
	else
	{
        linebreak = false;
        if(lookahead == SPACE)
        {
            skip = true;
            lookahead = scan();
        }
        else
            skip = false;
	}
}
void parser::match(token_t t)
{
	if(lookahead == t) { accept(); return; }
	lookahead == EOI ? more() : error();
}
/*
expression
: [prefix] OP (prefix | primary)
| [infix] (primary | postfix | suffix) OP (prefix | primary)
| [primary] (# | prefix | infix) OP
| [postfix] (primary | postfix | suffix) OP
| [suffix] (primary | postfix) OP
;
*/
void parser::shift(frame_t &s)
{
	std::map<token_t,size_t>::const_iterator iter;
	switch(s.mode)
	{
	case mode_start://#.
	case mode_prefix://prefix.
	case mode_infix://infix.
	//[prefix | primary]
		iter = s_prefix_token.find(lookahead);
		if (iter != s_prefix_token.end()) {
			if (s.prec < 0 && s_oper[iter->second].prec >= -s.prec)
				s.prec = 0;
			accept();
			s.op = iter->second;
			s.mode = mode_prefix;
			return;
		}
		if (s.prec < 0) {
			if (-s.prec == precInequality)
				s.op = inequality();
			else
				s.op = primary();
			s.mode = mode_primary;
			return;
		}
		s.op = primary();
		s.mode = mode_primary;
		return;
	case mode_primary://primary.
	case mode_postfix://postfix.
	//[suffix]
		if (lookahead == LBRACKET) {
			s.op = suffix();
			s.mode = mode_suffix;
			return;
		}
	case mode_suffix://suffix.
	//[infix | postfix]
		if (linebreak || s_end.count(lookahead)) {
			s.mode = mode_end;
			return;
		}
		iter = s_infix_token.find(lookahead);
		if (iter != s_infix_token.end()) {
			if (s.prec > 0 && s_oper[iter->second].prec <= s.prec) {
				s.mode = mode_end;
				return;
			}
			accept();
			s.op = iter->second;
			s.mode = mode_infix;
			return;
		}
		iter = s_postfix_token.find(lookahead);
		if (iter != s_postfix_token.end()) {
			accept();
			s.op = iter->second;
			s.mode = mode_postfix;
			return;
		}
		//MINUS -> PLUS MINUS
		if (lookahead == MINUS) {
			s.op = operPLUS;
			s.mode = mode_infix;
			return;
		}
		//SPACE -> STAR
		s.op = operSTAR;
		s.mode = mode_infix;
		return;
	}
}
void parser::reduce(frame_t &s)
{
	size_t r[3];
	switch(s.mode)
	{
	case mode_infix:case mode_postfix:case mode_end:
		while(!oper_stack.empty() && oper_stack.size() != s.offset) {
			size_t oper = oper_stack.top();
			if (s_oper[oper].postfix) {
				r[0] = node(tag_expression,oper);
				r[1] = oprn_stack.top(); oprn_stack.pop();
				insert(r[0],r[1]);
			}
			else if (compare(s)) {
				r[0] = node(tag_expression,oper);
				if(s_oper[oper].prefix)
				{
					r[1] = oprn_stack.top(); oprn_stack.pop();
					insert(r[0],r[1]);
				}
				else
				{
					r[1] = oprn_stack.top(); oprn_stack.pop();
					r[2] = oprn_stack.top(); oprn_stack.pop();
					insert(r[0],r[2]);
					insert(r[0],r[1]);
				}
			}
			else
				break;
			oper_stack.pop();
			oprn_stack.push(r[0]);
		}
		return;
	case mode_suffix:
		{
			r[0] = oprn_stack.top(); oprn_stack.pop();
			while(!oper_stack.empty() && s_oper[oper_stack.top()].postfix)
			{
				r[1] = node(tag_expression,oper_stack.top());
				oper_stack.pop();
				insert(r[1],r[0]);
				r[0] = r[1];
			}
			insert(s.op,r[0]);
		}
		return;
	}
}
bool parser::compare(frame_t &s)
{
	if(s.mode == mode_end) return true;
	if(s_oper[s.op].prec < s_oper[oper_stack.top()].prec) return true;
	if(s_oper[s.op].prec == s_oper[oper_stack.top()].prec) return s_oper[s.op].rassoc;
	return false;
}
size_t parser::expression(int prec)
{
	size_t r[1];
	frame_t s(oper_stack.size(), mode_start, prec);
	while(s.mode != mode_end)
	{
		shift(s);
		reduce(s);
		switch(s.mode)
		{
		case mode_prefix:case mode_infix:case mode_postfix: oper_stack.push(s.op); break;
		case mode_primary:case mode_suffix: oprn_stack.push(s.op);	break;
		}
	}
	r[0] = oprn_stack.top(); oprn_stack.pop();
	return r[0];
}

/*
inequality
: >precInequality (INEQUALITY >precInequality)*
;
*/
size_t parser::inequality() {
	size_t r[2];
	r[1] = expression(precInequality);
	if (s_inequality.count(lookahead)) {
		r[0] = node(tag_inequality, -1);
		insert(r[0],r[1]);
		do {
			r[1] = node(tag_primary,instr_symbol);
			note(r[1],s_oper[s_infix_token[lookahead]].name);
			accept();
			insert(r[0],r[1]);
			insert(r[0],expression(precInequality));
		} while(s_inequality.count(lookahead));
		return r[0];
	}
	return r[1];
}

/*
expression(-precInequality)
: (primary -> inequality)
;
expression
: expression(-precInequality) (SEMICOLON expression(-precInequality))*
;
*/
size_t parser::expression() {
	size_t r[2];
	r[1] = expression(-precInequality);
	if (lookahead == SEMICOLON) {
		r[0] = node(tag_expression,-1);
		insert(r[0],r[1]);
		do {
			accept();
			r[1] = expression(-precInequality);
			insert(r[0],r[1]);
		} while (lookahead == SEMICOLON);
		return r[0];
	}
	return r[1];
}

/*
suffix
: ( LBRACKET LBRACKET sequence? RBRACKET RBRACKET
| LBRACKET sequence? RBRACKET
)*
;
*/
size_t parser::suffix()
{
	size_t r[3];
	r[0] = node(tag_suffix,-1);
	while(lookahead == LBRACKET)
	{
		accept();
		if(lookahead == LBRACKET)
		{
			accept();
			if(lookahead == RBRACKET)
			{
				accept();
				match(RBRACKET);
				r[1] = node(tag_suffix,-2);
				insert(r[0],r[1]);
				continue;
			}
			else
			{
				r[1] = sequence();
				match(RBRACKET);
				match(RBRACKET);
				r[2] = node(tag_suffix,-3);
				insert(r[2],r[1]);
				insert(r[0],r[2]);
				continue;
			}
		}
		if(lookahead == RBRACKET)
		{
			accept();
			r[1] = node(tag_suffix,-4);
			insert(r[0],r[1]);
			continue;
		}
		r[1] = sequence();
		match(RBRACKET);
		r[2] = node(tag_suffix,-5);
		insert(r[2],r[1]);
		insert(r[0],r[2]);
		continue;
	}
	return r[0];
}

/*
sequence
: [sequence] expression (COMMA expression)*
;
*/
size_t parser::sequence()
{
	size_t r[2];
	r[0] = node(tag_sequence,-1);
	r[1] = expression();
	insert(r[0],r[1]);
	while (lookahead == COMMA) {
		accept();
		r[1] = expression();
		insert(r[0],r[1]);
	}
	return r[0];
}

/*
primary
: LBRACE sequence? RBRACE
| LPAREN expression RPAREN
| (UNDERLINE | UNDERLINE_UNDERLINE) (IDENTIFIER | PERIOD)?
| (POUND | POUND_POUND) INTEGER?
| INTEGER
| REAL
| STRING
| IDENTIFIER (UNDERLINE | UNDERLINE_UNDERLINE) (IDENTIFIER | PERIOD)?
| IDENTIFIER (COLON expression)?
| IDENTIFIER
;
*/
size_t parser::primary()
{
	size_t r[3];
	int value = 0;
	switch(lookahead)
	{
	case LBRACE:
		accept();
		if(lookahead == RBRACE)
		{
			accept();
			r[0] = node(tag_primary,-1);
		}
		else
		{
			r[0] = node(tag_primary,-2);
			r[1] = sequence();
			match(RBRACE);
			insert(r[0],r[1]);
		}
		return r[0];
	case LPAREN:
		accept();
		r[0] = expression();
		match(RPAREN);
		r[1] = node(tag_primary,-3);
		insert(r[1],r[0]);
		return r[1];
	case UNDERLINE:
		if(value == 0) value = -4;
	case UNDERLINE_UNDERLINE:
		if(value == 0) value = -7;
		accept();
		if(!skip)
		{
			if(lookahead == IDENTIFIER)
			{
				r[0] = node(tag_primary,value);
				r[1] = node(tag_primary,instr_symbol);
				note(r[1],text);
				accept();
				insert(r[0],r[1]);
				return r[0];
			}
			if(lookahead == PERIOD)
			{
				accept();
				return node(tag_primary,value - 1);
			}
		}
		return node(tag_primary,value - 2);
	case POUND:
		if(value == 0) value = -10;
	case POUND_POUND:
		if(value == 0) value = -12;
		accept();
		if(lookahead == INTEGER)
		{
			r[0] = node(tag_primary,value);
			r[1] = node(tag_primary,instr_integer);
			note(r[1],text);
			accept();
			insert(r[0],r[1]);
			return r[0];
		}
		return node(tag_primary,value - 1);
	case INTEGER:
		r[0] = node(tag_primary,instr_integer);
		note(r[0],text);
		accept();
		return r[0];
	case REAL:
		r[0] = node(tag_primary,instr_float);
		note(r[0],text);
		accept();
		return r[0];
	case STRING:
		r[0] = node(tag_primary,instr_string);
		note(r[0],text);
		accept();
		return r[0];
	case IDENTIFIER:
		r[0] = node(tag_primary,instr_symbol);
		note(r[0],text);
		accept();
		if(!skip)
		{
			switch(lookahead)
			{
			case UNDERLINE:
				if(value == 0) value = -14;
			case UNDERLINE_UNDERLINE:
				if(value == 0) value = -17;
				accept();
				if(!skip)
				{
					if(lookahead == IDENTIFIER)
					{
						r[1] = node(tag_primary,value);
						r[2] = node(tag_primary,instr_symbol);
						note(r[2],text);
						accept();
						insert(r[1],r[0]);
						insert(r[1],r[2]);
						return r[1];
					}
					if(lookahead == PERIOD)
					{
						accept();
						r[1] = node(tag_primary,value - 1);
						insert(r[1],r[0]);
						return r[1];
					}
				}
				r[1] = node(tag_primary,value - 2);
				insert(r[1],r[0]);
				return r[1];
			case COLON:
				accept();
				r[1] = expression();
				r[2] = node(tag_primary,-20);
				insert(r[2],r[0]);
				insert(r[2],r[1]);
				return r[2];
			case COLON_COLON:
				accept();
				if(lookahead == IDENTIFIER)
				{
					r[1] = node(tag_primary,-21);
					r[2] = node(tag_primary,instr_symbol);
					note(r[2],text);
					accept();
					insert(r[1],r[0]);
					insert(r[1],r[2]);
					return r[1];
				}
				else
					error();
				break;
			}
		}
		return r[0];
	}
	//SPACE -> L"Null"
	if(lookback == COMMA || lookback == SEMICOLON)
	{
		r[0] = node(tag_primary,instr_symbol);
		note(r[0],L"Null");
		return r[0];
	}
	lookahead == EOI ? more() : error();
}
//////////////////////////////////////
}
