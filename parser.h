/*
 * parser.h
 *
 *  Created on: Jan 14, 2017
 *      Author: Tommyisr
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "map.h"
#include "store.h"
#include "pokemonGo.h"

#define CHUNK_SIZE 250
/*
 * this function gets an input and an output files and reads the commands
 * from the input file and execute it into the output file
 */
void activatePraser(FILE* input, FILE* output, PokemonGo PG);
/*
 * this function chooses to which function execute according to the given
 * command
 */
void executingCommands(char* command1, char* command2, char* parameters,
		PokemonGo PG, FILE* output);
/*
 * this function turns numbers that are passed as char* to numbers
 */
int funcTranslateCharToInt(char* str);
/*
 * executes related trainer functions
 */
void commandsOfTrainer(char* command2, char* parameters, PokemonGo PG,
		FILE* output);
/*
 * frees all the memory allocation
 */
void charFreeArg(char* arg1, char* arg2, char* arg3, char* arg4);
/*
 * execut related store function
 */
void commandsOfStore(char* command2, char* parameters, PokemonGo PG);
/*
 * execut related battel function
 */
void commandsOfBattle(char* command2, char* parameters, PokemonGo PG,
		FILE* file_o);
/*
 * execut related pokemon function
 */
void commandsOfPokemon(char* command2, char* parameters, PokemonGo PG);
/*
 * execut related report function
 */
void commandsOfReport(char* command2, char* parameters, PokemonGo PG,
		FILE* output);

#endif /* PARSER_H_ */
