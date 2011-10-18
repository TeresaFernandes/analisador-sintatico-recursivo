#include "Sintatico.h"

void Sintatico::saveState(){
	this->tmp_scanner = this->scanner;
	this->tmp_currentToken = this->currentToken;
	this->tmp_previousToken = this->previousToken;
}

void Sintatico::loadState(){
	this->scanner = this->tmp_scanner;
	this->currentToken = this->tmp_currentToken;
	this->previousToken = this->tmp_previousToken;
}


void Sintatico::parse(Lexico *scanner, Semantico *semanticAnalyser) throw (AnalysisError)
{
    this->scanner = scanner;
    this->semanticAnalyser = semanticAnalyser;

    if (previousToken != 0 && previousToken != currentToken)
        delete previousToken;
    previousToken = 0;

    if (currentToken != 0)
        delete currentToken;
    currentToken = scanner->nextToken();
    if (currentToken == 0)
        currentToken = new Token(DOLLAR, "$", 0);

    program();

    if (currentToken->getId() != DOLLAR)
        throw SyntaticError(PARSER_ERROR[DOLLAR], currentToken->getPosition());
}

void Sintatico::match(int token) throw (AnalysisError)
{
    if (currentToken->getId() == token)
    {
        if (previousToken != 0)
            delete previousToken;
        previousToken = currentToken;
        currentToken = scanner->nextToken();
        if (currentToken == 0)
        {
            int pos = 0;
            if (previousToken != 0)
                pos = previousToken->getId()+previousToken->getLexeme().size();

            currentToken = new Token(DOLLAR, "$", pos);
        }
    }
    else
        throw SyntaticError(PARSER_ERROR[token], currentToken->getPosition());
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
		variable_definition_part();

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

		switch(currentToken->getId())
		{
			case PROCEDURE_:
				procedure_declaration();
				break;
			case FUNCTION_:
				function_declaration();
				break;
			default:
				//match(SEMI_COLON_);
		}
		match(SEMI_COLON_);//TODO

	}
}

void Sintatico::procedure_declaration() throw (AnalysisError) {
	procedure_heading();

	match(SEMI_COLON_);

	procedure_body();
}

void procedure_body() throw (AnalysisError){
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


//Statements
void Sintatico::statement_sequence() throw (AnalysisError) {
	statement();

	while(currentToken->getId() == SEMI_COLON_) {
		match(SEMI_COLON_);

		statement();
	}
}

void Sintatico::statement() throw (AnalysisError) {
	if (currentToken->getId()==IDENTIFIER_)
	{
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
			throw SyntaticError(PARSER_ERROR[IDENTIFIER_], currentToken->getPosition());
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
			throw SyntaticError(PARSER_ERROR[95], currentToken->getPosition());
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
			throw SyntaticError(PARSER_ERROR[97], currentToken->getPosition());
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
			throw SyntaticError(PARSER_ERROR[100], currentToken->getPosition());
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
			throw SyntaticError(PARSER_ERROR[103], currentToken->getPosition());
	}
}

void Sintatico::if_statement() throw (AnalysisError) {
	match(IF_);

	expression();

	match(THEN_);

	statement();

	if (currentToken->getID() == ELSE_){
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
		macth(SEMI_COLON_);

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
		macth(COMMA_);

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
		case CHAR_://TODO verificar na gramatica a manipulação de 'chars'
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
		case LBRAC_://TODO mudar o simbolo de lista para "[|" e "|]"
			actual_value();
			break;
		case SIGN_://TODO adicionar token no lexico
			actual_value();
			break;
		default:
			throw SyntaticError(PARSER_ERROR[109], currentToken->getPosition());
	}
}

void Sintatico::actual_value() throw (AnalysisError) {
	expression();
}
//END - Statements

//Expressions
void Sintatico::expression()throw (AnalysisError){
	simple_expression();

	if (currentToken->getId()==RELATIONAL_OPERATOR_){//TODO adicionar token no lexico
		relational_operator();
		simple_expression();
	}

}


void Sintatico::simple_expression()throw (AnalysisError){
	if (currentToken->getId()==SIGN_){//TODO adicionar token no lexico
		match(SIGN_);
	}
	term();
	while(currentToken->getId()==ADDITIONAL_OPERATOR_){//TODO adicionar token no lexico
		addition_operator();
		term();
	}

}

void Sintatico::term()throw (AnalysisError){
	factor();
	while(currentToken->getId()==MULTIPLICATION_OPERATOR){//TODO adicionar token no lexico
		multiplication_operator();
		factor();
	}
}


void factor()throw (AnalysisError){
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
							// Retorno o estado da lista de lexemas.
							loadState();

							try{
								function_designator();
							}catch (AnalysisError){
								// Retorno o estado da lista de lexemas.
								loadState();//TODO é para retornar ao estado mesmo?
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
				string();
				break;
			case SET_:
				set();
				break;
			case NIL_:
				match(NIL_);
				break;
			case CHAR_:
				charr();//TODO criar uma definição na gramatica
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
			case LBRAC_:
				list();
				break;
			default:
				throw SyntaticError("Era esperado FACTOR", currentToken->getPosition());
		}
}

void Sintatico::relational_operator()throw (AnalysisError){
	match(RELATIONAL_OPERATOR_);
}

void addition_operator ()throw (AnalysisError){
	match(ADDITIONAL_OPERATOR_);
}

void multiplication_operator ()throw (AnalysisError){
	match(MULTIPLICATION_OPERATOR_);//TODO adicionar token no lexico
}

void variable ()throw (AnalysisError){
	switch(currentToken->getId()){
		case IDENTIFIER_:
			saveStatus();
			try{
				variable_identifier();
			}catch (AnalysisError){
				loadStatus();
				try{
					component_variable();
				}catch (AnalysisError){
					loadStatus();
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
//não precisa verificar novamente se é um identificador pq o metodo só eh chamado se o token atual for um identificador
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

void Sintatico::list()throw (AnalysisError){//TODO mudança da regra de list na gramática para evitar a recursão a esquerda
	switch(currentToken->getId()){
		case IDENTIFIER_:
			identifier();

			if(currentToken->getId()==ARROBA_){
				match(ARROBA_);
				list();
			}

			break;
		case LBRAC_:
			match(LBRAC_BAR);//TODO adicionar token no lexico

			element_list();

			match(RBRAC_BAR);//TODO adicionar token no lexico

			if(currentToken->getId()==ARROBA_){
				match(ARROBA_);
				list();
			}
			break;

		default:
			expression();

			match(CONS_);

			list();
	}

}

void Sintatico::set () throw (AnalysisError){
	match(LBRAC_);

	element_list();

	match(RBRAC_);
}

void Sintatico::element_list () throw (AnalysisError){
//TODO verificar antes se começa com expression, já que é opicional?
	try{
		expression();
		while(currentToken->getId()){
			match(COLON_);
			expression();
		}
	}catch(AnalysisError){

	}
}

void Sintatico::function_designator () throw (AnalysisError){
	function_identifier();

	if (currentToken->getId() == LPAREN_){
		actual_parameter_list();
	}
}
//END - Expressions

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
