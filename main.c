#include "store.h"
#include "evolutions.h"
#include "trainer.h"
#include "pokemon.h"
#include "location.h"
#include "pokemonGo.h"
#include "parser.h"

#define CHUNK_SIZE 250
// All the functions headers
int funcTranslateCharToInt(char* str);
bool readLocations(char line[], Map world, Map pokedex);
bool readEvolutions(char buffer[], Map evolutions);
bool readPokedex(char buffer[], Map pokedex);
bool readFromFile(FILE* file, int arg, Map pokedex, Map evolutions, Map world);
bool openFiles(char**argv, FILE* pokedex_f, FILE* evolutions_f,
		FILE* locations_f, Map pokedex, Map evolutions, Map world);

/*
 * this function opens all the files that are passed by arguments
 * and tries to read from them using the readFromFile func
 */
bool openFiles(char** argv, FILE* pokedex_f, FILE* evolutions_f,
		FILE* locations_f, Map pokedex, Map evolutions, Map world) {
	if (!readFromFile(pokedex_f, 2, pokedex, evolutions, world))
		return false;
	if (!readFromFile(evolutions_f, 4, pokedex, evolutions, world)) {
		fclose(pokedex_f);
		return false;
	}
	if (!readFromFile(locations_f, 6, pokedex, evolutions, world)) {
		fclose(evolutions_f);
		fclose(pokedex_f);
		return false;
	}
	return true;
}
/*
 * reads from files that are passed as an arguments and creates the maps:
 * pokedex, evolutions and world
 */
bool readFromFile(FILE* file, int arg, Map pokedex, Map evolutions, Map world) {
	if (!file) {
		mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
		return false;
	}
	char buffer[CHUNK_SIZE];
	while (fgets(buffer, CHUNK_SIZE, file) != NULL) {
		if (arg == 2) {
			readPokedex(buffer, pokedex);
		} else if (arg == 4) {
			readEvolutions(buffer, evolutions);
		} else {
			readLocations(buffer, world, pokedex);
		}
	}
	return true;
}
/*
 * read the pokodex file and updates the pokodex map
 */
bool readPokedex(char buffer[], Map pokedex) {
	char* str = buffer;
	char* pch;
	pch = strtok(str, " \t");
	char* name = malloc(sizeof(char) * strlen(pch) + 1);
	if (!name)
		return false;
	strcpy(name, pch);
	pch = strtok(NULL, " ,.-");
	int cp;
	cp = funcTranslateCharToInt(pch);
	pch = strtok(NULL, "\n\0");
	char* types = malloc(sizeof(char) * strlen(pch) + 1);
	if (!types)
		return false;
	strcpy(types, pch);
	Pokemon poke = pokemonCreate(cp, name, types);
	if (!poke) {
		free(name);
		free(types);
		return false;
	}
	mapPut(pokedex, name, poke);
	free(name);
	free(types);
	pokemonDestroy(poke);
	return true;
}
/*
 * read the evolutions file and updates the evolutions map
 */
bool readEvolutions(char buffer[], Map evolutions) {
	char* str = buffer;
	char* pch;
	pch = strtok(str, " \t");
	char* nameBefore = malloc(sizeof(char) * strlen(pch) + 1);
	if (!nameBefore)
		return false;
	strcpy(nameBefore, pch);
	pch = strtok(NULL, " ,.-");
	char* nameAfter = malloc(sizeof(char) * strlen(pch) + 1);
	if (!nameAfter)
		return false;
	strcpy(nameAfter, pch);
	pch = strtok(NULL, "\n\0");
	int lvl;
	lvl = funcTranslateCharToInt(pch); // new function char to int
	Evolutions evo = evoCreate(nameBefore, nameAfter, lvl);
	if (!evo) {
		free(nameBefore);
		free(nameAfter);
		return false;
	}
	mapPut(evolutions, nameBefore, evo);
	free(nameBefore);
	free(nameAfter);
	freeEvo(evo);
	return true;
}
/*
 * read the locations file and the pokodex map and updates the world map
 */
bool readLocations(char line[], Map world, Map pokedex) {
	char d_line[strlen(line) + 1];
	strcpy(d_line, line);
	char* first_word = strtok(d_line, " \t;\n");
	if (!first_word)
		return false;
	Location new_location = locationCreate(first_word);
	if (!new_location)
		return false;
	char * file_line = strtok(line, ";\n");
	char* middel_line = strtok(NULL, " \n");
	while (middel_line != NULL) {
		locationAddNearLocation(new_location, middel_line);
		middel_line = strtok(NULL, " \n");
	}
	if (!file_line)
		return false;
	file_line = strtok(file_line, " ");
	while (file_line != NULL) {
		if (strcmp(first_word, file_line) == 0) {
			file_line = strtok(NULL, " ");
			continue;
		}
		Pokemon new_location_pokemon = mapGet(pokedex, file_line);
		if (!new_location_pokemon)
			return false;
		LocationResult result = locationAddPokemon(new_location,
				new_location_pokemon);
		if (result != LOCATION_SUCCESS)
			return false;
		file_line = strtok(NULL, " ");
	}
	mapPut(world, first_word, new_location);
	locationDestroy(new_location);
	return true;
}

/*
 * the main function that executes all the program
 */
int main(int argc, char** argv) {
	FILE* read_f, *output_f;
	if (argc < 7 || argc > 11 || argc == 8 || argc == 10) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
	}
	if (argc == 11 && strcmp(argv[7], "-i") == 0
			&& strcmp(argv[9], "-o") == 0) { //we have them both -i , -o in the correct order
		read_f = fopen(argv[8], "r");
		if (!read_f) {
			mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
			return 0;
		}
		output_f = fopen(argv[10], "a");
		if (!output_f) {
			fclose(read_f);
			mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
			return 0;
		}
	} else if (argc == 7) { // -i, -o does not exists
		read_f = stdin;
		output_f = stdout;
	} else if (strcmp(argv[7], "-i") == 0 && argc == 9) { //if we have only -i
		read_f = fopen(argv[8], "r");
		if (!read_f) {
			mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
			return 0;
		}
		output_f = stdout;
	} else if (strcmp(argv[7], "-o") == 0 && argc == 9) { //if we have only -o
		output_f = fopen(argv[8], "a");
		if (!output_f) {
			mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
			return 0;
		}
		read_f = stdin;
	} else { //-i and -o are in the opposite order
		mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return 0;
	}
	FILE* pokedex_f = fopen(argv[2], "r");
	FILE* evolutions_f = fopen(argv[4], "r");
	FILE* locations_f = fopen(argv[6], "r");
	//create and initilizes all the ADT's
	PokemonGo PG = pokemonGoCreate();
	if (!PG) {
		destroyGameADTS(PG);
		return 0;
	}
	if (!openFiles(argv, pokedex_f, evolutions_f, locations_f,
			(Map) pokemonGoGetPtrByStr(PG, "pokedex"),
			(Map) pokemonGoGetPtrByStr(PG, "evolutions"),
			(Map) pokemonGoGetPtrByStr(PG, "world"))) {
		destroyGameADTS(PG);
		return 0;
	}

	fclose(pokedex_f);
	fclose(evolutions_f);
	fclose(locations_f);
	activatePraser(read_f, output_f, PG);
	if (read_f != stdin)
		fclose(read_f);
	if (output_f != stdout)
		fclose(output_f);
	destroyGameADTS(PG);
	return 0;
}
