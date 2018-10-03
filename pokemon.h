/*
 * pokemon.h
 *
 *  Created on: 20 בדצמ× 2016
 *      Author: Dvir
 */

#ifndef POKEMON_H_
#define POKEMON_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "print_utils.h"

#define MAX_HP 100

/*
 * all the possibilities for a result as for Pokemon
 */
typedef enum PokemonResult_e {
	POKEMON_SUCCESS,
	POKEMON_TYPE_ALREADY_EXISTS,
	POKEMON_OUT_OF_MEMORY,
	POKEMON_NULL_ARGUMENT,
	POKEMON_ITEM_DOES_NOT_EXIST,
	POKEMON_IS_UP_TO_DATE,
	POKEMON_FAIL
} PokemonResult;
/*
 * the struct declaration for a Pokemon
 */
typedef struct pokemon_s *Pokemon;


/*
 * @param
 * int cp, char* name, char* type
 * creates a new pokemon and return it
 */
Pokemon pokemonCreate(int cp, char* name, char* type);
/*
 * @param Pokemon pokemon
 * a mask function for the pokemonIsTypeSpecialCover
 * which check and transfer the pokemon
 * if it gets NULL returns -1
 * else returns 10/20/30
 */
int pokemonIsTypeSpecial(Pokemon pokemon);
/*
 * @param Pokemon pokemon
 * allocate and create a copy of a given Pokemon.
 * returns NULL if the process has failed
 */
Pokemon pokemonCopy(Pokemon old_pokemon);
/*
 * @param Pokemon pokemon
 * free all the allocated space of a pokemon
 */
void pokemonDestroy(Pokemon pokemon);
/*
 * @param Pokemon pokemon
 * returns the Types of a pokemon
 */
char* pokemonGetTypes(Pokemon pokemon);
/*
 * @param Pokemon pokemon
 * update all the pokemons attributes after a fight
 * update the cp with the its new value - due to a candy
 * update the two pokemons after they have attacked each other
 * and returns as a reference the two new xp of the trainers
 * @return
 * POKEMON_NULL_ARGUMENT - gets a NULL as pokemon
 * POKEMON_SUCCESS - update the pokemons attributes
 */
PokemonResult pokemonAfterATotalFight(Pokemon pokemon1, Pokemon pokemon2,
		double* xp1, double* xp2);
/*
 * @param Pokemon pokemon
 * update the cp with the its new value - due to a candy
 * @return
 * POKEMON_NULL_ARGUMENT - gets a NULL as pokemon
 * POKEMON_SUCCESS - update the pokemons attributes
 */
PokemonResult pokemonGetCandy(Pokemon pokemon, int candy_value);
/*
 * @param Pokemon pokemon
 * update the hp with the its new value - due to a potion
 * @return
 * POKEMON_NULL_ARGUMENT - gets a NULL as pokemon
 * POKEMON_SUCCESS - update the pokemons attributes
 */
PokemonResult pokemonGetPotion(Pokemon pokemon, double potion_value);
/*
 * @param Pokemon pokemon
 * returns the Pokemon current HP as double
 */
double pokemonCurrHP(Pokemon pokemon);
/*
 * @param Pokemon pokemon
 * print the given pokemon's name using the mtmPrintPokemon code
 */
void pokemonPrint(Pokemon pokemon, FILE* output_f);
/*
 * @param Pokemon pokemon
 * this function returns the given pokemon's name
 */
char* pokemonGetName(Pokemon pokemon);
/*
 * @param
 * Pokemon pokemon1, Pokemon pokemon2
 * compare between to pokemons names
 * if they are equal it returns 0
 * if the first is bigger it returns a positive number
 * if the second is bigger it returns a negative number
 */
int pokemonCompareNames(Pokemon pokemon1, Pokemon pokemon2);
/*
 *  @param Pokemon pokemon
 *	checks whether or not a given pokemon can be hunted by
 *	 a trainer. if it is return True
 *	 else return False
 */
bool pokemonIsAbleToBeHunted(Pokemon pokemon);
/*
 * @param Pokemon pok, int numPokemons
 *	updates the given pokemon's ID
 */
void pokemonAddId(Pokemon pok, int numPokemons);
/*
 * @param Pokemon pokemon
 *	returns the ID of a given pokemon
 */
int getIDfromPokemon(Pokemon pokemon);
/*
 * @param Pokemon pokemon
 *	returns the level of a given pokemon
 */
int pokemonGetLevel(Pokemon pokemon);
/*
 *@param Pokemon pokemon
 *	returns the cp of a given pokemon
 */
int pokemonGetCP(Pokemon pokemon);
/*
 * @param Pokemon pokemon, int cp, char* new_name,char* type
 *	updates the pokemon situation after an evolution
 */
void pokemonUpdateAfterEvolution(Pokemon pokemon, int cp, char* new_name,
		char* type);
/*
 *@param Pokemon pokemon1, Pokemon pokemon2
 * return true if the two given pokemons are equal in all their
 * properties, else return false
 */
bool pokemonIsEqual(Pokemon pokemon1, Pokemon pokemon2);


#endif /* POKEMON_H_ */
