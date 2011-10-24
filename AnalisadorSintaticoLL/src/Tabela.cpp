/*
 * Tabela.cpp
 *
 *  Created on: 23/10/2011
 *      Author: Claudio
 */

#include "Tabela.h"

Tabela::Tabela() {
}

Tabela::~Tabela() {
}

void Tabela::buildTabela() {
	//program
	int r_program[3] = {program_heading,block,DOT_};
	tabela[program][PROGRAM_] = r_program;

	//program-heading
	int r_program_heading[3] = {PROGRAM_,IDENTIFIER_,SEMI_COLON_};
	tabela[program_heading][PROGRAM_] = r_program_heading;

	//block
	int r_block[2] = {declaration_part,statement_part};
	tabela[block][CONST_] = r_block;
	tabela[block][TYPE_] = r_block;
	tabela[block][VAR_] = r_block;
	tabela[block][PROCEDURE_] = r_block;
	tabela[block][FUNCTION_] = r_block;
	tabela[block][DOT_] = r_block;
	tabela[block][SEMI_COLON_] = r_block;

	//declaration-part
	int r_declaration_part[4] = {constant_definition_part,type_definition_part,variable_declaration_part,procedure_and_function_declaration_part};
	tabela[declaration_part][CONST_] = r_declaration_part;
	tabela[declaration_part][TYPE_] = r_declaration_part;
	tabela[declaration_part][VAR_] = r_declaration_part;
	tabela[declaration_part][PROCEDURE_] = r_declaration_part;
	tabela[declaration_part][FUNCTION_] = r_declaration_part;
	tabela[declaration_part][BEGIN_] = r_declaration_part;

	//constant-definition-part
//  int r_constant_definition_part[] = {CONST_,constante_definition,SEMI_COLON_, };
//	tabela[constant_definition_part][CONST_]= r_constant_definition_part;

	//constant-definition
	int r_constant_definition[3] = {IDENTIFIER_,EQUAL_,constant};
	tabela[constant_definition][IDENTIFIER_] = r_constant_definition;

	//type-definition-part
//	int r_type_definition_part[] = {TYPE_,type_definition,SEMI_COLON_, };
//	tabela[type_definition_part][TYPE_] = r_type_definition_part;

	//type-definition
	int r_type_definition[3] = {IDENTIFIER_,EQUAL_,type};
	tabela[type_definition][IDENTIFIER_] = r_type_definition;

	//variable-declaration-part
//	int r_variable_declaration_part[] {VAR_,variable_declaration,SEMI_COLOM_, };
//	tabela[variable_declaration_part][VAR_] = r_variable_declaration_part;

	//variable-declaration
	int r_variable_declaration[3] = {identifier_list,COLOM_,type};
	tabela[variable_declaration][IDENTIFIER_] = r_variable_declaration;





}
