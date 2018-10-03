#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "map.h"
#include "store.h"
#include "pokemonGo.h"
#include "parser.h"
#define CHUNK_SIZE 250

/*
 * this function gets an input and an output files and reads the commands
 * from the input file and execute it into the output file
 */
void activatePraser(FILE* input, FILE* output, PokemonGo PG) {
	if (!input || !output) {
		mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
		return;
	}
	char buffer[CHUNK_SIZE];
	while (fgets(buffer, CHUNK_SIZE, input) != NULL) {
		char* str = buffer;
		char* pch;
		pch = strtok(str, " \t\n\0");
		if (!pch)
			continue;
		char* command1 = malloc(sizeof(char) * strlen(pch) + 1);
		if (!command1)
			continue;
		strcpy(command1, pch);
		pch = strtok(NULL, " \t\n\0");
		char* command2 = malloc(sizeof(char) * strlen(pch) + 1);
		if (!command2)
			continue;
		strcpy(command2, pch);
		pch = strtok(NULL, "\n\0");
		char* parameteres;
		if (pch) {
			parameteres = malloc(sizeof(char) * strlen(pch) + 1);
			if (!parameteres)
				continue;
			strcpy(parameteres, pch);
		} else
			parameteres = NULL;
		executingCommands(command1, command2, parameteres, PG, output);
		free(command1);
		free(command2);
		free(parameteres);
	}
}
/*
 * this function chooses to which function execute according to the given
 * command
 */
void executingCommands(char* command1, char* command2, char* parameters,
		PokemonGo PG, FILE* output) {
	if (strcmp(command1, "trainer") == 0) {
		commandsOfTrainer(command2, parameters, PG, output);
	}
	if (strcmp(command1, "store") == 0) {
		commandsOfStore(command2, parameters, PG);
	}
	if (strcmp(command1, "battle") == 0) {
		commandsOfBattle(command2, parameters, PG, output);
	}
	if (strcmp(command1, "pokemon") == 0) {
		commandsOfPokemon(command2, parameters, PG);
	}
	if (strcmp(command1, "report") == 0) {
		commandsOfReport(command2, parameters, PG, output);
	}
}
/*
 * this function turns numbers that are passed as char* to numbers
 */
int funcTranslateCharToInt(char* str) {
	int a, sum = 0;
	int len = strlen(str);
	bool minus = true;
	int i = 0;
	if (str[0] == '-') {
		i++;
		minus = false;
	}
	while (i < len) {
		sum = sum * 10;
		a = str[i] - '0';
		sum += a;
		i++;
	}
	if (minus == false)
		sum = -sum;
	return sum;
}
/*
 * executes related trainer functions
 */

void commandsOfTrainer(char* command2, char* parameters, PokemonGo PG,
		FILE* output) {
	if (!command2 || !parameters || !PG)
		return;
	char* arg1, *arg2, *arg3;
	char* pch;
	pch = strtok(parameters, " \t");
	arg1 = malloc(sizeof(char) * strlen(pch) + 1);
	if (!arg1)
		return;
	strcpy(arg1, pch);
	pch = strtok(NULL, " \t\n");
	arg2 = malloc(sizeof(char) * strlen(pch) + 1);
	if (!arg2) {
		free(arg1);
		return;
	}
	strcpy(arg2, pch);
	pch = strtok(NULL, " \t\n");
	if (pch) {
		arg3 = malloc(sizeof(char) * strlen(pch) + 1);
		if (!arg3) {
			charFreeArg(arg1, arg2, NULL, NULL);
			return;
		}
		strcpy(arg3, pch);
	} else
		arg3 = NULL;
	if (strcmp(command2, "add") == 0) {
		int money = funcTranslateCharToInt(arg2);
		trainerAdd(PG, arg1, money, arg3, output);
	} else if (strcmp(command2, "purchase") == 0) {
		int value = funcTranslateCharToInt(arg3);
		trainerPurchase(PG, arg1, arg2, value);
	} else {
		trainerGo(PG, arg1, arg2, output);
	}
	charFreeArg(arg1, arg2, arg3, NULL);
	return;
}
/*
 * frees all the memory allocation
 */
void charFreeArg(char* arg1, char* arg2, char* arg3, char* arg4) {
	if (arg3 != NULL)
		free(arg3);
	if (arg4 != NULL)
		free(arg4);
	free(arg1);
	free(arg2);
}
/*
 * execut related store function
 */
void commandsOfStore(char* command2, char* parameters, PokemonGo PG) {
	if (!command2 || !parameters || !PG)
		return;
	char* pch;
	pch = strtok(parameters, " \t");
	char* arg1 = malloc(sizeof(char) * strlen(pch) + 1);
	if (!arg1)
		return;
	strcpy(arg1, pch);
	pch = strtok(NULL, " \t\n\0");
	char* arg2 = malloc(sizeof(char) * strlen(pch) + 1);
	if (!arg2) {
		free(arg1);
		return;
	}
	strcpy(arg2, pch);
	pch = strtok(NULL, " \t\n\0");
	char* arg3 = malloc(sizeof(char) * strlen(pch) + 1);
	if (!arg3) {
		charFreeArg(arg1, arg2, NULL, NULL);
		return;
	}
	strcpy(arg3, pch);
	int value = funcTranslateCharToInt(arg2);
	int quantity = funcTranslateCharToInt(arg3);
	storeAddItem(PG, arg1, value, quantity);
	charFreeArg(arg1, arg2, arg3, NULL);
	return;
}
/*
 * execut related battel function
 */
void commandsOfBattle(char* command2, char* parameters, PokemonGo PG,
		FILE* file_o) {
	if (!command2 || !parameters || !PG)
		return;
	char *arg1 = NULL, *arg2 = NULL, *arg3 = NULL, *arg4 = NULL;
	char* pch;
	pch = strtok(parameters, " \t");
	arg1 = malloc(sizeof(char) * strlen(pch) + 1);
	if (!arg1)
		return;
	strcpy(arg1, pch);
	pch = strtok(NULL, " \t\n\0");
	arg2 = malloc(sizeof(char) * strlen(pch) + 1);
	if (!arg2) {
		free(arg1);
		return;
	}
	strcpy(arg2, pch);
	pch = strtok(NULL, " \t\n\0");
	arg3 = malloc(sizeof(char) * strlen(pch) + 1);
	if (!arg3) {
		charFreeArg(arg1, arg2, NULL, NULL);
		return;
	}
	strcpy(arg3, pch);
	pch = strtok(NULL, " \t\n\0");
	arg4 = malloc(sizeof(char) * strlen(pch) + 1);
	if (!arg4) {
		charFreeArg(arg1, arg2, arg3, NULL);
		return;
	}
	strcpy(arg4, pch);
	int pokemon1 = funcTranslateCharToInt(arg3);
	int pokemon2 = funcTranslateCharToInt(arg4);
	battleFight(PG, arg1, arg2, pokemon1, pokemon2, file_o);
	charFreeArg(arg1, arg2, arg3, arg4);
	return;
}
/*
 * execut related pokemon function
 */
void commandsOfPokemon(char* command2, char* parameters, PokemonGo PG) {
	if (!command2 || !parameters || !PG)
		return;
	char* pch;
	pch = strtok(parameters, " \t");
	char* arg1 = malloc(sizeof(char) * strlen(pch) + 1);
	if (!arg1)
		return;
	strcpy(arg1, pch);
	pch = strtok(NULL, " \t\n\0");
	char* arg2 = malloc(sizeof(char) * strlen(pch) + 1);
	if (!arg2) {
		free(arg1);
		return;
	}
	strcpy(arg2, pch);
	int id = funcTranslateCharToInt(arg2);
	if (strcmp(command2, "heal") == 0)
		pokemonHeal(PG, arg1, id);
	else
		pokemonTrain(PG, arg1, id);
	charFreeArg(arg1, arg2, NULL, NULL);
	return;
}
/*
 * execut related report function
 */
void commandsOfReport(char* command2, char* parameters, PokemonGo PG,
		FILE* output) {
	if (!command2 || !PG || !output)
		return;

	if (strcmp(command2, "locations") == 0) {
		reportLocations(PG, output);
		return;
	} else if (strcmp(command2, "stock") == 0) {
		reportStock(PG, output);
		return;
	}

	char* pch;
	pch = strtok(parameters, " \t");
	char* arg1 = malloc(sizeof(char) * strlen(pch) + 1);
	if (!arg1)
		return;
	strcpy(arg1, pch);
	reportTrainer(PG, arg1, output);
	charFreeArg(arg1, NULL, NULL, NULL);
	return;
}
