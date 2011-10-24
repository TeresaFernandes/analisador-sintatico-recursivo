#include "Sintatico.h"
Sintatico::Sintatico() {
    yyin = fopen("data.in","r");
    yyout = fopen("data.out","w");
    currentToken=Token();
}

Sintatico::~Sintatico(){}
    
Token Sintatico::intToToken (int i){
	if (i==STRING_){  
	  return Token(i,--stringLexem,yylineno);
	}
	return Token(i,yytext,yylineno);
}

void Sintatico::parse() throw (ErroSintatico)
{
    currentToken = intToToken(yylex());
    printf("Leu o lexema = %s \n", currentToken.lexeme);
    
    program();
  
    if (currentToken.id != DOLLAR)
        throw ErroSintatico((char*)"Fim de arquivo inesperado", currentToken.line);
}

void Sintatico:: match(int token) throw (ErroSintatico)
{
      if (currentToken.id == token){
	
	currentToken = intToToken(yylex());
	printf("Leu o lexema = %s \n", currentToken.lexeme);
      }else
	
        throw ErroSintatico((char*) /*" \"" + currentToken.lexeme + "\*/"token inesperado", currentToken.line);
}

//Programs and Blocks
void Sintatico::program () throw (ErroSintatico) {
	program_heading();

	block();

	match(DOT_);
}

void Sintatico::program_heading() throw (ErroSintatico) {
	match(PROGRAM_);
	
	identifier();

	match(SEMI_COLON_);
}

void Sintatico::block() throw (ErroSintatico) {
	declaration_part();

	statement_part();
}

void Sintatico::declaration_part() throw (ErroSintatico) {
	if (currentToken.id == CONST_)
		constant_definition_part();

	if (currentToken.id == TYPE_)
		type_definition_part();

	if (currentToken.id == VAR_)
		variable_declaration_part();

	procedure_and_function_declaration_part();
}

void Sintatico::constant_definition_part () throw (ErroSintatico) {
	match(CONST_);

	constant_definition();

	match(SEMI_COLON_);

	while (currentToken.id == IDENTIFIER_) {
		constant_definition();
		match(SEMI_COLON_);
	}
}

void Sintatico::constant_definition() throw (ErroSintatico) {
	identifier();

	match(EQUAL_);

	constant();
}

void Sintatico::type_definition_part() throw (ErroSintatico) {
	match(TYPE_);

	type_definition();

	match(SEMI_COLON_);

	while (currentToken.id == IDENTIFIER_) {
		type_definition();

		match(SEMI_COLON_);
	}
}

void Sintatico::type_definition() throw (ErroSintatico) {
	identifier();

	match(EQUAL_);

	type();
}

void Sintatico::variable_declaration_part() throw (ErroSintatico) {
	match(VAR_);

	variable_declaration();

	match(SEMI_COLON_);

	while (currentToken.id == IDENTIFIER_) {
		variable_declaration();

		match(SEMI_COLON_);
	}
}

void Sintatico::variable_declaration() throw (ErroSintatico) {
	identifier_list();

	match(COLON_);

	type();
}

void Sintatico::procedure_and_function_declaration_part() throw (ErroSintatico) {
	while (currentToken.id == FUNCTION_ || currentToken.id == PROCEDURE_) {

		if (currentToken.id == PROCEDURE_) {
			procedure_declaration();
		} else if (currentToken.id == FUNCTION_) {
			function_declaration();
		}
		match(SEMI_COLON_);

	}
}

void Sintatico::procedure_declaration() throw (ErroSintatico) {
	procedure_heading();

	match(SEMI_COLON_);

	block();
}

void Sintatico::function_declaration() throw (ErroSintatico) {
	function_heading();

	match(SEMI_COLON_);

	block();
}

void Sintatico::statement_part() throw (ErroSintatico) {
	match(BEGIN_);

	statement_sequence();

	match(END_);
}
//END - Programs and Blocks

// Procedure and Function Definitions
void Sintatico::procedure_heading() throw (ErroSintatico) {
	match(PROCEDURE_);
	
	identifier();
	
	if(currentToken.id == LPAREN_)
		formal_parameter_list();
}

void Sintatico::function_heading() throw (ErroSintatico) {
	match(FUNCTION_);

	identifier();

	if(currentToken.id == LPAREN_)
	    formal_parameter_list();

	match(COLON_);

	identifier();
}

void Sintatico::formal_parameter_list() throw (ErroSintatico) {
	match(LPAREN_);

	formal_parameter_section();

	while(currentToken.id == SEMI_COLON_) {
		match(SEMI_COLON_);

		formal_parameter_section();
	}

	match(RPAREN_);
}

void Sintatico::formal_parameter_section() throw (ErroSintatico) {
	switch(currentToken.id) {
	  
	  case IDENTIFIER_:
		  value_parameter_section();
		  break;
	  case VAR_:
		  variable_parameter_section();
		  break;
	  default:
		  throw ErroSintatico((char*)"Era esperado a \"var\" ou um Identificador", currentToken.line);
	}
}

void Sintatico::value_parameter_section() throw (ErroSintatico) {
	identifier_list();

	match(COLON_);

	parameter_type();
}

void Sintatico::variable_parameter_section() throw (ErroSintatico) {
	match(VAR_);

	identifier_list();

	match(COLON_);

	parameter_type();
}

void Sintatico::parameter_type() throw (ErroSintatico) {
	switch(currentToken.id) {
	case IDENTIFIER_:
		identifier();
		break;
	case ARRAY_:
		array_schema();
		break;
	case LIST_:
		list_type();
		break;
	default:
		throw ErroSintatico((char*)"Era esperado um Identificador, \"array\" ou \"list\"", currentToken.line);
	}
}

void Sintatico::array_schema() throw (ErroSintatico) {
	match(ARRAY_);

	match(RBRAC_);

	bound_specification();

	while(currentToken.id == SEMI_COLON_) {
		match(SEMI_COLON_);

		bound_specification();
	}

	match(LBRAC_);

	match(OF_);

	switch(currentToken.id){
	    case IDENTIFIER_:
		    match(IDENTIFIER_);
		    break;
	    case ARRAY_:
		    array_schema();
		    break;
	    default:
		    throw ErroSintatico((char*)"Era esperado um Identificador ou \"array\"", currentToken.line);
	}
}

void Sintatico::bound_specification() throw (ErroSintatico) {
	identifier();

	match(DOTDOT_);

	identifier();

	match(COLON_);

	identifier();
}

//END - Procedure and Function Definitions

//Statements
void Sintatico::statement_sequence() throw (ErroSintatico) {
	statement();

	while(currentToken.id == SEMI_COLON_) {
		match(SEMI_COLON_);

		statement();
	}
}

void Sintatico::statement() throw (ErroSintatico) {
	if (currentToken.id == IDENTIFIER_) {
	  
		simple_statement();
		
	} else if (currentToken.id == BEGIN_ || currentToken.id ==  WHILE_ || 
		   currentToken.id ==  FOR_ || currentToken.id ==  IF_ || 
		   currentToken.id == CASE_){
	  
		structured_statement();
	}
}

void Sintatico::simple_statement() throw (ErroSintatico) {
	match(IDENTIFIER_);
	
	if (currentToken.id == LBRAC_ ||currentToken.id == DOT_ ||
	    currentToken.id == UPARROW_ || currentToken.id == ASSIGNMENT_){
	  
	    assignment_statement();
	
	}else if (currentToken.id ==  LPAREN_){
	  
	      procedure_statement();	      
	}
}

void Sintatico::assignment_statement() throw (ErroSintatico) {
	variable_aux();
	
	match(ASSIGNMENT_);
	
	expression();
}

void Sintatico::procedure_statement() throw (ErroSintatico) {
	
	if (currentToken.id == LPAREN_)
		actual_parameter_list();
}

void Sintatico::structured_statement() throw (ErroSintatico) {
	
	switch(currentToken.id)
	{
		case BEGIN_:
			compound_statement();
			break;
		case WHILE_:
			repetitive_statement();
			break;
		case FOR_:
			repetitive_statement();
			break;
		case IF_:
			conditional_statement();
			break;
		case CASE_:
			conditional_statement();
			break;
		default:
			throw ErroSintatico((char*)"Era esperado \"begin\" , \"while\", \"for\", \"if\" ou \"case\"", currentToken.line);
	}
}

void Sintatico::compound_statement() throw (ErroSintatico) {
	match(BEGIN_);

	statement_sequence();

	match(END_);
}

//não precisa do switch pq so entra nesse metodo se tiver passado por structured-statement
void Sintatico::repetitive_statement() throw (ErroSintatico) {
	if(currentToken.id==WHILE_)
	{	
	    while_statement();
	}else{
	
	    for_statement();
	}
}

void Sintatico::while_statement() throw (ErroSintatico) {
	match(WHILE_);

	expression();

	match(DO_);

	statement();
}

void Sintatico::for_statement() throw (ErroSintatico) {
	match(FOR_);

	identifier();

	match(ASSIGNMENT_);

	expression();

	switch(currentToken.id)
	{
		case TO_:
			match(TO_);
			break;
		case DOWNTO_:
			match(DOWNTO_);
			break;
		default:
			throw ErroSintatico((char*)"Era esperado \"to\" ou \"downto\" ", currentToken.line);
	}

	expression();

	match(DO_);

	statement();
}

void Sintatico::conditional_statement() throw (ErroSintatico) {
	if(currentToken.id==IF_){
	  
		if_statement();
		
	}else{
	  
		case_statement();
		
	}
}

void Sintatico::if_statement() throw (ErroSintatico) {
	match(IF_);

	expression();

	match(THEN_);

	statement();

	if (currentToken.id == ELSE_){
	  
		match(ELSE_);
		
		statement();
	}
}

void Sintatico::case_statement() throw (ErroSintatico) {
	match(CASE_);

	expression();

	match(OF_);

	case_limb();

	while(currentToken.id == SEMI_COLON_) {
		match(SEMI_COLON_);

		case_limb();
	}

	if(currentToken.id == SEMI_COLON_)
		match(SEMI_COLON_);

	match(END_);
}

void Sintatico::case_limb() throw (ErroSintatico) {
	case_label_list();

	match(COLON_);

	statement();
}

void Sintatico::case_label_list() throw (ErroSintatico) {
	constant();
	
	while(currentToken.id == COMMA_) {
		match(COMMA_);

		constant();
	}
}

void Sintatico::actual_parameter_list() throw (ErroSintatico) {
	match(LPAREN_);

	expression();

	while(currentToken.id == COMMA_) {
		match(COMMA_);

		expression();
	}

	match(RPAREN_);
}

//END - Statements

//Expressions
void Sintatico::expression()throw (ErroSintatico){
	switch(currentToken.id){
	case SIGN_: 
	case IDENTIFIER_:
	case REAL_NUMBER_:
	case INTEGER_NUMBER_:
	case STRING_:
	case NIL_:
	case LBRAC_:
	case LPAREN_:
	case NOT_:
	    simple_expression();
	    
	    if (currentToken.id==EQUAL_ || currentToken.id==NOTEQUAL_ ||
	        currentToken.id==LT_ || currentToken.id==LE_||
	        currentToken.id==GT_||currentToken.id==GE_||
	        currentToken.id==IN_){
	      
		relational_operator();
		simple_expression();
	    }
	    break;
	case LBRAC_BAR_:
	    
	    list();
	    break;
	    
	default : 
	  throw ErroSintatico((char*)"Era esperado \"+\" , \"-\" , \"[|\" , \"[\", \"(\", \"not\", \"nil\", um Numero, um Identificador ou uma String", currentToken.line);
	}
}


void Sintatico::simple_expression()throw (ErroSintatico){
	if (currentToken.id==SIGN_){
		match(SIGN_);
	}
	
	term();
	
	while(currentToken.id==SIGN_|| currentToken.id==OR_){
		addition_operator();
		
		term();
	}

}

void Sintatico::term()throw (ErroSintatico){
	factor();
	
	while(currentToken.id==OP_MULT_ || currentToken.id==OP_DIV_ || currentToken.id==DIV_
			|| currentToken.id==MOD_ || currentToken.id==AND_){

		multiplication_operator();
	
		factor();
	}
}


void Sintatico::factor()throw (ErroSintatico){
	switch (currentToken.id){
	    
	    case IDENTIFIER_:
	      identifier();
	      factor_aux();
	      break;
	      
	    case INTEGER_NUMBER_:
	    case REAL_NUMBER_:
	      number();
	      break;
	    
	    case STRING_:
	      match(STRING_);
	      break;

	    case LBRAC_:
	      set();
	      break;
	    
	    case NIL_:
	      match(NIL_);
	      break;
	    
	    case LPAREN_:
	      match(LPAREN_);
	      expression();
	      match(RPAREN_);
	      break;
	      
	    case NOT_:
	      match(NOT_);
	      factor();
	      break;

	    default:
		  throw ErroSintatico((char*)"Era esperado \"nil\", \"[\", \"(\", \"not\", um Identificador,  um Numero ou uma String ", currentToken.line);
		}
}

void Sintatico::factor_aux()throw (ErroSintatico){

    if(currentToken.id == LBRAC_ || currentToken.id == DOT_ || currentToken.id == UPARROW_){
	variable_aux();
    }else if (currentToken.id == LPAREN_){
    
      actual_parameter_list();
    }
}
void Sintatico::relational_operator()throw (ErroSintatico){
	switch(currentToken.id){
		case EQUAL_: match(EQUAL_);break;
		case NOTEQUAL_: match(NOTEQUAL_);break;
		case LT_: match(LT_);break;
		case LE_: match(LE_);break;
		case GT_: match(GT_);break;
		case GE_: match(GE_);break;
		case IN_: match(IN_);break;

		default:
			throw ErroSintatico((char*)"Era esperado um operador condicional", currentToken.line);

	}
}

void Sintatico::addition_operator ()throw (ErroSintatico){
	switch(currentToken.id){
		case SIGN_: match(SIGN_);break;
		case OR_: match(OR_);break;

		default:
			throw ErroSintatico((char*)"Era esperado um operador de adição", currentToken.line);

	}
}

void Sintatico::multiplication_operator ()throw (ErroSintatico){
	switch(currentToken.id){
		case OP_MULT_: match(OP_MULT_);break;
		case OP_DIV_: match(OP_DIV_);break;
		case DIV_: match(DIV_);break;
		case MOD_: match(MOD_);break;
		case AND_: match(AND_);break;

		default:
			throw ErroSintatico((char*)"Era esperado um operador de multiplicação", currentToken.line);

	}
}

void Sintatico::variable_aux ()throw (ErroSintatico){
	if(currentToken.id == LBRAC_ || currentToken.id == DOT_ || currentToken.id == UPARROW_ ){
	
	  component_variable();
	  variable_aux();
	}	
}

void Sintatico::component_variable()throw (ErroSintatico){
	switch(currentToken.id){
	  
	  case LBRAC_: indexed_variable();break;
	  case DOT_: field_designator();break;
	  case UPARROW_: match(UPARROW_);break;
	  
	  default:
		throw ErroSintatico((char*)"Era esperado \"[\" , \".\" ou \"^\"", currentToken.line);
	}
}

void Sintatico::indexed_variable()throw (ErroSintatico){
	match(LBRAC_);

	expression_list();

	match(RBRAC_);
}

void Sintatico::field_designator()throw (ErroSintatico){
	match(DOT_);

	identifier();
}

void Sintatico::list()throw (ErroSintatico){
	match(LBRAC_BAR_);
	
	expression_list();
	
	match(RBRAC_BAR_);
	
	list_aux();	
}

void Sintatico::list_aux()throw (ErroSintatico){

    if(currentToken.id==ARROBA_){
      match(ARROBA_);
      
      list();
      
      list_aux();
      
    }else if (currentToken.id==CONS_){
      
      match(CONS_);
      
      factor();
    }
}

void Sintatico::set () throw (ErroSintatico){
	match(LBRAC_);
	
	element_list();

	match(RBRAC_);
}

void Sintatico::element_list () throw (ErroSintatico){

	if(currentToken.id==SIGN_ || currentToken.id==IDENTIFIER_
	  || currentToken.id==REAL_NUMBER_ || currentToken.id==INTEGER_NUMBER_
	  || currentToken.id==STRING_ || currentToken.id==NIL_ 
	  || currentToken.id==LBRAC_  || currentToken.id==LPAREN_ 
	  || currentToken.id==NOT_   || currentToken.id==LBRAC_BAR_){
	  
	    expression();
	    
	    while(currentToken.id==COMMA_){
	    
	      match(COMMA_);
	      expression();
	    }
	  
	}	    
}

//END - Expressions

//Types
void Sintatico::type() throw (ErroSintatico){
	  switch(currentToken.id){
	    case IDENTIFIER_: 
	    case SIGN_:
	    case INTEGER_NUMBER_:
	    case REAL_NUMBER_:
	    case STRING_:
	      simple_type_aux();
	      break;
	      
	    case LPAREN_:
	      enumerated_type();
	      break;
	      
	    case ARRAY_:
	    case RECORD_:
	    case SET_:
	    case LIST_:
	      structured_type();
	      break;
	      
	    case UPARROW_:
	      pointer_type();
	      break;
	      
	    default:
		throw ErroSintatico((char*)"Era esperado \"+\" , \"-\" , \"(\" , \"^\", \"list\", \"set\", \"array\", \"record\", um Identificador, um Numero ou uma String", currentToken.line);
	  }      
 
}

void Sintatico::simple_type() throw (ErroSintatico){
	 switch(currentToken.id){
	    case IDENTIFIER_: 
	    case SIGN_:
	    case INTEGER_NUMBER_:
	    case REAL_NUMBER_:
	    case STRING_:
	      subrange_type();
	      break;
	      
	    case LPAREN_:
	      enumerated_type();
	      break;
	      
	     default:
		throw ErroSintatico((char*)"Era esperado \"+\" , \"-\" , \"(\" , um Identificador, um Numero ou uma String", currentToken.line);
	 }
}

void Sintatico::simple_type_aux() throw (ErroSintatico){
	switch(currentToken.id){
	    case IDENTIFIER_: 
	      match(IDENTIFIER_);
	      
	      if (currentToken.id==DOTDOT_){
	      
		subrange_type_aux();
	      }
	      break;
	      
	    case SIGN_:
	    case INTEGER_NUMBER_:
	    case REAL_NUMBER_:
	    case STRING_:
	      constant_aux();
	      subrange_type_aux();
	      break;
	       
	     default:
		throw ErroSintatico((char*)"Era esperado \"+\" , \"-\" , um Identificador, um Numero ou uma String", currentToken.line);
	 }
  
}
void Sintatico::enumerated_type() throw (ErroSintatico){
	match(LPAREN_);

	identifier_list();

	match(RPAREN_);
}

void Sintatico::subrange_type() throw (ErroSintatico){
	constant();

	match(DOTDOT_);

	constant();
}

void Sintatico::subrange_type_aux() throw (ErroSintatico){
	match(DOTDOT_);

	constant();
}

void Sintatico::constant_aux()throw (ErroSintatico){
	 switch(currentToken.id){
	    
	   case SIGN_: 
	      match(SIGN_);
	      
	      switch(currentToken.id){
		  case INTEGER_NUMBER_: match(INTEGER_NUMBER_);break;
		  case REAL_NUMBER_:match(REAL_NUMBER_);break;
		  case IDENTIFIER_: match(IDENTIFIER_); break;
		  
		  default:
		    throw ErroSintatico((char*)"Era esperado um Numero ou um Identificador", currentToken.line);
	      
	      }
	      break;
	  
	  case INTEGER_NUMBER_: match(INTEGER_NUMBER_);break;
	  case REAL_NUMBER_:match(REAL_NUMBER_);break;
	  case STRING_: match(STRING_); break;
	    
	  default:
	     throw ErroSintatico((char*)"Era esperado um Numero ou uma String", currentToken.line);
      
	    
	 }
  
}

void Sintatico::structured_type() throw (ErroSintatico){
	switch(currentToken.id){
	
	  case ARRAY_: array_type(); break;
	  case RECORD_: record_type(); break;
	  case SET_: set_type(); break;
	  case LIST_: list_type(); break;
	 
	   default:
	     throw ErroSintatico((char*)"Era esperado \"array\", \"record\", \"set\" ou \"list\"", currentToken.line);
      
	}
}

void Sintatico::array_type() throw (ErroSintatico){
	match(ARRAY_);

	match(LBRAC_);

	simple_type();

	while(currentToken.id==COMMA_){
		match(COMMA_);
		
		simple_type();
	}

	match(RBRAC_);

	match (OF_);

	type();
}

void Sintatico::list_type() throw (ErroSintatico){
	match(LIST_);

	match(OF_);

	type();
}

void Sintatico::record_type() throw (ErroSintatico){
	match(RECORD_);

	field_list();

	match(END_);
}

void Sintatico::set_type() throw (ErroSintatico){
	match(SET_);

	match(OF_);

	type();
}

void Sintatico::pointer_type() throw (ErroSintatico){
	match(UPARROW_);

	identifier();
}
//END

// Variable and Identifier Categories
void Sintatico::identifier() throw (ErroSintatico) {
	match(IDENTIFIER_);
}
//END - Variable and Identifier Categories

// Record Fields
void Sintatico::field_list() throw (ErroSintatico) {
	if(currentToken.id == IDENTIFIER_) {
		fixed_part();
		if(currentToken.id == SEMI_COLON_)
			match(SEMI_COLON_);
	}

}

void Sintatico::fixed_part() throw (ErroSintatico) {
	record_section();

	while(currentToken.id == SEMI_COLON_){
		match(SEMI_COLON_);
		record_section();
	}
}

void Sintatico::record_section() throw (ErroSintatico) {
	identifier_list();

	match(COLON_);

	type();
}
//END - Record Fields

// Low Level Definitions

void Sintatico::identifier_list() throw (ErroSintatico) {
	identifier();
	while(currentToken.id == COMMA_){
		match(COMMA_);
		identifier();
	}
}

void Sintatico::expression_list() throw (ErroSintatico) {
	expression();
	while(currentToken.id == COMMA_){
		match(COMMA_);
		expression();
	}
}

void Sintatico::number() throw (ErroSintatico) {
	switch(currentToken.id){
	case INTEGER_NUMBER_:
		match(INTEGER_NUMBER_);
		break;
	case REAL_NUMBER_:
		match(REAL_NUMBER_);
		break;
	default:
		throw ErroSintatico((char*)"Era esperado um Numero", currentToken.line);
	}
}

void Sintatico::constant() throw (ErroSintatico) {
	switch(currentToken.id){
	 
	case SIGN_ :
	     match(SIGN_);
	      
	      switch(currentToken.id){
		  case INTEGER_NUMBER_: match(INTEGER_NUMBER_);break;
		  case REAL_NUMBER_:match(REAL_NUMBER_);break;
		  case IDENTIFIER_: match(IDENTIFIER_); break;
		  
		  default:
		    throw ErroSintatico((char*)"Era esperado um Numero ou um Identificador", currentToken.line);
	      
	      }
	      break;
	case IDENTIFIER_:
		match(IDENTIFIER_);
		break;
	case INTEGER_NUMBER_:
		number();
		break;
	case REAL_NUMBER_:
		number();
		break;
	case STRING_:
		match(STRING_);
		break;
	default:
		throw ErroSintatico((char*)"constant invalido", currentToken.line);
	}
}
// END - Low Level Definitions


int main(){
   Sintatico t;
   try{
      t.parse();
   }catch (ErroSintatico c){
   
     printf("Erro line %d: %s\n", c.getPosition(), c.getMessage());
  }
   return 0;
}
