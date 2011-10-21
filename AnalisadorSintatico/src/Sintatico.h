#ifndef Sintatico_H
#define Sintatico_H

#include "Token.h"
#include "SyntaticError.h"

class Sintatico
{
public:

    Sintatico() {
    	yyin = fopen("Lexico/data.in","r");
    	yyout = fopen("Lexico/data.out","w");
    }

    ~Sintatico()
    {
        if (previousToken != 0 && previousToken != currentToken) delete previousToken;
        if (currentToken != 0)  delete currentToken;
    }

    void parse() throw (AnalysisError);

private:

    Token *currentToken;
    Token *previousToken;

    // Atributos usados para guardar o estado da lista de lexemas.
    Token *tmp_currentToken;
	Token *tmp_previousToken;
	// fim.

    void match(int token) throw (AnalysisError);

	// Guarda o estado da lista de lexemas.
    void saveState();
    // Retorna ao estado da lista de lexemas.
    void loadState();
    //Controe um objeto Token com base no "token" recebido, recuperando a linha e o lexema correspondentes
    Token intToToken(int i);

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
	void procedure_heading() throw (AnalysisError);
	void function_heading() throw (AnalysisError);
	void result_type() throw (AnalysisError);
	void formal_parameter_list() throw (AnalysisError);
	void formal_parameter_section() throw (AnalysisError);
	void value_parameter_section() throw (AnalysisError);
	void variable_parameter_section() throw (AnalysisError);
	void parameter_type() throw (AnalysisError);
	void array_schema() throw (AnalysisError);
	void bound_specification() throw (AnalysisError);
	void ordinal_type_identifier() throw (AnalysisError);
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
	void relational_operator()throw (AnalysisError);
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
	void identifier() throw (AnalysisError);
	void referenced_variable() throw (AnalysisError);
	void record_variable() throw (AnalysisError);
	void pointer_variable() throw (AnalysisError);
	void actual_variable() throw (AnalysisError);
	void array_variable() throw (AnalysisError);
	void field_identifier() throw (AnalysisError);
	void constant_identifier() throw (AnalysisError);
	void variable_identifier() throw (AnalysisError);
	void type_identifier() throw (AnalysisError);
	void procedure_identifier() throw (AnalysisError);
	void function_identifier() throw (AnalysisError);
	void bound_identifier() throw (AnalysisError);
	//END - Variable and Identifier Categories

	//Record Fields
	void field_list() throw (AnalysisError);
	void fixed_part() throw (AnalysisError);
	void record_section() throw (AnalysisError);
	//END - Record Fields

	//Low Level Definitions
	void variable_list() throw (AnalysisError);
	void identifier_list() throw (AnalysisError);
	void expression_list() throw (AnalysisError);
	void number() throw (AnalysisError);
	void integer_number() throw (AnalysisError);
	void real_number() throw (AnalysisError);
	void sign() throw (AnalysisError);
	void stringg() throw (AnalysisError);
	void constant() throw (AnalysisError);
	//END - Low Level Definitions
};

#endif
