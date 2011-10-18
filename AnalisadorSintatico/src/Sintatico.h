#ifndef Sintatico_H
#define Sintatico_H

#include "Constants.h"
#include "Token.h"
#include "Lexico.h"
#include "Semantico.h"
#include "SyntaticError.h"

class Sintatico
{
public:
    Sintatico() : previousToken(0), currentToken(0) { }

    ~Sintatico()
    {
        if (previousToken != 0 && previousToken != currentToken) delete previousToken;
        if (currentToken != 0)  delete currentToken;
    }

    void parse(Lexico *scanner, Semantico *semanticAnalyser) throw (AnalysisError);

private:
    Token *currentToken;
    Token *previousToken;
    Lexico *scanner;
    Semantico *semanticAnalyser;

    // Atributos usados para guardar o estado da lista de lexemas.
    Token *tmp_currentToken;
	Token *tmp_previousToken;
	Lexico *tmp_scanner;
	// fim.

    void match(int token) throw (AnalysisError);

	// Guarda o estado da lista de lexemas.
    void saveState();
    // Retorna ao estado da lista de lexemas.
    void loadState();

    //Programs and Blocks
	void program () throw (AnalysisError);
	void program_heading() throw (AnalysisError);
	void block() throw (AnalysisError);
	void declaration_part() throw (AnalysisError);
	void constant_definition_part () throw (AnalysisError);
	void constant_definition() throw (AnalysisError);
	void type_definition_part() throw (AnalysisError);
	void type_definition() throw (AnalysisError);
	void variable_declaration_part() throw (AnalysisError);
	void variable_declaration() throw (AnalysisError);
	void procedure_and_function_declaration_part() throw (AnalysisError);
	void procedure_declaration() throw (AnalysisError);
	void procedure_body() throw (AnalysisError);
	void function_declaration() throw (AnalysisError);
	void function_body() throw (AnalysisError);
	void statement_part() throw (AnalysisError);
	//END - Programs and Blocks

	//Statements
	void statement_sequence() throw (AnalysisError);
	void statement() throw (AnalysisError);
	void simple_statement() throw (AnalysisError);
	void assignment_statement() throw (AnalysisError);
	void procedure_statement() throw (AnalysisError);
	void structured_statement() throw (AnalysisError);
	void compound_statement() throw (AnalysisError);
	void repetitive_statement() throw (AnalysisError);
	void while_statement() throw (AnalysisError);
	void for_statement() throw (AnalysisError);
	void initial_expression() throw (AnalysisError);
	void final_expression() throw (AnalysisError);
	void conditional_statement() throw (AnalysisError);
	void if_statement() throw (AnalysisError);
	void case_statement() throw (AnalysisError);
	void case_limb() throw (AnalysisError);
	void case_label_list() throw (AnalysisError);
	void actual_parameter_list() throw (AnalysisError);
	void actual_parameter() throw (AnalysisError);
	void actual_value() throw (AnalysisError);
	//END - Statements

	//Expressions
	void expression()throw (AnalysisError);
	void simple_expression()throw (AnalysisError);
	void term ()throw (AnalysisError);
	void factor ()throw (AnalysisError);
	bool relational_operator ()throw (AnalysisError);
	void addition_operator ()throw (AnalysisError);
	void multiplication_operator ()throw (AnalysisError);
	void variable ()throw (AnalysisError);
	void component_variable()throw (AnalysisError);
	void indexed_variable()throw (AnalysisError);
	void field_designator()throw (AnalysisError);
	void list()throw (AnalysisError);
	void set()throw (AnalysisError);
	void element_list()throw (AnalysisError);
	void function_designator()throw (AnalysisError);
	//END - Expressions

	// Variable and Identifier Categories
	void Sintatico::identifier() throw (AnalysisError);
	void Sintatico::referenced_variable() throw (AnalysisError);
	void Sintatico::record_variable() throw (AnalysisError);
	void Sintatico::pointer_variable() throw (AnalysisError);
	void Sintatico::actual_variable() throw (AnalysisError);
	void Sintatico::array_variable() throw (AnalysisError);
	void Sintatico::field_identifier() throw (AnalysisError);
	void Sintatico::constant_identifier() throw (AnalysisError);
	void Sintatico::variable_identifier() throw (AnalysisError);
	void Sintatico::type_identifier() throw (AnalysisError);
	void Sintatico::procedure_identifier() throw (AnalysisError);
	void Sintatico::function_identifier() throw (AnalysisError);
	void Sintatico::bound_identifier() throw (AnalysisError);
};

#endif
