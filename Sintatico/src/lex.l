%{


#include <stdio.h>

char * stringLexem;

/* Quando retorno o enum ZERO "0", o sistema para a execução. Podemos usar futuramente como um token de ERRO*/
 //TYPE_INTEGER_, TYPE_REAL_, TYPE_CHAR_, TYPE_STRING_, TYPE_BOOLEAN_, TRUE_, FALSE_USES_,BOOLEAN_,CHAR_,
enum TokenId{DOLLAR,  STRING_, REAL_NUMBER_, INTEGER_NUMBER_, IDENTIFIER_, SET_, OF_, RECORD_, END_, LIST_, ARRAY_, IN_, NOT_, NIL_, CASE_, ELSE_, THEN_, IF_, DO_, DOWNTO_, TO_, FOR_, WHILE_, BEGIN_, ASSIGNMENT_, VAR_, DIV_, MOD_, AND_, OR_, WRITE_, WRITELN_, READ_, READLN_, FUNCTION_, PROCEDURE_, TYPE_, CONST_, PROGRAM_, COMMA_, DOT_,UPARROW_, COLON_, SEMI_COLON_, DOTDOT_, LBRAC_, RBRAC_, LPAREN_, RPAREN_, ARROBA_, LBRAC_BAR_, RBRAC_BAR_, CONS_, OP_MULT_, OP_DIV_, EQUAL_, NOTEQUAL_, LT_, LE_, GT_, GE_, SIGN_};


%}

/* definições regulares */

delim				[\ t]
ws					{delim}+
letter				[A-Za-z]
digit				[0-9]
id					{letter}({letter}|{digit}|"_")*
sign				[+-]
integer_number			{sign}?{digit}+([Ee]{sign}?{digit}+)?
real_number			{sign}?{digit}+\.({digit}+)?([Ee]{sign}?{digit}+)?
char				['].[']
%x string comment


%%

\{        			{BEGIN(comment);}
<comment>[^\{\}\n<<EOF>>]*  	;
<comment>\n      		++yylineno;
<comment><<EOF>>     		{printf( "LINE %d: O comentario nao foi finalizado", yylineno); yyterminate();}
<comment>\}      		{BEGIN(INITIAL);}

\'         			{BEGIN (string);}
<string>[^\'\n<<EOF>>]* 	stringLexem = yytext;
<string>\n      		++yylineno;
<string><<EOF>>      		{printf("LINE %d: A string nao foi finalizada", yylineno);yyterminate();}
<string>\'			{ BEGIN(INITIAL);return STRING_;}


{ws}				{}
[\n]				{yylineno++;}
"then"				{return (THEN_);}
"if"				{return (IF_);}
"else"				{return (ELSE_);}
"set" 				{return (SET_);}
"of" 				{return (OF_);}
"record" 			{return (RECORD_);}
"end" 				{return (END_);}
"list" 				{return (LIST_);}
"array" 			{return (ARRAY_);}
"div"				{return (DIV_);}
"mod"				{return (MOD_);}
"and"   			{return (AND_);}
"or" 				{return (OR_);}
"in" 				{return (IN_);}
"not" 				{return (NOT_);}
"nil" 				{return (NIL_);}
"case" 				{return (CASE_);}
"downto" 			{return (DOWNTO_);}
"do" 				{return (DO_);}
"to" 				{return (TO_);}
"for" 				{return (FOR_);}
"while" 			{return (WHILE_);}
"begin" 			{return (BEGIN_);}
"var"				{return (VAR_);}
"function" 			{return (FUNCTION_);}
"procedure" 			{return (PROCEDURE_);}
"type" 				{return (TYPE_);}
"const" 			{return (CONST_);}
"program" 			{return (PROGRAM_);}
"write"				{return (WRITE_);}
"readln"			{return (READLN_);}
"read"				{return (READ_);}
"writeln"			{return (WRITELN_);}
".." 				{return (DOTDOT_);}
","    				{return (COMMA_);}
"."    				{return (DOT_);}
{sign}				{return (SIGN_);}
"^"    				{return (UPARROW_);}
":=" 				{return (ASSIGNMENT_);}
"::"				{return (CONS_);}
":"    				{return (COLON_);}
";"    				{return (SEMI_COLON_);}
"[|"				{return (LBRAC_BAR_);}
"|]"				{return (RBRAC_BAR_);}
"["    				{return (LBRAC_);}
"]"    				{return (RBRAC_);}
"("    				{return (LPAREN_);}
")"    				{return (RPAREN_);}
"@"	   			{return (ARROBA_);}
"*"				{return (OP_MULT_);}
"/"				{return (OP_DIV_);}
"<"				{return (LT_);}
"<="				{return (LE_);}
"="				{return (EQUAL_);}
"<>"				{return (NOTEQUAL_);}
">"				{return (GT_);}
">="				{return (GE_);}

{id}				{return (IDENTIFIER_);}
{integer_number}		{return (INTEGER_NUMBER_);}
{real_number}			{return (REAL_NUMBER_);}


.				{printf("LINE %d:Caractere desconhecido", yylineno);}
<<EOF>>				{yytext=(char*)"$";return (DOLLAR);}
		
		
%%
