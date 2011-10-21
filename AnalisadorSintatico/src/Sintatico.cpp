#include "Sintatico.h"

void Sintatico::saveState(){
	this->tmp_currentToken = this->currentToken;
	this->tmp_previousToken = this->previousToken;
}

void Sintatico::loadState(){
	this->currentToken = this->tmp_currentToken;
	this->previousToken = this->tmp_previousToken;
}

Token Sintatico::intToToken (int i){
	if (i==STRING_)
		return Token (i, stringLexem,yylineno);

	return Token (i, yytext,yylineno);
}

void Sintatico::parse() throw (AnalysisError)
{
    if (previousToken != 0 && previousToken != currentToken)
        delete previousToken;
    previousToken = 0;

    if (currentToken != 0)
        delete currentToken;
    *currentToken = intToToken(yylex());
    if (currentToken == 0)
        currentToken = new Token(DOLLAR, "$", 0);

    program();

    if (currentToken->getId() != DOLLAR)
        throw SyntaticError("DOllar", currentToken->getPosition());
}

void Sintatico::match(int token) throw (AnalysisError)
{
    if (currentToken->getId() == token)
    {
        if (previousToken != 0)
            delete previousToken;
        previousToken = currentToken;
        *currentToken = intToToken(yylex());
        if (currentToken == 0)
        {
            if (previousToken != 0)
            currentToken = new Token(DOLLAR, "$", 0);
        }
    }
    else
        throw SyntaticError("", currentToken->getPosition());
}

//Programs and Blocks
void Sintatico::program () throw (AnalysisError) {
	program_heading();

	block();

	match(DOT_);
}

void Sintatico::program_heading() throw (AnalysisError) {
	match(PROGRAM_);

	identifier();

	match(SEMI_COLON_);
}

void Sintatico::block() throw (AnalysisError) {
	declaration_part();

	statement_part();
}

void Sintatico::declaration_part() throw (AnalysisError) {
	if (currentToken->getId() == CONST_)
		constant_definition_part();

	if (currentToken->getId() == TYPE_)
		type_definition_part();

	if (currentToken->getId() == VAR_)
		variable_declaration_part();

	procedure_and_function_declaration_part();
}

void Sintatico::constant_definition_part () throw (AnalysisError) {
	match(CONST_);

	constant_definition();

	match(SEMI_COLON_);

	while (currentToken->getId() == IDENTIFIER_) {
		constant_definition();
		match(SEMI_COLON_);
	}
}

void Sintatico::constant_definition() throw (AnalysisError) {
	identifier();

	match(EQUAL_);

	constant();
}

void Sintatico::type_definition_part() throw (AnalysisError) {
	match(TYPE_);

	type_definition();

	match(SEMI_COLON_);

	while (currentToken->getId() == IDENTIFIER_) {
		type_definition();

		match(SEMI_COLON_);
	}
}

void Sintatico::type_definition() throw (AnalysisError) {
	identifier();

	match(EQUAL_);

	type();
}

void Sintatico::variable_declaration_part() throw (AnalysisError) {
	match(VAR_);

	variable_declaration();

	match(SEMI_COLON_);

	while (currentToken->getId() == IDENTIFIER_) {
		variable_declaration();

		match(SEMI_COLON_);
	}
}

void Sintatico::variable_declaration() throw (AnalysisError) {
	identifier_list();

	match(COLON_);

	type();
}

void Sintatico::procedure_and_function_declaration_part() throw (AnalysisError) {
	while (currentToken->getId() == FUNCTION_ || currentToken->getId() == PROCEDURE_) {

		if (currentToken->getId() == PROCEDURE_) {
			procedure_declaration();
		} else if (currentToken->getId() == FUNCTION_) {
			function_declaration();
		}
		match(SEMI_COLON_);

	}
}

void Sintatico::procedure_declaration() throw (AnalysisError) {
	procedure_heading();

	match(SEMI_COLON_);

	procedure_body();
}

void Sintatico::procedure_body() throw (AnalysisError){
	block();
}

void Sintatico::function_declaration() throw (AnalysisError) {
	function_heading();

	match(SEMI_COLON_);

	function_body();
}

void Sintatico::function_body() throw(AnalysisError){
	block();
}

void Sintatico::statement_part() throw (AnalysisError) {
	match(BEGIN_);

	statement_sequence();

	match(END_);
}
//END - Programs and Blocks

// Procedure and Function Definitions
void Sintatico::procedure_heading() throw (AnalysisError) {
	match(PROCEDURE_);
	identifier();
	if(currentToken->getId() == LPAREN_)
		formal_parameter_list();
}

void Sintatico::function_heading() throw (AnalysisError) {
	match(FUNCTION_);

	identifier();

	if(currentToken->getId() == LPAREN_)
			formal_parameter_list();

	match(COLON_);

	result_type();
}

void Sintatico::result_type() throw (AnalysisError) {
	type_identifier();
}

void Sintatico::formal_parameter_list() throw (AnalysisError) {
	match(LPAREN_);

	formal_parameter_section();

	while(currentToken->getId() == SEMI_COLON_) {
		match(SEMI_COLON_);

		formal_parameter_section();
	}

	match(RPAREN_);
}

void Sintatico::formal_parameter_section() throw (AnalysisError) {
	switch(currentToken->getId()) {
	case IDENTIFIER_:
		value_parameter_section();
		break;
	case VAR_:
		variable_parameter_section();
		break;
	default:
		throw SyntaticError("formal_parameter_section inv�lido", currentToken->getPosition());
	}
}

void Sintatico::value_parameter_section() throw (AnalysisError) {
	identifier_list();

	match(COLON_);

	parameter_type();
}

void Sintatico::variable_parameter_section() throw (AnalysisError) {
	match(VAR_);

	identifier_list();

	match(COLON_);

	parameter_type();
}

void Sintatico::parameter_type() throw (AnalysisError) {
	switch(currentToken->getId()) {
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
		throw SyntaticError("parameter_type inv�lido", currentToken->getPosition());
	}
}

void Sintatico::array_schema() throw (AnalysisError) {
	match(ARRAY_);

	match(RBRAC_);

	bound_specification();

	while(currentToken->getId() == SEMI_COLON_) {
		match(SEMI_COLON_);

		bound_specification();
	}

	match(LBRAC_);

	match(OF_);

	switch(currentToken->getId()){
	case IDENTIFIER_:
		type_identifier();
		break;
	case ARRAY_:
		array_schema();
		break;
	default:
		throw SyntaticError("array_schema inv�lido", currentToken->getPosition());
	}
}

void Sintatico::bound_specification() throw (AnalysisError) {
	identifier();

	match(DOTDOT_);

	identifier();

	match(COLON_);

	ordinal_type_identifier();
}

void Sintatico::ordinal_type_identifier() throw (AnalysisError) {
	type_identifier();
}
//END - Procedure and Function Definitions

//Statements
void Sintatico::statement_sequence() throw (AnalysisError) {
	statement();

	while(currentToken->getId() == SEMI_COLON_) {
		match(SEMI_COLON_);

		statement();
	}
}

void Sintatico::statement() throw (AnalysisError) {
	if (currentToken->getId() == IDENTIFIER_) {
		simple_statement();
	} else{
		structured_statement();
	}
}

void Sintatico::simple_statement() throw (AnalysisError) {
	// Guarda o estado da lista de lexemas.
	saveState();

	try {
		assignment_statement();
	} catch (AnalysisError) { // Em caso de falha:
		// Retorno o estado da lista de lexemas.
		loadState();

		try {
			procedure_statement();
		} catch (AnalysisError) { // Em caso de falha:
			// Retorno o estado da lista de lexemas.
			loadState();
		}
	}
}

void Sintatico::assignment_statement() throw (AnalysisError) {
	switch (currentToken->getId())
	{
		case IDENTIFIER_:
			// Guarda o estado da lista de lexemas.
			saveState();

			try {
				variable();
			} catch (AnalysisError) { // Em caso de falha:
				// Retorno o estado da lista de lexemas.
				loadState();

				function_identifier();
			}

			match(ASSIGNMENT_);

			expression();
			break;
		default:
			throw SyntaticError("IDENTIFIER_", currentToken->getPosition());
	}
}

void Sintatico::procedure_statement() throw (AnalysisError) {
	procedure_identifier();

	if (currentToken->getId() == LPAREN_)
		actual_parameter_list();
}

void Sintatico::structured_statement() throw (AnalysisError) {
	switch(currentToken->getId())
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
			throw SyntaticError("structured_statement inv�lido", currentToken->getPosition());
	}
}

void Sintatico::compound_statement() throw (AnalysisError) {
	match(BEGIN_);

	statement_sequence();

	match(END_);
}

void Sintatico::repetitive_statement() throw (AnalysisError) {
	switch(currentToken->getId())
	{
		case WHILE_:
			while_statement();
			break;
		case FOR_:
			for_statement();
			break;
		default:
			throw SyntaticError("repetitive_statement inv�lido", currentToken->getPosition());
	}
}

void Sintatico::while_statement() throw (AnalysisError) {
	match(WHILE_);

	expression();

	match(DO_);

	statement();
}

void Sintatico::for_statement() throw (AnalysisError) {
	match(FOR_);

	variable_identifier();

	match(ASSIGNMENT_);

	initial_expression();

	switch(currentToken->getId())
	{
		case TO_:
			match(TO_);
			break;
		case DOWNTO_:
			match(DOWNTO_);
			break;
		default:
			throw SyntaticError("for_statement inv�lido", currentToken->getPosition());
	}

	final_expression();

	match(DO_);

	statement();
}

void Sintatico::initial_expression() throw (AnalysisError) {
	expression();
}

void Sintatico::final_expression() throw (AnalysisError) {
	expression();
}

void Sintatico::conditional_statement() throw (AnalysisError) {
	switch(currentToken->getId())
	{
		case IF_:
			if_statement();
			break;
		case CASE_:
			case_statement();
			break;
		default:
			throw SyntaticError("conditional_statement inv�lido", currentToken->getPosition());
	}
}

void Sintatico::if_statement() throw (AnalysisError) {
	match(IF_);

	expression();

	match(THEN_);

	statement();

	if (currentToken->getId() == ELSE_){
		match(ELSE_);
		statement();
	}
}

void Sintatico::case_statement() throw (AnalysisError) {
	match(CASE_);

	expression();

	match(OF_);

	case_limb();

	while(currentToken->getId() == SEMI_COLON_) {
		match(SEMI_COLON_);

		case_limb();
	}

	if(currentToken->getId() == SEMI_COLON_)
		match(SEMI_COLON_);

	match(END_);
}

void Sintatico::case_limb() throw (AnalysisError) {
	case_label_list();

	match(COLON_);

	statement();
}

void Sintatico::case_label_list() throw (AnalysisError) {
	constant();
	while(currentToken->getId() == COMMA_) {
		match(COMMA_);

		constant();
	}
}

void Sintatico::actual_parameter_list() throw (AnalysisError) {
	match(LPAREN_);

	actual_parameter();

	while(currentToken->getId() == COMMA_) {
		match(COMMA_);

		actual_parameter();
	}

	match(RPAREN_);
}

void Sintatico::actual_parameter() throw (AnalysisError) {
	switch (currentToken->getId())
	{
		case IDENTIFIER_:
			// Guarda o estado da lista de lexemas.
			saveState();

			try {
				actual_value();
			} catch (AnalysisError) { // Em caso de falha:
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
			throw SyntaticError("actual_parameter inv�lido", currentToken->getPosition());
	}
}

void Sintatico::actual_value() throw (AnalysisError) {
	expression();
}
//END - Statements

//Expressions
void Sintatico::expression()throw (AnalysisError){
	simple_expression();

	if (currentToken->getId()==EQUAL_ || currentToken->getId()==NOTEQUAL_ || currentToken->getId()==LT_ || currentToken->getId()==LE_
			|| currentToken->getId()==GT_||currentToken->getId()==GE_||currentToken->getId()==IN_){
		relational_operator();
		simple_expression();
	}

}


void Sintatico::simple_expression()throw (AnalysisError){
	if (currentToken->getId()==PLUS_ ||currentToken->getId()==MINUS_ ){//
		sign();
	}
	term();
	while(currentToken->getId()==PLUS_ || currentToken->getId()== MINUS_ || currentToken->getId()==OR_){
		addition_operator();
		term();
	}

}

void Sintatico::term()throw (AnalysisError){
	factor();
	while(currentToken->getId()==OP_MULT_ || currentToken->getId()==OP_DIV_ || currentToken->getId()==DIV_
			|| currentToken->getId()==MOD_ || currentToken->getId()==AND_){

		multiplication_operator();
		factor();
	}
}


void Sintatico::factor()throw (AnalysisError){
	switch (currentToken->getId())
		{
			case IDENTIFIER_:
				// Guarda o estado da lista de lexemas.
				saveState();

				try {
					variable();
				} catch (AnalysisError) {
					// Retorno o estado da lista de lexemas.
					loadState();

					try{
						constant_identifier();
					}catch (AnalysisError){
						// Retorno o estado da lista de lexemas.
						loadState();

						try{
							bound_identifier();
						}catch(AnalysisError){

							loadState();

							try{
								function_designator();
							}catch (AnalysisError){
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
				throw SyntaticError("Era esperado FACTOR", currentToken->getPosition());
		}
}

void Sintatico::relational_operator()throw (AnalysisError){
	switch(currentToken->getId()){
		case EQUAL_: match(EQUAL_);break;
		case NOTEQUAL_: match(NOTEQUAL_);break;
		case LT_: match(LT_);break;
		case LE_: match(LE_);break;
		case GT_: match(GT_);break;
		case GE_: match(GE_);break;
		case IN_: match(IN_);break;

		default:
			throw SyntaticError("Era esperado um operador condicional", currentToken->getPosition());

	}
}

void Sintatico::addition_operator ()throw (AnalysisError){
	switch(currentToken->getId()){
		case PLUS_: match(PLUS_);break;
		case MINUS_: match(MINUS_);break;
		case OR_: match(OR_);break;

		default:
			throw SyntaticError("Era esperado um operador de adição", currentToken->getPosition());

	}
}

void Sintatico::multiplication_operator ()throw (AnalysisError){
	switch(currentToken->getId()){
		case OP_MULT_: match(OP_MULT_);break;
		case OP_DIV_: match(OP_DIV_);break;
		case DIV_: match(DIV_);break;
		case MOD_: match(MOD_);break;
		case AND_: match(AND_);break;

		default:
			throw SyntaticError("Era esperado um operador de multiplicação", currentToken->getPosition());

	}
}

void Sintatico::variable ()throw (AnalysisError){
	switch(currentToken->getId()){
		case IDENTIFIER_:
			saveState();
			try{
				variable_identifier();
			}catch (AnalysisError){
				loadState();
				try{
					component_variable();
				}catch (AnalysisError){
					loadState();
					try{
						referenced_variable();
					}catch (AnalysisError){
						loadState();
					}
				}
			}
			break;

		default:
			throw SyntaticError("Era esperado um identificador", currentToken->getPosition());
	}
}

void Sintatico::component_variable()throw (AnalysisError){
//n�o precisa verificar novamente se � um identificador pq o metodo s� eh chamado se o token atual for um identificador
	saveState();

	try{
		indexed_variable();
	}catch (AnalysisError){
		loadState();

		try{
			field_designator();
		}catch(AnalysisError){
			loadState();
		}
	}
}

void Sintatico::indexed_variable()throw (AnalysisError){
	array_variable();

	match(LBRAC_);

	expression_list();

	match(RBRAC_);
}

void Sintatico::field_designator()throw (AnalysisError){
	record_variable();

	match(DOT_);

	field_identifier();
}

void Sintatico::list()throw (AnalysisError){
	switch(currentToken->getId()){
		case IDENTIFIER_:
			identifier();

			if(currentToken->getId()==ARROBA_){
				match(ARROBA_);
				list();
			}

			break;
		case LBRAC_BAR_:
			match(LBRAC_BAR_);

			element_list();

			match(RBRAC_BAR_);

			if(currentToken->getId()==ARROBA_){
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

void Sintatico::set () throw (AnalysisError){
	match(LBRAC_);

	element_list();

	match(RBRAC_);
}

void Sintatico::element_list () throw (AnalysisError){

	saveState();
	try{
		expression();
		while(currentToken->getId()){
			match(COLON_);
			expression();
		}
	}catch(AnalysisError){
		loadState();
	}
}

void Sintatico::function_designator () throw (AnalysisError){
	function_identifier();

	if (currentToken->getId() == LPAREN_){
		actual_parameter_list();
	}
}
//END - Expressions

//Types
void Sintatico::type() throw (AnalysisError){
	saveState();
	try{
		simple_type();
	}catch (AnalysisError){
		loadState();
		try{
			strutured_type();
		}catch (AnalysisError){
			loadState();
			try{
				pointer_type();
			}catch(AnalysisError){
				loadState();
				try{
					type_identifier();
				}catch(AnalysisError){
					loadState();
				}
			}
		}
	}
}

void Sintatico::simple_type() throw (AnalysisError){
	saveState();
	try{
		subrange_type();
	}catch (AnalysisError){
		loadState();
		try{
			enumerated_type();
		}catch (AnalysisError){
			loadState();
		}
	}
}

void Sintatico::enumerated_type() throw (AnalysisError){
	match(LPAREN_);

	identifier_list();

	match(RPAREN_);
}

void Sintatico::subrange_type() throw (AnalysisError){
	lower_bound();

	match(DOT_);

	match(DOT_);

	upper_bound();
}

void Sintatico::lower_bound() throw (AnalysisError){
	constant();
}

void Sintatico::upper_bound () throw (AnalysisError){
	constant();
}

void Sintatico::strutured_type() throw (AnalysisError){
	saveState();
	try{
		array_type();
	}catch(AnalysisError){
		loadState();
		try{
			record_type();
		}catch(AnalysisError){
			loadState();
			try{
				set_type();
			}catch(AnalysisError){
				loadState();
				try{
					list_type();
				}catch(AnalysisError){
					loadState();
				}
			}
		}
	}
}

void Sintatico::array_type() throw (AnalysisError){
	match(ARRAY_);

	match(LBRAC_);

	index_type();

	while(currentToken->getId()==COLON_){
		match(COLON_);
		index_type();
	}

	match(RBRAC_);

	match (OF_);

	element_type();
}

void Sintatico::list_type() throw (AnalysisError){
	match(LIST_);

	match(OF_);

	element_type();
}

void Sintatico::index_type() throw (AnalysisError){
	simple_type();
}

void Sintatico::element_type() throw (AnalysisError){
	type();
}

void Sintatico::record_type() throw (AnalysisError){
	match(RECORD_);

	field_list();

	match(END_);
}

void Sintatico::set_type() throw (AnalysisError){
	match(SET_);

	match(OF_);

	base_type();
}

void Sintatico::base_type() throw (AnalysisError){
	type();
}

void Sintatico::pointer_type() throw (AnalysisError){
	match(UPARROW_);

	type_identifier();
}
//END

// Variable and Identifier Categories
void Sintatico::identifier() throw (AnalysisError) {
	match(IDENTIFIER_);
}
void Sintatico::referenced_variable() throw (AnalysisError) {
	pointer_variable();
	match(UPARROW_);
}
void Sintatico::record_variable() throw (AnalysisError) {
	variable();
}
void Sintatico::pointer_variable() throw (AnalysisError) {
	variable();
}
void Sintatico::actual_variable() throw (AnalysisError) {
	variable();
}
void Sintatico::array_variable() throw (AnalysisError) {
	variable();
}
void Sintatico::field_identifier() throw (AnalysisError) {
	identifier();
}
void Sintatico::constant_identifier() throw (AnalysisError) {
	identifier();
}
void Sintatico::variable_identifier() throw (AnalysisError) {
	identifier();
}
void Sintatico::type_identifier() throw (AnalysisError) {
	identifier();
}
void Sintatico::procedure_identifier() throw (AnalysisError) {
	identifier();
}
void Sintatico::function_identifier() throw (AnalysisError) {
	identifier();
}
void Sintatico::bound_identifier() throw (AnalysisError) {
	identifier();
}
//END - Variable and Identifier Categories

// Record Fields
void Sintatico::field_list() throw (AnalysisError) {
	if(currentToken->getId() == IDENTIFIER_) {
		fixed_part();
		if(currentToken->getId() == SEMI_COLON_)
			match(SEMI_COLON_);
	}

}

void Sintatico::fixed_part() throw (AnalysisError) {
	record_section();

	while(currentToken->getId() == SEMI_COLON_){
		match(SEMI_COLON_);
		record_section();
	}
}

void Sintatico::record_section() throw (AnalysisError) {
	identifier_list();

	match(COLON_);

	type();
}
//END - Record Fields

// Low Level Definitions
void Sintatico::variable_list() throw (AnalysisError) {
	variable();
	while(currentToken->getId() == COMMA_){
		match(COMMA_);
		variable();
	}
}

void Sintatico::identifier_list() throw (AnalysisError) {
	identifier();
	while(currentToken->getId() == COMMA_){
		match(COMMA_);
		identifier();
	}
}

void Sintatico::expression_list() throw (AnalysisError) {
	expression();
	while(currentToken->getId() == COMMA_){
		match(COMMA_);
		expression();
	}
}

void Sintatico::number() throw (AnalysisError) {
	switch(currentToken->getId()){
	case INTEGER_NUMBER_:
		integer_number();
		break;
	case REAL_NUMBER_:
		real_number();
		break;
	default:
		throw SyntaticError("number inv�lido", currentToken->getPosition());
	}
}

void Sintatico::integer_number() throw (AnalysisError) {
	match(INTEGER_NUMBER_);
}

void Sintatico::real_number() throw (AnalysisError) {
	match(REAL_NUMBER_);
}

void Sintatico::sign() throw (AnalysisError) {
	switch(currentToken->getId()){
	case PLUS_:
		match(PLUS_);
		break;
	case MINUS_:
		match(MINUS_);
		break;
	default:
		throw SyntaticError("sign inv�lido", currentToken->getPosition());
	}
}

void Sintatico::stringg() throw (AnalysisError) {
	match(STRING_);
}

void Sintatico::constant() throw (AnalysisError) {
	switch(currentToken->getId()){
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
		throw SyntaticError("constant inv�lido", currentToken->getPosition());
	}
}
// END - Low Level Definitions

int main(){
	Sintatico s;
	s.parse();
	return 0;
}
