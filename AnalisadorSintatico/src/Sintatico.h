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

	//Procedure and Function Definitions
	void Sintatico::procedure_heading() throw (AnalysisError);
	void Sintatico::function_heading() throw (AnalysisError);
	void Sintatico::result_type() throw (AnalysisError);
	void Sintatico::formal_parameter_list() throw (AnalysisError);
	void Sintatico::formal_parameter_section() throw (AnalysisError);
	void Sintatico::value_parameter_section() throw (AnalysisError);
	void Sintatico::variable_parameter_section() throw (AnalysisError);
	void Sintatico::parameter_type() throw (AnalysisError);
	void Sintatico::array_schema() throw (AnalysisError);
	void Sintatico::bound_specification() throw (AnalysisError);
	void Sintatico::ordinal_type_identifier() throw (AnalysisError);
	//END - Procedure and Function Definitions

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

	//Types
		void type() throw (AnalysisError);
		void simple_type() throw (AnalysisError);
		void enumerated_type() throw (AnalysisError);
		void subrange_type() throw (AnalysisError);
		void lower_bound() throw (AnalysisError);
		void upper_bound () throw (AnalysisError);
		void strutured_type() throw (AnalysisError);
		void array_type() throw (AnalysisError);
		void list_type() throw (AnalysisError);
		void index_type() throw (AnalysisError);
		void element_type() throw (AnalysisError);
		void record_type() throw (AnalysisError);
		void set_type() throw (AnalysisError);
		void base_type() throw (AnalysisError);
		void pointer_type() throw (AnalysisError);
	//END

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
	//END - Variable and Identifier Categories

	//Record Fields
	void Sintatico::field_list() throw (AnalysisError);
	void Sintatico::fixed_part() throw (AnalysisError);
	void Sintatico::record_section() throw (AnalysisError);
	//END - Record Fields

	//Low Level Definitions
	void Sintatico::variable_list() throw (AnalysisError);
	void Sintatico::identifier_list() throw (AnalysisError);
	void Sintatico::expression_list() throw (AnalysisError);
	void Sintatico::number() throw (AnalysisError);
	void Sintatico::integer_number() throw (AnalysisError);
	void Sintatico::real_number() throw (AnalysisError);
	void Sintatico::sign() throw (AnalysisError);
	void Sintatico::string() throw (AnalysisError);
	void Sintatico::constant() throw (AnalysisError);
	//END - Low Level Definitions
};

#endif
