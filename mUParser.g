parser grammar mUParser;

options {
	tokenVocab=mULexer;
}

program
	:	(expr SEMICOLON WS*)*
	;

expr
	:	ADD_ASSIGN
	{
		System.out.println("ADD_ASSIGN");
	}
	|	AND_OP
	{
		System.out.println("AND_OP");
	}
	|	ASSIGN
	{
		System.out.println("ASSIGN");
	}
	|	ASSIGN_EQ
	{
		System.out.println("ASSIGN_EQ");
	}
	|	ASSIGN_GT
	{
		System.out.println("ASSIGN_GT");
	}
	|	ASSIGN_NE
	{
		System.out.println("ASSIGN_NE");
	}
	|	ASSIGN_PERIOD
	{
		System.out.println("ASSIGN_PERIOD");
	}
	|	AT
	{
		System.out.println("AT");
	}
	|	AT_AT
	{
		System.out.println("AT_AT");
	}
	|	BANG
	{
		System.out.println("BANG");
	}
	|	BOOL_AND_OP
	{
		System.out.println("BOOL_AND_OP");
	}
	|	BOOL_OR_OP
	{
		System.out.println("BOOL_OR_OP");
	}
	|	CIRCUMFLEX
	{
		System.out.println("CIRCUMFLEX");
	}
	|	COLON
	{
		System.out.println("COLON");
	}
	|	COLON_ASSIGN
	{
		System.out.println("COLON_ASSIGN");
	}
	|	COLON_GT
	{
		System.out.println("COLON_GT");
	}
	|	COMMENT
	{
		System.out.println("COMMENT");
	}
	|	DEC_OP
	{
		System.out.println("DEC_OP");
	}
	|	DIV_ASSIGN
	{
		System.out.println("DIV_ASSIGN");
	}
	|	EQ_OP
	{
		System.out.println("EQ_OP");
	}
	|	ESC_SEQ
	{
		System.out.println("ESC_SEQ");
	}
	|	GE_OP
	{
		System.out.println("GE_OP");
	}
	|	GT_OP
	{
		System.out.println("GT_OP");
	}
	|	IDENTIFIER
	{
		System.out.println("IDENTIFIER");
	}
	|	INC_OP
	{
		System.out.println("INC_OP");
	}
	|	INTEGER
	{
		System.out.println("INTEGER");
	}
	|	LEFT_OP
	{
		System.out.println("LEFT_OP");
	}
	|	LE_OP
	{
		System.out.println("LE_OP");
	}
	|	LT_GT
	{
		System.out.println("LT_GT");
	}
	|	LT_OP
	{
		System.out.println("LT_OP");
	}
	|	MINUS
	{
		System.out.println("MINUS");
	}
	|	MOD_ASSIGN
	{
		System.out.println("MOD_ASSIGN");
	}
	|	MUL_ASSIGN
	{
		System.out.println("MUL_ASSIGN");
	}
	|	NE_OP
	{
		System.out.println("NE_OP");
	}
	|	OCTAL_ESC
	{
		System.out.println("OCTAL_ESC");
	}
	|	OR_OP
	{
		System.out.println("OR_OP");
	}
	|	PERCENT
	{
		System.out.println("PERCENT");
	}
	|	PERIOD
	{
		System.out.println("PERIOD");
	}
	|	PERIOD_PERIOD
	{
		System.out.println("PERIOD_PERIOD");
	}
	|	PLUS
	{
		System.out.println("PLUS");
	}
	|	POW_ASSIGN
	{
		System.out.println("POW_ASSIGN");
	}
	|	POW_COLON_ASSIGN
	{
		System.out.println("POW_COLON_ASSIGN");
	}
	|	PTR_OP
	{
		System.out.println("PTR_OP");
	}
	|	QUESTIONMARK
	{
		System.out.println("QUESTIONMARK");
	}
	|	QUOTE
	{
		System.out.println("QUOTE");
	}
	|	REAL
	{
		System.out.println("REAL");
	}
	|	RIGHT_GT
	{
		System.out.println("RIGHT_GT");
	}
	|	RIGHT_OP
	{
		System.out.println("RIGHT_OP");
	}
	|	SEMICOLON
	{
		System.out.println("SEMICOLON");
	}
	|	SLASH
	{
		System.out.println("SLASH");
	}
	|	SLASH_AT
	{
		System.out.println("SLASH_AT");
	}
	|	SLASH_PERIOD
	{
		System.out.println("SLASH_PERIOD");
	}
	|	SLASH_SEMICOLON
	{
		System.out.println("SLASH_SEMICOLON");
	}
	|	SLASH_SLASH
	{
		System.out.println("SLASH_SLASH");
	}
	|	SLASH_SLASH_AT
	{
		System.out.println("SLASH_SLASH_AT");
	}
	|	SLASH_SLASH_PERIOD
	{
		System.out.println("SLASH_SLASH_PERIOD");
	}
	|	STAR
	{
		System.out.println("STAR");
	}
	|	STRING
	{
		System.out.println("STRING");
	}
	|	SUB_ASSIGN
	{
		System.out.println("SUB_ASSIGN");
	}
	|	TILDE
	{
		System.out.println("TILDE");
	}
	|	TILDE_TILDE
	{
		System.out.println("TILDE_TILDE");
	}
	|	UNICODE_ESC
	{
		System.out.println("UNICODE_ESC");
	}
	|	WS
	{
		System.out.println("WS");
	}
	;
