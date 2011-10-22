#include<iostream>
#include "lex.yy.c"
#include "Token.h"
#include "ErroSintatico.h"

using namespace std;

class Sintatico
{
public:
    Sintatico();
    ~Sintatico();
    void parse() throw (ErroSintatico);

//private:

    Token *currentToken;
    Token *previousToken;

    // Atributos usados para guardar o estado da lista de lexemas.
    Token *tmp_currentToken;
    Token *tmp_previousToken;
	// fim.

    void match(int token) throw (ErroSintatico) ;

	// Guarda o estado da lista de lexemas.
    void saveState();
    // Retorna ao estado da lista de lexemas.
    void loadState();
    //Controe um objeto Token com base no "token" recebido, recuperando a linha e o lexema correspondentes
    Token intToToken(int i);

	    //Programs and Blocks
    void program () throw (ErroSintatico) ;
    void program_heading() throw (ErroSintatico) ;
    void block() throw (ErroSintatico) ;
    void declaration_part() throw (ErroSintatico) ;
    void constant_definition_part () throw (ErroSintatico) ;
    void constant_definition() throw (ErroSintatico) ;
    void type_definition_part() throw (ErroSintatico) ;
    void type_definition() throw (ErroSintatico) ;
    void variable_declaration_part() throw (ErroSintatico) ;
    void variable_declaration() throw (ErroSintatico) ;
    void procedure_and_function_declaration_part() throw (ErroSintatico) ;
    void procedure_declaration() throw (ErroSintatico) ;
    void procedure_body() throw (ErroSintatico) ;
    void function_declaration() throw (ErroSintatico) ;
    void function_body() throw (ErroSintatico) ;
    void statement_part() throw (ErroSintatico) ;
    //END - Programs and Blocks

    //Procedure and Function Definitions
    void procedure_heading() throw (ErroSintatico) ;
    void function_heading() throw (ErroSintatico) ;
    void result_type() throw (ErroSintatico) ;
    void formal_parameter_list() throw (ErroSintatico) ;
    void formal_parameter_section() throw (ErroSintatico) ;
    void value_parameter_section() throw (ErroSintatico) ;
    void variable_parameter_section() throw (ErroSintatico) ;
    void parameter_type() throw (ErroSintatico) ;
    void array_schema() throw (ErroSintatico) ;
    void bound_specification() throw (ErroSintatico) ;
    void ordinal_type_identifier() throw (ErroSintatico) ;
    //END - Procedure and Function Definitions

    //Statements
    void statement_sequence() throw (ErroSintatico) ;
    void statement() throw (ErroSintatico) ;
    void simple_statement() throw (ErroSintatico) ;
    void assignment_statement() throw (ErroSintatico) ;
    void procedure_statement() throw (ErroSintatico) ;
    void structured_statement() throw (ErroSintatico) ;
    void compound_statement() throw (ErroSintatico) ;
    void repetitive_statement() throw (ErroSintatico) ;
    void while_statement() throw (ErroSintatico) ;
    void for_statement() throw (ErroSintatico) ;
    void initial_expression() throw (ErroSintatico) ;
    void final_expression() throw (ErroSintatico) ;
    void conditional_statement() throw (ErroSintatico) ;
    void if_statement() throw (ErroSintatico) ;
    void case_statement() throw (ErroSintatico) ;
    void case_limb() throw (ErroSintatico) ;
    void case_label_list() throw (ErroSintatico) ;
    void actual_parameter_list() throw (ErroSintatico) ;
    void actual_parameter() throw (ErroSintatico) ;
    void actual_value() throw (ErroSintatico) ;
    //END - Statements

    //Expressions
    void expression() throw (ErroSintatico) ;
    void simple_expression() throw (ErroSintatico) ;
    void term () throw (ErroSintatico) ;
    void factor () throw (ErroSintatico) ;
    void relational_operator() throw (ErroSintatico) ;
    void addition_operator () throw (ErroSintatico) ;
    void multiplication_operator () throw (ErroSintatico) ;
    void variable () throw (ErroSintatico) ;
    void component_variable() throw (ErroSintatico) ;
    void indexed_variable() throw (ErroSintatico) ;
    void field_designator() throw (ErroSintatico) ;
    void list() throw (ErroSintatico) ;
    void set() throw (ErroSintatico) ;
    void element_list() throw (ErroSintatico) ;
    void function_designator() throw (ErroSintatico) ;
    //END - Expressions

    //Types
    void type() throw (ErroSintatico) ;
    void simple_type() throw (ErroSintatico) ;
    void enumerated_type() throw (ErroSintatico) ;
    void subrange_type() throw (ErroSintatico) ;
    void lower_bound() throw (ErroSintatico) ;
    void upper_bound () throw (ErroSintatico) ;
    void strutured_type() throw (ErroSintatico) ;
    void array_type() throw (ErroSintatico) ;
    void list_type() throw (ErroSintatico) ;
    void index_type() throw (ErroSintatico) ;
    void element_type() throw (ErroSintatico) ;
    void record_type() throw (ErroSintatico) ;
    void set_type() throw (ErroSintatico) ;
    void base_type() throw (ErroSintatico) ;
    void pointer_type() throw (ErroSintatico) ;
    //END

    // Variable and Identifier Categories
    void identifier() throw (ErroSintatico) ;
    void referenced_variable() throw (ErroSintatico) ;
    void record_variable() throw (ErroSintatico) ;
    void pointer_variable() throw (ErroSintatico) ;
    void actual_variable() throw (ErroSintatico) ;
    void array_variable() throw (ErroSintatico) ;
    void field_identifier() throw (ErroSintatico) ;
    void constant_identifier() throw (ErroSintatico) ;
    void variable_identifier() throw (ErroSintatico) ;
    void type_identifier() throw (ErroSintatico) ;
    void procedure_identifier() throw (ErroSintatico) ;
    void function_identifier() throw (ErroSintatico) ;
    void bound_identifier() throw (ErroSintatico) ;
    //END - Variable and Identifier Categories

    //Record Fields
    void field_list() throw (ErroSintatico) ;
    void fixed_part() throw (ErroSintatico) ;
    void record_section() throw (ErroSintatico) ;
    //END - Record Fields

    //Low Level Definitions
    void variable_list() throw (ErroSintatico) ;
    void identifier_list() throw (ErroSintatico) ;
    void expression_list() throw (ErroSintatico) ;
    void number() throw (ErroSintatico) ;
    void integer_number() throw (ErroSintatico) ;
    void real_number() throw (ErroSintatico) ;
    void sign() throw (ErroSintatico) ;
    void stringg() throw (ErroSintatico) ;
    void constant() throw (ErroSintatico) ;
    //END - Low Level Definitions
};
