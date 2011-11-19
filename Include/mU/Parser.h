#pragma once
#include "Var.h"

namespace mU {
//////////////////////////////////////
class parser
{
public:
	DLL parser();
	DLL parser(wistream&);
	DLL void start(wistream&); //和新的数据流相关联
	DLL void parse(); //parse一个完整表达式
	DLL var result(); //取得当前parse的结果
	DLL void clear();
	DLL bool has_more() { return lookahead != EOI; }

	// FIXME; wrap these two data members with getters
	size_t lineno;
	size_t column;

	DLL static const wchar *character_name(wchar x); //unicode字符的名称

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
		const wchar *name; //名字
		const wchar *show; //显示
		bool postfix;
		bool prefix;
		bool rassoc; //是否右结合
		size_t prec; //优先级
		Var symbol; //对应symbol
		oper_t(token_t _token,const wchar *_name,
			const wchar *_show, bool _postfix = false,
			bool _prefix = false,bool _rassoc = false) : 
		token(_token),name(_name),
			show(_show),postfix(_postfix),
			prefix(_prefix),rassoc(_rassoc),
			prec(s_prec),symbol(Sym(name, System)) {}
	};
	DLL static const oper_t *lookup_oper(Var); //运算符的输出显示，如Plus到+
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
	struct char_t //unicode码位和名称对应
	{
		wint_t unicode;
		const wchar *name;
		Var symbol;
		char_t(size_t _unicode,const wchar *_name) : 
		unicode(_unicode),name(_name),symbol(Sym(name, System)) {}
	};
	DLL static std::vector<char_t> s_char; //能认识的特殊字符
	DLL static stdext::hash_map<wint_t,size_t> s_unicode; //unicode码位到s_char下标
	DLL static stdext::hash_map<wstring,size_t> s_name; //特殊名称到s_char下标
	DLL static size_t s_prec; //当前最高优先级

	DLL static std::vector<oper_t> s_oper; //存所有运算符
	DLL static std::map<token_t,size_t> s_postfix_token, s_prefix_token, s_infix_token; //从token_t枚举值到s_oper下标映射
	DLL static stdext::hash_map<Var,size_t> s_oper_symbol; //从某个符号到s_oper下标映射
	DLL static std::set<token_t> s_end; //用来判断expression扫描是否结束
	DLL static size_t operPLUS, operSTAR;
	DLL static int precInequality;
	DLL static std::set<token_t> s_inequality;
	DLL static void init(); //初始化所有static变量

	/* 一下五个成员是lexer向parser的交流变量，scan()会修改 */
	wstring text; //token_t对应字符串
	token_t lookahead;
	token_t lookback;
	bool skip; //本次scan是否跳过空格
	bool linebreak; //是否跳过换行符

private:
	wistream *m_in;
	void error();
	void more();

	std::stack<wchar> char_stack;
	wchar read(bool in_string_literal=false); //底层，忽略注释，转换字符
	token_t scan(); //调用lexer！
	
	struct node_t //语法分析到代码生成中间数据结构
	{
		tag_t tag;
		int value; //依据具体tag而定。建议改成union或enum，也可以和tag_t合并
	};
	std::vector<node_t> m_node; //表达式树结点
	std::multimap<size_t,size_t> m_child; //和m_node一起构成树，从父亲指向孩子
	stdext::hash_set<wstring> m_entry; //字符串池，wstring的c_str()返回字符串地址不会改变，用来节约内存
	std::map<size_t,const wchar*> m_note; //从m_node下标指向m_entry管理的内容

	std::stack<size_t> oper_stack;
	std::stack<size_t> oprn_stack;
	enum mode_t {
		mode_start, mode_prefix, mode_infix, mode_primary,
		mode_postfix, mode_suffix, mode_end = -1
	};
	struct frame_t //每一层expression建立一个，该层语法分析完毕销毁
	{
		size_t offset; //栈底：oper_stack, oprn_stack。所有frame_t栈堆在一起
		mode_t mode;
		size_t op; //当前扫到的还未入栈的操作符在s_oper下标及其
		int prec; //优先级

		frame_t(size_t o,mode_t m,int p = 0) :
		offset(o),mode(m),op(0),prec(p) {}
	};
	size_t node(tag_t t,int v); //create new node，放入m_node，返回下标
	void insert(size_t i, size_t j); //添加父子关系
	const wchar* entry(const wstring &s); //在m_entry插入/查找并返回
	void note(size_t i, const wstring &s); //m_note新加域，指定字符串

	/* LR模式 */
	void shift(frame_t &s);
	void reduce(frame_t &s);
	bool compare(frame_t &s); //工具函数，比较优先级，高？低？相同->右结合？
	/* 工具函数，token级别底层操作 */
	void accept(); //接受下一个token
	void match(token_t t); //下一个token必须为指定的，否则报错
	/* LL模式 */
	size_t expression(int);
	size_t inequality();
	size_t expression();
	size_t suffix();
	size_t sequence();
	size_t primary();

	std::vector<var> m_code; //管理最后生成的var树的所有var
	std::stack<std::pair<Var,size_t> > code_stack;
	typedef std::multimap<size_t,size_t>::const_iterator iter_t;
	iter_t lower(size_t i);
	iter_t upper(size_t i);
	size_t count(size_t i);
	void emit(Var); //填充表达式
	void head(size_t n); //生成长为n（头部也算）的表达式
	void gen(size_t m); //将中间数据结构的m为根的子树代码生成到对应var树，放入m_code
};
//////////////////////////////////////
}