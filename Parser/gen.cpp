#include <nV/Kernel.h>
#include <nV/Parser.h>
#include <nV/utils.h>
#include <stdexcept>
#include <vector>

namespace nV {
var Parser::eval() {
    var r;
    while (parsing->mLookahead != EOI) {
        try {
            r = gen(parse());
        } catch (std::exception&) {
            kernel.logging(__FUNCTIONW__) << _W("Parse error at line ") << parsing->lineno
                                          << _W(" column ") << parsing->column << _W(".") << endl;
            throw;
        }
        try {
            r = kernel.eval(r);
        } catch (std::exception&) {
            kernel.logging(__FUNCTIONW__) << _W("Eval error at line ") << parsing->lineno
                                          << _W(" column ") << parsing->column << _W(".") << endl;
            kernel.start();
            throw;
        }
    }
    return r;
}
var Parser::parseall() {
    while (parsing->mLookahead != EOI) {
        try {
			gen(parse());
        } catch (std::exception&) {
            kernel.logging(__FUNCTIONW__) << _W("Parse error at line ") << parsing->lineno
                                          << _W(" column ") << parsing->column << _W(".") << endl;
            throw;
        }
    }
	return nV::list(genning->mCode.size(), genning->mCode.begin());
}
void Parser::parseonly() {
    while (parsing->mLookahead != EOI) {
        try {
			parse();
        } catch (std::exception&) {
            kernel.logging(__FUNCTIONW__) << _W("Parse error at line ") << parsing->lineno
                                          << _W(" column ") << parsing->column << _W(".") << endl;
            throw;
        }
    }
}
var Parser::gen(uint m) {
	if (!genning)
		genning = new Genning;
	genning->start(kernel, mNode, mNote);
	genning->gen(m);
	return genning->mCode.back();
}
void Parser::Genning::emit(const var& x) {
    if (mGen.empty()) {
        mCode.push_back(x);
        return;
    }
    std::pair<var, uint> &c = mGen.back();
    c.first.tuple()[c.second] = x;
    if (++c.second == c.first.tuple().size)
        mGen.pop_back();
}
void Parser::Genning::head(uint n) {
    var r(tuple(n));
    emit(r);
    mGen.push_back(std::make_pair(r, 0));
}
#define oper(x) emit($.x)
void Parser::Genning::gen(uint m) {
    Parser::Node& n = (*pNode)[m];
	TreeIterator iter(n.treenode.firstChild, (*pNode), false);
	uint nChild = n.treenode.nChild;

    switch (n.tag) {
    case tag_sequence: {
			std::vector<TreeIterator> temp;
			for (uint i = 0; i < nChild; ++i) {
				temp.push_back(iter);
				++iter;
			}
			for (sint i = nChild - 1; i >= 0; --i) {
				gen(*temp[i]);
			}
    }
    break;
    case tag_expression: {
        if (grammar.oper[n.value].postfix || grammar.oper[n.value].prefix) {
            head(2);
            emit(grammar.operSymbol[n.value]);
            gen(*iter);
            break;
        }
        head(3);
        emit(grammar.operSymbol[n.value]);
		TreeIterator temp = iter;
        gen(*(++iter));
		gen(*temp);
	} 
        break;
    case tag_suffix:
        switch (n.value) {
        case -1: {
			std::vector<TreeIterator> temp;
			for (uint i = 0; i < nChild; ++i) {
				temp.push_back(iter);
				++iter;
			}
			std::vector<TreeIterator>::const_iterator temp_iter
				= temp.begin();
			while (temp_iter != temp.end() - 1) {
                ++temp_iter;
				gen(**temp_iter);
			}
			gen(*temp.front());
            while (temp_iter != temp.begin()) {
                gen(**temp_iter);
                --temp_iter;
            }
        }
        break;
        case -2:
            head(1);
            n.value = -4;
            break;
        case -3:
            head(count(*iter) + 1);
            n.value = -5;
            break;
        case -4:
            break;
        case -5:
            gen(*iter);
            break;
        }
        break;
	case tag_primary: {
			head(nChild);
			std::vector<TreeIterator> temp;
			for (uint i = 0; i < nChild; ++i) {
				temp.push_back(iter);
				++iter;
			}
			for (sint i = nChild - 1; i >= 0; --i) {
				gen(*temp[i]);
			}
		}
        break;
    case tag_atom:
        switch (n.value) {
        case instr_null: {
            emit(null);
        }
        break;
        case instr_integer: {
            const wstring& s = (*pNote)[(*pNode)[m].note];
            emit(new Integer(string(s.begin(), s.end()).c_str(), 10));
        }
        break;
        case instr_float: {
			 const wstring& s = (*pNote)[(*pNode)[m].note];
            emit(new Real(string(s.begin(), s.end()).c_str(), 10, s.length()));
        }
        break;
        case instr_string: {
            emit(new String((*pNote)[(*pNode)[m].note]));
        }
        break;
        case -1: {
			uint cnt = count(*iter);
			if (cnt > 1) {
				head(cnt + 1);
				oper(Sequence);
			}
			gen(*iter);
		}
		break;
        }
        break;
    case tag_brace:
        switch (n.value) {
        case -1: {
            head(1);
            oper(List);
        }
        break;
        case -2: {
            head(count(*iter) + 1);
            oper(List);
            gen(*iter);
        }
        break;
        }
        break;
    case tag_backquote:
        switch (n.value) {
        case -1: {
            sym c = (*pKernel).context();
			std::vector<TreeIterator> temp;
			for (uint i = 0; i < nChild; ++i) {
				temp.push_back(iter);
				++iter;
			}
            for (sint i = nChild - 1; i >= 0; --i) {
                c = c->symbol(wstr((*pNote)[(*pNode)[*temp[i]].note]));
            }
            emit(c);
        }
        break;
        }
        break;
    case tag_symbol:
        switch (n.value) {
        case instr_symbol: {
            wcs s = wstr((*pNote)[(*pNode)[m].note]);
            if (islower(s[0]))
                emit(key(s));
            else
                emit((*pKernel).symbol(s));
        }
        break;
        case -1: {
			TreeIterator temp = iter;
            (*pKernel).beginContext(root->symbol(wstr((*pNote)[(*pNode)[*(++iter)].note])));
            gen(*temp);
            (*pKernel).endContext();
        }
        break;
        }
        break;
    case tag_blank:
        switch (n.value) {
        case instr_symbol: {
            head(2);
            oper(Blank);
            gen(*iter);
        }
        break;
        case -1: {
            head(1);
            oper(Blank);
        }
        break;
        }
        break;
    case tag_pound:
        switch (n.value) {
        case -1: {
            wstringstream wss;
            wss << (*pNote)[(*pNode)[m].note];
            uint k = 0;
            wss >> k;
            emit(key(k));
        }
        break;
        case -2: {
            emit(key(wstr((*pNote)[(*pNode)[m].note])));
        }
        break;
        case instr_key: {
            emit(key(reinterpret_cast<wcs>(0)));
        }
        break;
        }
        break;
    case tag_dollar:
        switch (n.value) {
        case -1: {
            emit((*pKernel).symbol(wstr((*pNote)[(*pNode)[m].note])));
        }
        break;
        case instr_symbol: {
            emit((*pKernel).context());
        }
        break;
        }
        break;
    }
}
}
