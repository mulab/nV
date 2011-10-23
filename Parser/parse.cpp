#include <nV/Parser.h>
#include <stdexcept>

namespace nV {
void Parser::start(wistream& in) {
	start();
	if (!parsing)
		parsing = new Parsing;
    parsing->start(in,mNode,mNote);
    parsing->accept();
}
uint Parser::parse() {
    if (parsing->mLookahead == EOI)
        parsing->error();
    uint r = parsing->expression();
    if (!parsing->mLinebreak && parsing->mLookahead != EOI)
        parsing->error();
    return r;
}
uint Parser::Parsing::node(Tag t, int v) {
    Node r;
    r.tag = t;
    r.value = v;

	r.treenode.index = pNode->size();

    pNode->push_back(r);
    return pNode->size() - 1;
}
void Parser::Parsing::note(uint n, const wstring& s) {
	pNote->push_back(wstr(s));
	(*pNode)[n].note = pNote->size() - 1;
}
void Parser::Parsing::insert(uint i, uint j) {
	(*pNode)[j].treenode.rightSibling = (*pNode)[i].treenode.firstChild;
	(*pNode)[i].treenode.firstChild = j;
	(*pNode)[i].treenode.nChild++;

}
void Parser::Parsing::error() {
    throw std::logic_error("error");
}
void Parser::Parsing::more() {
    throw std::logic_error("more");
}
void Parser::Parsing::accept(bool skipping) {
    mLookback = mLookahead;
    mLookahead = scan();
    if (skipping)
		skip();
}
void Parser::Parsing::skip() {
    if (mLookahead == NEWLINE) {
        mLinebreak = mSkip = true;
        mLookahead = scan();
    } else {
        mLinebreak = false;
        if (mLookahead == SPACE) {
            mSkip = true;
            mLookahead = scan();
        } else
            mSkip = false;
    }
}
void Parser::Parsing::match(Token t, bool skipping) {
    if (mLookahead == t) {
        accept(skipping);
        return;
    }
    mLookahead == EOI ? more() : error();
}

/*
sequence
: [sequence] expression (DELIMITER expression)*
;
*/
uint Parser::Parsing::sequence(Token t) {
    uint r[2];
	r[0] = node(tag_sequence, -1);
    r[1] = expression();
	insert(r[0], r[1]);
	while (mLookahead == t) {
		accept();
		r[1] = expression();
		insert(r[0], r[1]);
	}
	return r[0];
}

/*
expression
: [prefix] PREFIX (prefix | primary)
| [infix] (primary | postfix | suffix) INFIX                                                                                
| [primary] (# | prefix | infix) PRIMARY
| [postfix] (primary | postfix | suffix) POSTFIX
| [suffix] (primary | postfix) SUFFIX
;
*/
void Parser::Parsing::shift(Frame &s) {
    std::unordered_map<Token, uint>::const_iterator iter;
    switch (s.mode) {
    case mode_start://#.
    case mode_prefix://prefix.
    case mode_infix://infix.
        //->prefix | primary
        iter = grammar.prefixToken.find(mLookahead);
        if (iter != grammar.prefixToken.end()) {
            accept();
            s.op = iter->second;
            s.mode = mode_prefix;
            return;
        }
        s.op = primary();
        s.mode = mode_primary;
        return;
    case mode_primary://primary.
    case mode_postfix://postfix.
        //->suffix
        if (mLookahead == LBRACKET) {		//·½À¨ºÅ
            s.op = suffix();
            s.mode = mode_suffix;
            return;
        }
    case mode_suffix://suffix.
        //->infix | postfix
        if (mLinebreak || grammar.end.count(mLookahead)) {
            s.mode = mode_end;
            return;
        }
        iter = grammar.infixToken.find(mLookahead);
        if (iter != grammar.infixToken.end()) {
            accept();
            s.op = iter->second;
            s.mode = mode_infix;
            return;
        }
        iter = grammar.postfixToken.find(mLookahead);
        if (iter != grammar.postfixToken.end()) {
            accept();
            s.op = iter->second;
            s.mode = mode_postfix;
            return;
        }
        if (mLookahead == BLANK && mLookback == SYMBOL) {
            //SYMBOL BLANK -> SYMBOL COLON BLANK
            s.op = grammar.operCOLON;					//Ã°ºÅ
            s.mode = mode_infix;
            return;
        }
        if (mLookahead == POUND) {
            //POUND -> PERIOD POUND
            s.op = grammar.operPERIOD;
            s.mode = mode_infix;
            return;
        }
        if (mLookahead == MINUS) {
            //MINUS -> PLUS MINUS
            s.op = grammar.operPLUS;
            s.mode = mode_infix;
            return;
        }
        //SPACE -> AT
        s.op = grammar.operAT;
        s.mode = mode_infix;
        return;
    }
}
void Parser::Parsing::reduce(Frame &s) {
    uint r[3];
    switch (s.mode) {
    case mode_infix:
    case mode_postfix:
    case mode_end:
        while (!mOper.empty() && mOper.size() != s.offset) {
            uint oper = mOper.back();
			if (grammar.oper[oper].postfix) {
				r[0] = node(tag_expression, oper);
				r[1] = mOprn.back();
				mOprn.pop_back();
				insert(r[0], r[1]);
			}
			else if (compare(s)) {
				r[0] = node(tag_expression, oper);
				if (grammar.oper[oper].prefix) {
					r[1] = mOprn.back();
					mOprn.pop_back();
					insert(r[0], r[1]);
				} else {
					r[1] = mOprn.back();
					mOprn.pop_back();
					r[2] = mOprn.back();
					mOprn.pop_back();
					insert(r[0], r[2]);
					insert(r[0], r[1]);
				}
			} else
				break;
            mOper.pop_back();
            mOprn.push_back(r[0]);
        }
        return;
    case mode_suffix: {
        r[0] = mOprn.back();
        mOprn.pop_back();
        while (!mOper.empty() && grammar.oper[mOper.back()].postfix) {
			r[1] = node(tag_expression, mOper.back());
			mOper.pop_back();
			insert(r[1], r[0]);
			r[0] = r[1];
		}
        insert(s.op, r[0]);
    }
    return;
    }
}
bool Parser::Parsing::compare(Frame &s) {
    if (s.mode == mode_end)
        return true;
    if (grammar.oper[s.op].prec < grammar.oper[mOper.back()].prec)
        return true;
    if (grammar.oper[s.op].prec == grammar.oper[mOper.back()].prec)
        return grammar.oper[s.op].left;
    return false;
}
uint Parser::Parsing::expression() {
    uint r[1];
    Frame s(mOper.size(), mode_start);
    while (s.mode != mode_end) {
        shift(s);
        reduce(s);
        switch (s.mode) {
        case mode_prefix:
        case mode_infix:
        case mode_postfix:
            mOper.push_back(s.op);
            break;
        case mode_primary:
        case mode_suffix:
            mOprn.push_back(s.op);
            break;
        }
    }
    r[0] = mOprn.back();
    mOprn.pop_back();
    return r[0];
}

/*
suffix
: [suffix] (LBRACKET sequence? RBRACKET)*
;
*/
uint Parser::Parsing::suffix() {
    uint r[3];
    r[0] = node(tag_suffix, -1);
    while (mLookahead == LBRACKET) {
        accept();
        if (mLookahead == RBRACKET) {
            accept();
            r[1] = node(tag_suffix, -2);
            insert(r[0], r[1]);
            continue;
        }
        r[1] = sequence();
        match(RBRACKET);
        r[2] = node(tag_suffix, -3);
        insert(r[2], r[1]);
        insert(r[0], r[2]);
    }
    return r[0];
}

uint Parser::Parsing::primary() {
	uint r[3];
	skip();
	r[1] = atom();
	if (r[1] == 0)
		mLookahead == EOI ? more() : error();
	if (mLookahead == SPACE) {
		skip();
		r[2] = atom();
		if (r[2] == 0)
			return r[1];
		r[0] = node(tag_primary, -1);
		insert(r[0], r[1]);
		insert(r[0], r[2]);
		while (mLookahead == SPACE) {
			skip();
			r[2] = atom();
			if (r[2] == 0)
				break;
			insert(r[0], r[2]);
		}
		skip();
		return r[0];
	}
	skip();
	return r[1];
}

/*
atom
: INTEGER
| FLOAT
| STRING
| LPAREN expression RPAREN
| brace
| backquote
| symbol
| blank
| POUND
;
*/
uint Parser::Parsing::atom() {
    uint r = 0;
    switch (mLookahead) {
    case INTEGER:
        r = node(tag_atom, instr_integer);
        note(r, mText);
        accept(false);
        return r;
    case FLOAT:
        r = node(tag_atom, instr_float);
        note(r, mText);
        accept(false);
        return r;
    case STRING:
        r = node(tag_atom, instr_string);
        note(r, mText);
        accept(false);
        return r;
    case LPAREN: {
        uint t;
        accept();
        t = sequence();
        match(RPAREN, false);
        r = node(tag_atom, -1);
        insert(r, t);
    }
    return r;
    case LBRACE:
        return brace();
    case BACKQUOTE:
        return backquote();
    case SYMBOL:
        return symbol();
    case BLANK:
        return blank();
    case POUND:
        return pound();
    case DOLLAR:
        return dollar();
	}
	if (mLookback == COMMA || mLookback == SEMICOLON)
		r = node(tag_atom, instr_null);
	return r;
}

/*
brace
: [brace] LBRACE sequence? RBRACE
;
*/
uint Parser::Parsing::brace() {
    uint r[2];
    accept();
    if (mLookahead == RBRACE) {
        accept(false);
        r[0] = node(tag_brace, -1);
    } else {
        r[0] = node(tag_brace, -2);
        r[1] = sequence();
        match(RBRACE, false);
        insert(r[0], r[1]);
    }
    return r[0];
}

/*
backquote
: [backquote] (BACKQUOTE SYMBOL)+
;
*/
uint Parser::Parsing::backquote() {
    uint r[2];
    r[0] = node(tag_backquote, -1);
    while (mLookahead == BACKQUOTE) {
        accept(false);
        if (mLookahead == SYMBOL) {
            r[1] = node(tag_backquote, instr_symbol);
            note(r[1], mText);
            insert(r[0], r[1]);
            accept(false);
        } else
            error();
    }
    return r[0];
}

/*
symbol
: [symbol] SYMBOL backquote?
;
*/
uint Parser::Parsing::symbol() {
    uint r[3];
    r[0] = node(tag_symbol, instr_symbol);
    note(r[0], mText);
    accept(false);
    if (mLookahead == BACKQUOTE) {
        r[1] = backquote();
        r[2] = node(tag_symbol, -1);
        insert(r[2], r[0]);
        insert(r[2], r[1]);
        return r[2];
    }
    return r[0];
}

/*
blank
: [blank] BLANK (backquote | symbol)?
;
*/
uint Parser::Parsing::blank() {
    uint r[2];
    accept(false);
    switch (mLookahead) {
    case BACKQUOTE:
        r[1] = backquote();
        r[0] = node(tag_blank, instr_symbol);
        insert(r[0], r[1]);
        break;
    case SYMBOL:
        r[1] = symbol();
        r[0] = node(tag_blank, instr_symbol);
        insert(r[0], r[1]);
        break;
    default:
        r[0] = node(tag_blank, -1);
        break;
    }
    return r[0];
}

/*
pound
: [pound] POUND (INTEGER | STRING | SYMBOL)?
;
*/
uint Parser::Parsing::pound() {
    uint r[2];
    accept(false);
    switch (mLookahead) {
    case INTEGER:
        r[0] = node(tag_pound, -1);
        note(r[0], mText);
        accept(false);
        break;
    case STRING:
    case SYMBOL:
        r[0] = node(tag_pound, -2);
        note(r[0], mText);
        accept(false);
        break;
    default:
        r[0] = node(tag_pound, instr_key);
        break;
    }
    return r[0];
}

/*
dollar
: [dollar] DOLLAR (STRING | SYMBOL)?
;
*/
uint Parser::Parsing::dollar() {
    uint r[2];
    accept(false);
    switch (mLookahead) {
    case STRING:
    case SYMBOL:
        r[0] = node(tag_dollar, -1);
        note(r[0], mText);
        accept(false);
        break;
    default:
        r[0] = node(tag_dollar, instr_symbol);
        break;
    }
    return r[0];
}
}
