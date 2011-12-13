grammar For_test;

option {
	
}

ID  :	('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*
    ;

INT :	'0'..'9'+
    ;

FLOAT
    :   ('0'..'9')* '.' ('0'..'9')* EXPONENT?
    |   ('0'..'9')+ EXPONENT
    ;

COMMENT
    :   '//' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
    |   '(*' ( options {greedy=false;} : . )* '*)' {$channel=HIDDEN;}
    ;

WS  :   ( ' '
    	|	'\t'
		|	'\r'
	    | 	'\n'
	    ) {$channel=HIDDEN;}
    ;

STRING
    :  '"' ( ESC_SEQ | ~('\\'|'"') )* '"'
    ;

CHAR:  '\'' ( ESC_SEQ | ~('\''|'\\') ) '\''
    ;

fragment
EXPONENT : ('e'|'E') ('+'|'-')? ('0'..'9')+ ;

fragment
HEX_DIGIT : ('0'..'9'|'a'..'f'|'A'..'F') ;

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
    :   '\\' 'u' HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT
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

PLUS
	:	'+'
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

