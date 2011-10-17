#include "Sintatico.h"

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
                pos = previousToken->getPosition()+previousToken->getLexeme().size();

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
				procedure-declaration();
				break;
			case FUNCTION_:
				function-declaration();
				break;
			default:
				match(SEMI_COLON_);
		}
	}
}

void Sintatico::procedure_declaration() throw (AnalysisError) {
	procedure_heading();

	match(SEMI_COLON_);

	procedure_body();
}

void Sintatico::procedure_body() throw (AnalysisError) {
	block();
}

void Sintatico::function_declaration() throw (AnalysisError) {
	function_heading();

	match(SEMI_COLON_);

	function_body();
}

void Sintatico::function_body() throw (AnalysisError) {
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
	switch (currentToken->getId())
	{
		case IDENTIFIER_:
			// Guarda o estado da lista de lexemas.
			tmp_scanner = this->scanner;
			tmp_currentToken = this->currentToken;
			tmp_previousToken = this->previousToken;
			// fim.
			try {
				simple_statement();
			} catch (AnalysisError) { // Em caso de falha:
				// Retorno o estado da lista de lexemas.
				this->scanner = this->tmp_scanner;
				this->currentToken = this->tmp_currentToken;
				this->previousToken = this->tmp_previousToken;

				structured_statement();
			}
			break;
		default:
			throw SyntaticError(PARSER_ERROR[IDENTIFIER_], currentToken->getPosition());
	}
}

void Sintatico::simple_statement() throw (AnalysisError) {
	// Guarda o estado da lista de lexemas.
	this->tmp_scanner = this->scanner;
	this->tmp_currentToken = this->currentToken;
	this->tmp_previousToken = this->previousToken;
	// fim.
	try {
		assignment_statement();
	} catch (AnalysisError) { // Em caso de falha:
		// Retorno o estado da lista de lexemas.
		this->scanner = this->tmp_scanner;
		this->currentToken = this->tmp_currentToken;
		this->previousToken = this->tmp_previousToken;

		try {
			procedure_statement();
		} catch (AnalysisError) { // Em caso de falha:
			// Retorno o estado da lista de lexemas.
			this->scanner = this->tmp_scanner;
			this->currentToken = this->tmp_currentToken;
			this->previousToken = this->tmp_previousToken;
		}
	}
}

void Sintatico::assignment_statement() throw (AnalysisError) {
	switch (currentToken->getId())
	{
		case IDENTIFIER_:
			// Guarda o estado da lista de lexemas.
			tmp_scanner = this->scanner;
			tmp_currentToken = this->currentToken;
			tmp_previousToken = this->previousToken;
			// fim.
			try {
				simple_statement();
			} catch (AnalysisError) { // Em caso de falha:
				// Retorno o estado da lista de lexemas.
				this->scanner = this->tmp_scanner;
				this->currentToken = this->tmp_currentToken;
				this->previousToken = this->tmp_previousToken;

				structured_statement();
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
	if (currentToken->getPosition() == LPAREN_)
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
			repetitive_statement();
			break;
		case FOR_:
			repetitive_statement();
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

	finalexpression();

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

	if (currentToken->getPosition() == ELSE_){
		match(ELSE_);
		statement();
	}
}

void Sintatico::case_statement() throw (AnalysisError) {
	match(CASE_);

	expression();

	match(OF_);

	case_limb();

	while(currentToken->getPosition() == SEMI_COLON_) {
		macth(SEMI_COLON_);

		case_limb();
	}

	if(currentToken->getPosition() == SEMI_COLON_)
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
	while(currentToken->getPosition() == COMMA_) {
		macth(COMMA_);

		constant();
	}
}

void Sintatico::actual_parameter_list() throw (AnalysisError) {
	match(LPAREN_);

	actual_parameter();

	while(currentToken->getPosition() == COMMA_) {
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
			tmp_scanner = this->scanner;
			tmp_currentToken = this->currentToken;
			tmp_previousToken = this->previousToken;
			// fim.
			try {
				actual_value();
			} catch (AnalysisError) { // Em caso de falha:
				// Retorno o estado da lista de lexemas.
				this->scanner = this->tmp_scanner;
				this->currentToken = this->tmp_currentToken;
				this->previousToken = this->tmp_previousToken;

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
		case LBRAC_:
			actual_value();
			break;
		case LKEY_:
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
