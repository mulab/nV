lexer grammar mulexer;

options {
	language=C;
}

IDENTIFIER
	:	('_' | 'a'..'z' | 'A'..'Z' | '\u0080'..'\uFFFE') ('_' | 'a'..'z' | 'A'..'Z' | '0'..'9' | '\u0080'..'\uFFFE')*
	;

INTEGER
	:	('0'..'9')+
    ;

REAL
    :   ('0'..'9')* '.' ('0'..'9')+
    ;

COMMENT
    :   '(*' ( options {greedy=false;} : . )* '*)' {$channel=HIDDEN;}
    ;

WS  :   ( ' '
    	|	'\t'
		|	'\r'
	    | 	'\n'
	    ) {}
    ;

STRING
    :  '"' ( ESC_SEQ | ~('\\'|'"') )* '"'
    ;

fragment
ESC_SEQ
    :   '\\' ('b'|'t'|'n'|'f'|'r'|'\"'|'\''|'\\')
    |   UNICODE_ESC
    |   OCTAL_ESC
    ;

fragment
OCTAL_ESC
    :   '\\' ('0'..'3') ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7')
    ;

fragment
UNICODE_ESC
    :   '\\' 'u' ('0'..'9'|'a'..'f'|'A'..'F') ('0'..'9'|'a'..'f'|'A'..'F') ('0'..'9'|'a'..'f'|'A'..'F') ('0'..'9'|'a'..'f'|'A'..'F')
    ;

SEMICOLON
    :	';'
    ;

QUESTIONMARK
	:	'?'
	;

LEFT_OP
	:	'<<'
	;

RIGHT_OP
	:	'>>'
	;

RIGHT_GT
	:	'>>>'
	;

ASSIGN
	:	'='
	;

COLON_ASSIGN
	:	':='
	;

POW_ASSIGN
	:	'^='
	;

POW_COLON_ASSIGN
	:	'^:='
	;

ASSIGN_PERIOD
	:	'=.'
	;

SLASH_SLASH
	:	'//'
	;

AND_OP
	:	'&'
	;

ADD_ASSIGN
	:	'+='
	;

SUB_ASSIGN
	:	'-='
	;

MUL_ASSIGN
	:	'*='
	;

DIV_ASSIGN
	:	'/='
	;

MOD_ASSIGN
	:	'%='
	;

ASSIGN_GT
	:	'=>'
	;

SLASH_PERIOD
	:	'/.'
	;

SLASH_SLASH_PERIOD
	:	'//.'
	;

PTR_OP
	:	'->'
	;

COLON_GT
	:	':>'
	;

SLASH_SEMICOLON
	:	'/;'
	;

TILDE_TILDE
	:	'~~'
	;

COLON
	:	':'
	;

OR_OP
	:	'|'
	;

PERIOD_PERIOD
	:	'..'
	;

BOOL_OR_OP
	:	'||'
	;

BOOL_AND_OP
	:	'&&'
	;

QUOTE
	:	'\''
	;

BANG
	:	'!'
	;

ASSIGN_EQ
	:	'==='
	;

ASSIGN_NE
	:	'=!='
	;

EQ_OP
	:	'=='
	;

LE_OP
	:	'<='
	;

GE_OP
	:	'>='
	;

NE_OP
	:	'!='
	;

LT_OP
	:	'<'
	;

GT_OP
	:	'>'
	;

STAR
	:	'*'
	;

SLASH
	:	'/'
	;

PERCENT
	:	'%'
	;

PLUS
	:	'+'
	;

MINUS
	:	'-'
	;

PERIOD
	:	'.'
	;

CIRCUMFLEX
	:	'^'
	;

LT_GT
	:	'<>'
	;

TILDE
	:	'~'
	;

SLASH_AT
	:	'/@'
	;

SLASH_SLASH_AT
	:	'//@'
	;

AT_AT
	:	'@@'
	;

AT	:	'@'
	;

INC_OP
	:	'++'
	;

DEC_OP
	:	'--'
	;

