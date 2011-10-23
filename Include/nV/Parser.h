#pragma once
#include "Grammar.h"

// TODO: 可通过语言动态修改语法定义,类似于Lisp,Maxima,LaTeX(\catcode)

namespace nV {
class Parser {
public:
    API Parser(Kernel&);
	Kernel& kernel;

	struct TreeNode {
		uint firstChild;
		uint rightSibling;
		uint nChild;
		uint index;
		TreeNode(): firstChild(0),rightSibling(0), index(0), nChild(0) {}
	};
	enum Tag {
		tag_invalid = -1,
		tag_sequence,
		tag_expression,
		tag_suffix,
		tag_primary,
		tag_atom,
		tag_brace,
		tag_backquote,
		tag_symbol,
		tag_blank,
		tag_pound,
		tag_dollar
	};
	enum Instr {
		//字节码/Stack/FullForm/MX
		instr_null,
		instr_symbol,
		instr_key,
		instr_integer,
		instr_float,
		instr_string,
		instr_tuple
	};
    struct Node {
        Tag tag;
        int value;
		uint note;
		TreeNode treenode;
    };
	struct NodeStore{
		Tag tag;
		int value;
		uint note;
		uint firstChild;
		uint rightSibling;
		uint nChild;
	};
	struct TreeIterator {
		uint index;
		bool islast;
		std::vector<Node> &arraytree;
		TreeIterator(uint Index, std::vector<Node>& atree, bool IsLast = false) : index(Index), arraytree(atree), islast(IsLast){}
		TreeIterator(const TreeIterator &iter) : index(iter.index), arraytree(iter.arraytree), islast(iter.islast){}
		TreeIterator& operator=(const TreeIterator &iter){ index = iter.index; arraytree = iter.arraytree; islast = iter.islast; return *this; }
		uint operator*() const {return index;}
		TreeIterator& operator++() { 
			if (arraytree[index].treenode.rightSibling > 0)
				index = arraytree[index].treenode.rightSibling; 
			else
				islast = true;
			return *this; 
		}
		TreeIterator operator++(int) { TreeIterator tmp = *this; ++*this; return tmp; }
		//TreeIterator& operator--() { 
		//	if (islast == true)
		//		islast = false;
		//	else
		//		index = arraytree[index].treenode.leftSibling; 
		//	return *this; 
		//}
		//TreeIterator operator--(int) { TreeIterator tmp = *this; --*this; return tmp; }
		bool operator==(const TreeIterator& rls) const { return (index == rls.index) && (islast == rls.islast); }
		bool operator!=(const TreeIterator& rls) const { return (index != rls.index) || (islast != rls.islast); }
	};
	std::vector<Node> mNode;
	std::vector<wstring> mNote;
	struct Parsing {
		wistream* mIn;
		wstring mText;
		Token mLookahead;
		// mLookStack?
		Token mLookback;
		bool mSkip;
		bool mLinebreak;

		uint lineno;
		uint column;

		std::vector<wchar> mChar;
		std::vector<uint> mOper;
		std::vector<uint> mOprn;

		std::vector<Node> *pNode;
		std::vector<wstring> *pNote;

		Parsing(): mIn(&wcin), pNode(0), pNote(0) {};

		void start(wistream& in, std::vector<Node> &pnode, std::vector<wstring> &pnote) {
			mIn = &in; pNode = &pnode; pNote = &pnote;
			lineno = 1; column = 1; node(tag_invalid, 0);
			mText.clear(); mChar.clear(); mOper.clear(); mOprn.clear();
		}

		wchar read();
		Token scan();
		void accept(bool = true);
		void skip();
		void match(Token, bool = true);

		void error();
		void more();

		uint sequence(Token = COMMA);
		uint expression();
		uint suffix();
		uint primary();
		uint atom();
		uint brace();
		uint backquote();
		uint symbol();
		uint blank();
		uint pound();
		uint dollar();

		enum Mode {
			mode_end = -1,
			mode_start, 
			mode_prefix, 
			mode_infix, 
			mode_primary,
			mode_postfix, 
			mode_suffix
		};
		struct Frame {
			uint offset; 
			Mode mode;
			uint op;

			Frame(uint $offset, Mode $mode, uint $op = 0) :
				offset($offset), mode($mode), op($op) {}
		};
		uint node(Tag, int);
		void note(uint, const wstring&);
		void insert(uint, uint);
		void shift(Frame&);
		void reduce(Frame&);
		bool compare(Frame&);
	} *parsing;
	void start() {
		mNode.clear();
		mNote.clear();
	}
	API void start(wistream&);
	API uint parse();
	API void parseonly();

	struct Genning {
		Kernel* pKernel;
		std::vector<Node> *pNode;
		std::vector<wstring> *pNote;
		std::vector<var> mCode;
		std::vector<std::pair<var, uint> > mGen;
		void start(Kernel& pkernel, std::vector<Node> &pnode, std::vector<wstring> &pnote) {
			mGen.clear();
			mCode.clear();
			pKernel = &pkernel;
			pNode = &pnode;
			pNote = &pnote;
		}
		uint count(uint i) {
			return (*pNode)[i].treenode.nChild;
		}
		void emit(const var&);
		void head(uint);
		void gen(uint);
	} *genning;
	API var gen(uint);
	API var parseall();
	API var eval();

	struct Loading {
		const char* file_name;
		std::vector<Node> *pNode;
		std::vector<wstring> *pNote;
		std::vector<uint> mRootIndex;
		uint index;
		void start(const char* x, std::vector<Node> &pnode, std::vector<wstring> &pnote) {
			file_name = x; pNode = &pnode; pNote = &pnote;
			index = 0; mRootIndex.clear();
		}
		void load();
		void load(std::ifstream&, std::ifstream&, std::ifstream&);
		uint rootindex(){ 
			if (index < mRootIndex.size())
				return mRootIndex[index++];
			else
				return 0;
		}
	} *loading;
	void save(const char*, std::vector<uint>&);
	API void save(const char*);
	API uint load(const char*);
};
}

