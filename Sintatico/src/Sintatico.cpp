#include "Sintatico.h"
Sintatico::Sintatico() {
    yyin = fopen("data.in","r");
    yyout = fopen("data.out","w");
    currentToken=new Token();
    previousToken=new Token();
}

Sintatico::~Sintatico()
{
    if (!previousToken->isNull() && previousToken != currentToken) delete previousToken;
    if (!currentToken->isNull())  delete currentToken;
}
    
void Sintatico::saveState(){
	this->tmp_currentToken = this->currentToken;
	this->tmp_previousToken = this->previousToken;
}

void Sintatico::loadState(){
	this->currentToken = this->tmp_currentToken;
	this->previousToken = this->tmp_previousToken;
}

Token Sintatico::intToToken (int i){
	if (i==STRING_){  
	  return Token(i,--stringLexem,yylineno);
	}
	return Token(i,yytext,yylineno);
}

void Sintatico::parse() throw (ErroSintatico)
{
    if (previousToken != NULL && previousToken != currentToken)
        delete previousToken;
    previousToken = new Token();

    if (currentToken != NULL)
        delete currentToken;
    *currentToken = intToToken(yylex());


    if (currentToken->isNull()){
    	currentToken = new Token(DOLLAR,(char *) "$", 0);
    }
    
    
    program();
  
    if (currentToken->id != DOLLAR)
        throw ErroSintatico((char*)"Dollar", currentToken->line);
}

void Sintatico:: match(int token) throw (ErroSintatico)
{
    
      if (currentToken->id == token)
    {
	printf("match %s\n", currentToken->lexeme);
        if (!previousToken->isNull())
            delete previousToken;
	
        previousToken = currentToken;
        *currentToken = intToToken(yylex());
	printf("currentToken = %d %s %d | priviuosTOken = %d %s %d\n", currentToken->id,currentToken->lexeme, currentToken->line, previousToken->id, previousToken->lexeme, previousToken->line);
	
        if (currentToken->isNull())
        {
            if (!previousToken->isNull()){
            	currentToken = new Token(DOLLAR, (char *)"$", 0);
            }
        }
    }
    else
        throw ErroSintatico((char*)"", currentToken->line);
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
	if (currentToken->id == CONST_)
		constant_definition_part();

	if (currentToken->id == TYPE_)
		type_definition_part();

	if (currentToken->id == VAR_)
		variable_declaration_part();

	procedure_and_function_declaration_part();
}

void Sintatico::constant_definition_part () throw (ErroSintatico) {
	match(CONST_);

	constant_definition();

	match(SEMI_COLON_);

	while (currentToken->id == IDENTIFIER_) {
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

	while (currentToken->id == IDENTIFIER_) {
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

	while (currentToken->id == IDENTIFIER_) {
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
	while (currentToken->id == FUNCTION_ || currentToken->id == PROCEDURE_) {

		if (currentToken->id == PROCEDURE_) {
			procedure_declaration();
		} else if (currentToken->id == FUNCTION_) {
			function_declaration();
		}
		match(SEMI_COLON_);

	}
}

void Sintatico::procedure_declaration() throw (ErroSintatico) {
	procedure_heading();

	match(SEMI_COLON_);

	procedure_body();
}

void Sintatico::procedure_body() throw (ErroSintatico){
	block();
}

void Sintatico::function_declaration() throw (ErroSintatico) {
	function_heading();

	match(SEMI_COLON_);

	function_body();
}

void Sintatico::function_body() throw (ErroSintatico){
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
	if(currentToken->id == LPAREN_)
		formal_parameter_list();
}

void Sintatico::function_heading() throw (ErroSintatico) {
	match(FUNCTION_);

	identifier();

	if(currentToken->id == LPAREN_)
			formal_parameter_list();

	match(COLON_);

	result_type();
}

void Sintatico::result_type() throw (ErroSintatico) {
	type_identifier();
}

void Sintatico::formal_parameter_list() throw (ErroSintatico) {
	match(LPAREN_);

	formal_parameter_section();

	while(currentToken->id == SEMI_COLON_) {
		match(SEMI_COLON_);

		formal_parameter_section();
	}

	match(RPAREN_);
}

void Sintatico::formal_parameter_section() throw (ErroSintatico) {
	switch(currentToken->id) {
	case IDENTIFIER_:
		value_parameter_section();
		break;
	case VAR_:
		variable_parameter_section();
		break;
	default:
		throw ErroSintatico((char*)"formal_parameter_section inv�lido", currentToken->line);
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
	switch(currentToken->id) {
	case IDENTIFIER_:
		type_identifier();
		break;
	case ARRAY_:
		array_schema();
		break;
	case LIST_:
		list_type();
		break;
	default:
		throw ErroSintatico((char*)"parameter_type inv�lido", currentToken->line);
	}
}

void Sintatico::array_schema() throw (ErroSintatico) {
	match(ARRAY_);

	match(RBRAC_);

	bound_specification();

	while(currentToken->id == SEMI_COLON_) {
		match(SEMI_COLON_);

		bound_specification();
	}

	match(LBRAC_);

	match(OF_);

	switch(currentToken->id){
	case IDENTIFIER_:
		type_identifier();
		break;
	case ARRAY_:
		array_schema();
		break;
	default:
		throw ErroSintatico((char*)"array_schema inv�lido", currentToken->line);
	}
}

void Sintatico::bound_specification() throw (ErroSintatico) {
	identifier();

	match(DOTDOT_);

	identifier();

	match(COLON_);

	ordinal_type_identifier();
}

void Sintatico::ordinal_type_identifier() throw (ErroSintatico) {
	type_identifier();
}
//END - Procedure and Function Definitions

//Statements
void Sintatico::statement_sequence() throw (ErroSintatico) {
	statement();

	while(currentToken->id == SEMI_COLON_) {
		match(SEMI_COLON_);

		statement();
	}
}

void Sintatico::statement() throw (ErroSintatico) {
	if (currentToken->id == IDENTIFIER_) {
		simple_statement();
	} else{
		structured_statement();
	}
}

void Sintatico::simple_statement() throw (ErroSintatico) {
	// Guarda o estado da lista de lexemas.
	saveState();

	try {
		assignment_statement();
	} catch (ErroSintatico) { // Em caso de falha:
		// Retorno o estado da lista de lexemas.
		loadState();

		try {
			procedure_statement();
		} catch (ErroSintatico) { // Em caso de falha:
			// Retorno o estado da lista de lexemas.
			loadState();
		}
	}
}

void Sintatico::assignment_statement() throw (ErroSintatico) {
	switch (currentToken->id)
	{
		case IDENTIFIER_:
			// Guarda o estado da lista de lexemas.
			saveState();

			try {
				variable();
			} catch (ErroSintatico) { // Em caso de falha:
				// Retorno o estado da lista de lexemas.
				loadState();

				function_identifier();
			}

			match(ASSIGNMENT_);

			expression();
			break;
		default:
			throw ErroSintatico((char*)"IDENTIFIER_", currentToken->line);
	}
}

void Sintatico::procedure_statement() throw (ErroSintatico) {
	procedure_identifier();

	if (currentToken->id == LPAREN_)
		actual_parameter_list();
}

void Sintatico::structured_statement() throw (ErroSintatico) {
	switch(currentToken->id)
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
			throw ErroSintatico((char*)"structured_statement inv�lido", currentToken->line);
	}
}

void Sintatico::compound_statement() throw (ErroSintatico) {
	match(BEGIN_);

	statement_sequence();

	match(END_);
}

void Sintatico::repetitive_statement() throw (ErroSintatico) {
	switch(currentToken->id)
	{
		case WHILE_:
			while_statement();
			break;
		case FOR_:
			for_statement();
			break;
		default:
			throw ErroSintatico((char*)"repetitive_statement inv�lido", currentToken->line);
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

	variable_identifier();

	match(ASSIGNMENT_);

	initial_expression();

	switch(currentToken->id)
	{
		case TO_:
			match(TO_);
			break;
		case DOWNTO_:
			match(DOWNTO_);
			break;
		default:
			throw ErroSintatico((char*)"for_statement inv�lido", currentToken->line);
	}

	final_expression();

	match(DO_);

	statement();
}

void Sintatico::initial_expression() throw (ErroSintatico) {
	expression();
}

void Sintatico::final_expression() throw (ErroSintatico) {
	expression();
}

void Sintatico::conditional_statement() throw (ErroSintatico) {
	switch(currentToken->id)
	{
		case IF_:
			if_statement();
			break;
		case CASE_:
			case_statement();
			break;
		default:
			throw ErroSintatico((char*)"conditional_statement inv�lido", currentToken->line);
	}
}

void Sintatico::if_statement() throw (ErroSintatico) {
	match(IF_);

	expression();

	match(THEN_);

	statement();

	if (currentToken->id == ELSE_){
		match(ELSE_);
		statement();
	}
}

void Sintatico::case_statement() throw (ErroSintatico) {
	match(CASE_);

	expression();

	match(OF_);

	case_limb();

	while(currentToken->id == SEMI_COLON_) {
		match(SEMI_COLON_);

		case_limb();
	}

	if(currentToken->id == SEMI_COLON_)
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
	while(currentToken->id == COMMA_) {
		match(COMMA_);

		constant();
	}
}

void Sintatico::actual_parameter_list() throw (ErroSintatico) {
	match(LPAREN_);

	actual_parameter();

	while(currentToken->id == COMMA_) {
		match(COMMA_);

		actual_parameter();
	}

	match(RPAREN_);
}

void Sintatico::actual_parameter() throw (ErroSintatico) {
	switch (currentToken->id)
	{
		case IDENTIFIER_:
			// Guarda o estado da lista de lexemas.
			saveState();

			try {
				actual_value();
			} catch (ErroSintatico) { // Em caso de falha:
				// Retorno o estado da lista de lexemas.
				loadState();

				actual_variable();
			}
			break;
		case INTEGER_NUMBER_:
			actual_value();
			break;
		case REAL_NUMBER_:
			actual_value();
			break;
		case STRING_:
			actual_value();
			break;
		case SET_:
			actual_value();
			break;
		case CHAR_:
			actual_value();
			break;
		case NIL_:
			actual_value();
			break;
		case NOT_:
			actual_value();
			break;
		case LPAREN_:
			actual_value();
			break;
		case LBRAC_BAR_:
			actual_value();
			break;
		case MINUS_: //case SIGN_:
			actual_value();
			break;
		case PLUS_:
			actual_value();
			break;
		default:
			throw ErroSintatico((char*)"actual_parameter inv�lido", currentToken->line);
	}
}

void Sintatico::actual_value() throw (ErroSintatico) {
	expression();
}
//END - Statements

//Expressions
void Sintatico::expression()throw (ErroSintatico){
	simple_expression();

	if (currentToken->id==EQUAL_ || currentToken->id==NOTEQUAL_ || currentToken->id==LT_ || currentToken->id==LE_
			|| currentToken->id==GT_||currentToken->id==GE_||currentToken->id==IN_){
		relational_operator();
		simple_expression();
	}

}


void Sintatico::simple_expression()throw (ErroSintatico){
	if (currentToken->id==PLUS_ ||currentToken->id==MINUS_ ){//
		sign();
	}
	term();
	while(currentToken->id==PLUS_ || currentToken->id== MINUS_ || currentToken->id==OR_){
		addition_operator();
		term();
	}

}

void Sintatico::term()throw (ErroSintatico){
	factor();
	while(currentToken->id==OP_MULT_ || currentToken->id==OP_DIV_ || currentToken->id==DIV_
			|| currentToken->id==MOD_ || currentToken->id==AND_){

		multiplication_operator();
		factor();
	}
}


void Sintatico::factor()throw (ErroSintatico){
	switch (currentToken->id)
		{
			case IDENTIFIER_:
				// Guarda o estado da lista de lexemas.
				saveState();

				try {
					variable();
				} catch (ErroSintatico) {
					// Retorno o estado da lista de lexemas.
					loadState();

					try{
						constant_identifier();
					}catch (ErroSintatico){
						// Retorno o estado da lista de lexemas.
						loadState();

						try{
							bound_identifier();
						}catch(ErroSintatico){

							loadState();

							try{
								function_designator();
							}catch (ErroSintatico){
								loadState();
							}
						}
					}
				}
				break;
			case INTEGER_NUMBER_:
				number();
				break;
			case REAL_NUMBER_:
				number();
				break;
			case STRING_:
				stringg();
				break;
			case SET_:
				set();
				break;
			case NIL_:
				match(NIL_);
				break;
			case CHAR_:
				//TODO charr(); criar uma defini��o na gramatica
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
			case LBRAC_BAR_:
				list();
				break;
			default:
				throw ErroSintatico((char*)"Era esperado FACTOR", currentToken->line);
		}
}

void Sintatico::relational_operator()throw (ErroSintatico){
	switch(currentToken->id){
		case EQUAL_: match(EQUAL_);break;
		case NOTEQUAL_: match(NOTEQUAL_);break;
		case LT_: match(LT_);break;
		case LE_: match(LE_);break;
		case GT_: match(GT_);break;
		case GE_: match(GE_);break;
		case IN_: match(IN_);break;

		default:
			throw ErroSintatico((char*)"Era esperado um operador condicional", currentToken->line);

	}
}

void Sintatico::addition_operator ()throw (ErroSintatico){
	switch(currentToken->id){
		case PLUS_: match(PLUS_);break;
		case MINUS_: match(MINUS_);break;
		case OR_: match(OR_);break;

		default:
			throw ErroSintatico((char*)"Era esperado um operador de adição", currentToken->line);

	}
}

void Sintatico::multiplication_operator ()throw (ErroSintatico){
	switch(currentToken->id){
		case OP_MULT_: match(OP_MULT_);break;
		case OP_DIV_: match(OP_DIV_);break;
		case DIV_: match(DIV_);break;
		case MOD_: match(MOD_);break;
		case AND_: match(AND_);break;

		default:
			throw ErroSintatico((char*)"Era esperado um operador de multiplicação", currentToken->line);

	}
}

void Sintatico::variable ()throw (ErroSintatico){
	switch(currentToken->id){
		case IDENTIFIER_:
			saveState();
			try{
				variable_identifier();
			}catch (ErroSintatico){
				loadState();
				try{
					component_variable();
				}catch (ErroSintatico){
					loadState();
					try{
						referenced_variable();
					}catch (ErroSintatico){
						loadState();
					}
				}
			}
			break;

		default:
			throw ErroSintatico((char*)"Era esperado um identificador", currentToken->line);
	}
}

void Sintatico::component_variable()throw (ErroSintatico){
//n�o precisa verificar novamente se � um identificador pq o metodo s� eh chamado se o token atual for um identificador
	saveState();

	try{
		indexed_variable();
	}catch (ErroSintatico){
		loadState();

		try{
			field_designator();
		}catch(ErroSintatico){
			loadState();
		}
	}
}

void Sintatico::indexed_variable()throw (ErroSintatico){
	array_variable();

	match(LBRAC_);

	expression_list();

	match(RBRAC_);
}

void Sintatico::field_designator()throw (ErroSintatico){
	record_variable();

	match(DOT_);

	field_identifier();
}

void Sintatico::list()throw (ErroSintatico){
	switch(currentToken->id){
		case IDENTIFIER_:
			identifier();

			if(currentToken->id==ARROBA_){
				match(ARROBA_);
				list();
			}

			break;
		case LBRAC_BAR_:
			match(LBRAC_BAR_);

			element_list();

			match(RBRAC_BAR_);

			if(currentToken->id==ARROBA_){
				match(ARROBA_);
				list();
			}
			break;

		default:
			expression();

			match(CONS_);

			list();
			break;
	}

}

void Sintatico::set () throw (ErroSintatico){
	match(LBRAC_);

	element_list();

	match(RBRAC_);
}

void Sintatico::element_list () throw (ErroSintatico){

	saveState();
	try{
		expression();
		while(currentToken->id){
			match(COLON_);
			expression();
		}
	}catch(ErroSintatico){
		loadState();
	}
}

void Sintatico::function_designator () throw (ErroSintatico){
	function_identifier();

	if (currentToken->id == LPAREN_){
		actual_parameter_list();
	}
}
//END - Expressions

//Types
void Sintatico::type() throw (ErroSintatico){
	saveState();
	try{
		simple_type();
	}catch (ErroSintatico){
		loadState();
		try{
			strutured_type();
		}catch (ErroSintatico){
			loadState();
			try{
				pointer_type();
			}catch(ErroSintatico){
				loadState();
				try{
					type_identifier();
				}catch(ErroSintatico){
					loadState();
				}
			}
		}
	}
}

void Sintatico::simple_type() throw (ErroSintatico){
	saveState();
	try{
		subrange_type();
	}catch (ErroSintatico){
		loadState();
		try{
			enumerated_type();
		}catch (ErroSintatico){
			loadState();
		}
	}
}

void Sintatico::enumerated_type() throw (ErroSintatico){
	match(LPAREN_);

	identifier_list();

	match(RPAREN_);
}

void Sintatico::subrange_type() throw (ErroSintatico){
	lower_bound();

	match(DOT_);

	match(DOT_);

	upper_bound();
}

void Sintatico::lower_bound() throw (ErroSintatico){
	constant();
}

void Sintatico::upper_bound () throw (ErroSintatico){
	constant();
}

void Sintatico::strutured_type() throw (ErroSintatico){
	saveState();
	try{
		array_type();
	}catch(ErroSintatico){
		loadState();
		try{
			record_type();
		}catch(ErroSintatico){
			loadState();
			try{
				set_type();
			}catch(ErroSintatico){
				loadState();
				try{
					list_type();
				}catch(ErroSintatico){
					loadState();
				}
			}
		}
	}
}

void Sintatico::array_type() throw (ErroSintatico){
	match(ARRAY_);

	match(LBRAC_);

	index_type();

	while(currentToken->id==COLON_){
		match(COLON_);
		index_type();
	}

	match(RBRAC_);

	match (OF_);

	element_type();
}

void Sintatico::list_type() throw (ErroSintatico){
	match(LIST_);

	match(OF_);

	element_type();
}

void Sintatico::index_type() throw (ErroSintatico){
	simple_type();
}

void Sintatico::element_type() throw (ErroSintatico){
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

	base_type();
}

void Sintatico::base_type() throw (ErroSintatico){
	type();
}

void Sintatico::pointer_type() throw (ErroSintatico){
	match(UPARROW_);

	type_identifier();
}
//END

// Variable and Identifier Categories
void Sintatico::identifier() throw (ErroSintatico) {
	match(IDENTIFIER_);
}
void Sintatico::referenced_variable() throw (ErroSintatico) {
	pointer_variable();
	match(UPARROW_);
}
void Sintatico::record_variable() throw (ErroSintatico) {
	variable();
}
void Sintatico::pointer_variable() throw (ErroSintatico) {
	variable();
}
void Sintatico::actual_variable() throw (ErroSintatico) {
	variable();
}
void Sintatico::array_variable() throw (ErroSintatico) {
	variable();
}
void Sintatico::field_identifier() throw (ErroSintatico) {
	identifier();
}
void Sintatico::constant_identifier() throw (ErroSintatico) {
	identifier();
}
void Sintatico::variable_identifier() throw (ErroSintatico) {
	identifier();
}
void Sintatico::type_identifier() throw (ErroSintatico) {
	identifier();
}
void Sintatico::procedure_identifier() throw (ErroSintatico) {
	identifier();
}
void Sintatico::function_identifier() throw (ErroSintatico) {
	identifier();
}
void Sintatico::bound_identifier() throw (ErroSintatico) {
	identifier();
}
//END - Variable and Identifier Categories

// Record Fields
void Sintatico::field_list() throw (ErroSintatico) {
	if(currentToken->id == IDENTIFIER_) {
		fixed_part();
		if(currentToken->id == SEMI_COLON_)
			match(SEMI_COLON_);
	}

}

void Sintatico::fixed_part() throw (ErroSintatico) {
	record_section();

	while(currentToken->id == SEMI_COLON_){
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
void Sintatico::variable_list() throw (ErroSintatico) {
	variable();
	while(currentToken->id == COMMA_){
		match(COMMA_);
		variable();
	}
}

void Sintatico::identifier_list() throw (ErroSintatico) {
	identifier();
	while(currentToken->id == COMMA_){
		match(COMMA_);
		identifier();
	}
}

void Sintatico::expression_list() throw (ErroSintatico) {
	expression();
	while(currentToken->id == COMMA_){
		match(COMMA_);
		expression();
	}
}

void Sintatico::number() throw (ErroSintatico) {
	switch(currentToken->id){
	case INTEGER_NUMBER_:
		integer_number();
		break;
	case REAL_NUMBER_:
		real_number();
		break;
	default:
		throw ErroSintatico((char*)"number inv�lido", currentToken->line);
	}
}

void Sintatico::integer_number() throw (ErroSintatico) {
	match(INTEGER_NUMBER_);
}

void Sintatico::real_number() throw (ErroSintatico) {
	match(REAL_NUMBER_);
}

void Sintatico::sign() throw (ErroSintatico) {
	switch(currentToken->id){
	case PLUS_:
		match(PLUS_);
		break;
	case MINUS_:
		match(MINUS_);
		break;
	default:
		throw ErroSintatico((char*)"sign inv�lido", currentToken->line);
	}
}

void Sintatico::stringg() throw (ErroSintatico) {
	match(STRING_);
}

void Sintatico::constant() throw (ErroSintatico) {
	switch(currentToken->id){
	case IDENTIFIER_:
		constant_identifier();
		break;
	case INTEGER_NUMBER_:
		number();
		break;
	case REAL_NUMBER_:
		number();
		break;
	case STRING_:
		stringg();
		break;
	default:
		throw ErroSintatico((char*)"constant invalido", currentToken->line);
	}
}
// END - Low Level Definitions


int main(){
   Sintatico t;
   t.parse();
   return 0;
}
