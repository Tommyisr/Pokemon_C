/*
 * pokemon.c
 *
 *  Created on: 20 בדצמ× 2016
 *      Author: Dvir
 */

#include "pokemon.h"
#define MAX_SPECIAL_TYPES_3 3
#define MAX_SPECIAL_TYPES_2 5
#define MAX_SPECIAL_TYPES_1 6

struct pokemon_s {
	char* type_p;
	int cp;
	double hp;
	int level;
	int id;
	int howCandy;
	char* name;
};

static int pokemonIsTypeSpecialCover(char pokemon_type_string[], int len);
static int pokemonIsTypeSpecial30(char demo[]);
static int pokemonIsTypeSpecial20(char demo[]);
static int pokemonIsTypeSpecial10(char demo[]);
/*
 * @param
 * int cp, char* name, char* type
 * creates a new pokemon and return it
 */
Pokemon pokemonCreate(int cp, char* name, char* type) {
	if (!name)
		return NULL;
	Pokemon new_pokemon = malloc(sizeof(*new_pokemon));
	if (!new_pokemon)
		return NULL;
	new_pokemon->cp = cp;
	new_pokemon->hp = 100;
	new_pokemon->level = 1;
	new_pokemon->id = 0;
	new_pokemon->howCandy = 0;
	new_pokemon->name = malloc(sizeof(char) * strlen(name) + 1);
	if (!new_pokemon->name) {
		free(new_pokemon);
		return NULL;
	}
	strcpy(new_pokemon->name, name);
	new_pokemon->type_p = malloc(sizeof(char) * strlen(type) + 1);
	if (!new_pokemon->type_p) {
		free(new_pokemon->name);
		free(new_pokemon);
		return NULL;
	}
	strcpy(new_pokemon->type_p, type);
	return new_pokemon;
}

/*
 * @param Pokemon pokemon
 * a mask function for the pokemonIsTypeSpecialCover
 * which check and transfer the pokemon
 * if it gets NULL returns -1
 * else returns 10/20/30
 */
int pokemonIsTypeSpecial(Pokemon pokemon) {
	if (!pokemon)
		return -1;
	return pokemonIsTypeSpecialCover(pokemon->type_p, strlen(pokemon->type_p));
}
/*
 * @param Pokemon pokemon
 * this function gets a pokemon and return if this pokemon should
 * get 30/20/10 (accordind to its type) as a cp when
 * it is being caught by a trainer
 */
static int pokemonIsTypeSpecial30(char demo[]) {
	char* cp_type = strtok(demo, " ");
	while (cp_type != NULL) {
		if ((strcmp(cp_type, "FLYING") == 0) || (strcmp(cp_type, "FIRE") == 0)
				|| (strcmp(cp_type, "POISON") == 0))
			return 30;
		cp_type = strtok(NULL, " \n");
	}
	return -1;
}
static int pokemonIsTypeSpecial20(char demo[]) {
	char* cp_type = strtok(demo, " ");
	while (cp_type != NULL) {
		if ((strcmp(cp_type, "ELECTRIC") == 0)
				|| (strcmp(cp_type, "FAIRY") == 0)
				|| (strcmp(cp_type, "ICE") == 0)
				|| (strcmp(cp_type, "ROCK") == 0)
				|| (strcmp(cp_type, "WATER") == 0))

			return 20;
		cp_type = strtok(NULL, " \n");
	}
	return -1;
}
static int pokemonIsTypeSpecial10(char demo[]) {
	char* cp_type = strtok(demo, " ");
	while (cp_type != NULL) {
		if ((strcmp(cp_type, "BUG") == 0) || (strcmp(cp_type, "GRASS") == 0)
				|| (strcmp(cp_type, "NORMAL") == 0)
				|| (strcmp(cp_type, "GROUND") == 0)
				|| (strcmp(cp_type, "GHOST") == 0)
				|| (strcmp(cp_type, "PSYCHIC") == 0))

			return 10;
		cp_type = strtok(NULL, " \n");
	}
	return -1;
}
static int pokemonIsTypeSpecialCover(char pokemon_type_string[], int len) {
	char copy_pokemon_type_string[len];
	int i = -1;
	strcpy(copy_pokemon_type_string, pokemon_type_string);
	i = pokemonIsTypeSpecial30(copy_pokemon_type_string);
	if (i != -1)
		return i;
	strcpy(copy_pokemon_type_string, pokemon_type_string);
	i = pokemonIsTypeSpecial20(copy_pokemon_type_string);
	if (i != -1)
		return i;
	strcpy(copy_pokemon_type_string, pokemon_type_string);
	i = pokemonIsTypeSpecial10(copy_pokemon_type_string);
	if (i != -1)
		return i;
	return -1;
}
/*
 * @param Pokemon pokemon
 * allocate and create a copy of a given Pokemon.
 * returns NULL if the process has failed
 */
Pokemon pokemonCopy(Pokemon old_pokemon) {
	if (!old_pokemon)
		return NULL;
	Pokemon new_pokemon = pokemonCreate(old_pokemon->cp, old_pokemon->name,
			old_pokemon->type_p);
	if (!new_pokemon)
		return NULL;
	strcpy(new_pokemon->type_p, old_pokemon->type_p);
	new_pokemon->howCandy = old_pokemon->howCandy;
	new_pokemon->level = old_pokemon->level;
	new_pokemon->id = old_pokemon->id;
	return new_pokemon;
}
/*
 * @param Pokemon pokemon
 * free all the allocated space of a pokemon
 */
void pokemonDestroy(Pokemon pokemon) {
	if (!pokemon)
		return;
	free(pokemon->name);
	free(pokemon->type_p);
	free(pokemon);
}
/*
 * @param Pokemon pokemon
 * returns the Types of a pokemon
 * TODO NOT CHECKED
 */
char* pokemonGetTypes(Pokemon pokemon) {
	if (!pokemon)
		return NULL;
	return pokemon->type_p;
}
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
		double* xp1, double* xp2) {
	if (!pokemon1 || !pokemon2 || *xp1 < 0 || *xp2 < 0)
		return POKEMON_NULL_ARGUMENT;
	double d1, d2;
	//Determine the delta parameter
	d1 = (*xp2) * (pokemon2->cp) * (1 + ((pokemon2->hp) / 200));
	d2 = (*xp1) * (pokemon1->cp) * (1 + ((pokemon1->hp) / 200));
	//calculate the new level
	int level1 = pokemon1->level, level2 = pokemon2->level;
	int fourth_sum = (((level1 + level2) / 4) + 1);
	pokemon1->level = level1 + fourth_sum;
	pokemon2->level = level2 + fourth_sum;
	//calculate the new hp

	pokemon1->hp = (pokemon1->hp) - d1;
	if (pokemon1->hp < 0)
		pokemon1->hp = 0;
	pokemon2->hp = (pokemon2->hp) - d2;
	if (pokemon2->hp < 0)
		pokemon2->hp = 0;
	//calculate the new xp
	*xp1 = (*xp1) + (d2 / 10);
	*xp2 = (*xp2) + (d1 / 10);
	return POKEMON_SUCCESS;
}
/*
 * @param Pokemon pokemon
 * update the cp with the its new value - due to a candy
 * @return
 * POKEMON_NULL_ARGUMENT - gets a NULL as pokemon
 * POKEMON_SUCCESS - update the pokemons attributes
 */
PokemonResult pokemonGetCandy(Pokemon pokemon, int candy_value) {
	if (!pokemon || candy_value < 0)
		return POKEMON_NULL_ARGUMENT;
	pokemon->cp += candy_value;
	pokemon->howCandy += candy_value;
	return POKEMON_SUCCESS;
}
/*
 * @param Pokemon pokemon
 * update the hp with the its new value - due to a potion
 * @return
 * POKEMON_NULL_ARGUMENT - gets a NULL as pokemon
 * POKEMON_SUCCESS - update the pokemons attributes
 */
PokemonResult pokemonGetPotion(Pokemon pokemon, double potion_value) {
	if (!pokemon || potion_value < 0)
		return POKEMON_NULL_ARGUMENT;
	pokemon->hp += potion_value;
	if (pokemon->hp > MAX_HP)
		pokemon->hp = MAX_HP;
	return POKEMON_SUCCESS;
}
/*
 * @param Pokemon pokemon
 * returns the Pokemon current HP as double
 */
double pokemonCurrHP(Pokemon pokemon) {
	if (!pokemon)
		return -1;
	return pokemon->hp;
}
/*
 * @param Pokemon pokemon
 * print the given pokemon's name using the mtmPrintPokemon code
 */
void pokemonPrint(Pokemon pokemon, FILE* output_f) {
	if (!pokemon)
		return;
	mtmPrintPokemon(output_f, pokemon->id, pokemon->name, pokemon->hp,
			pokemon->cp, pokemon->level);

}
/*
 * @param Pokemon pokemon
 * this function returns the given pokemon's name
 */
char* pokemonGetName(Pokemon pokemon) {
	if (!pokemon)
		return NULL;
	return pokemon->name;
}
/*
 * @param
 * Pokemon pokemon1, Pokemon pokemon2
 * compare between to pokemons names
 * if they are equal it returns 0
 * if the first is bigger it returns a positive number
 * if the second is bigger it returns a negative number
 */
int pokemonCompareNames(Pokemon pokemon1, Pokemon pokemon2) {
	return strcmp(pokemon1->name, pokemon2->name);
}
/*
 *  @param Pokemon pokemon
 *	checks whether or not a given pokemon can be hunted by
 *	 a trainer. if it is return True
 *	 else return False
 */
bool pokemonIsAbleToBeHunted(Pokemon pokemon) {
	if (!pokemon) {
		return false;
	}
	if (pokemon->hp == 100 && pokemon->level == 1) {
		return true;
	}
	return false;
}
/*
 * @param Pokemon pok, int numPokemons
 *	updates the given pokemon's ID
 */
void pokemonAddId(Pokemon pok, int numPokemons) {
	if (!pok)
		return;
	pok->id = numPokemons;
}
/*
 * @param Pokemon pokemon
 *	returns the ID of a given pokemon
 */
int getIDfromPokemon(Pokemon pokemon) {
	if (!pokemon)
		return -1;
	return pokemon->id;
}
/*
 * @param Pokemon pokemon
 *	returns the level of a given pokemon
 */
int pokemonGetLevel(Pokemon pokemon) {
	if (!pokemon)
		return -1;
	return pokemon->level;
}
/*
 *@param Pokemon pokemon
 *	returns the cp of a given pokemon
 */
int pokemonGetCP(Pokemon pokemon) {
	if (!pokemon)
		return -1;
	return pokemon->cp;
}
/*
 * @param Pokemon pokemon, int cp, char* new_name,char* type
 *	updates the pokemon situation after an evolution
 */
void pokemonUpdateAfterEvolution(Pokemon pokemon, int cp, char* new_name,
		char* type) {
	if (!pokemon || !new_name || !type)
		return;
	free(pokemon->name);
	free(pokemon->type_p);
	pokemon->name = malloc(sizeof(char) * (strlen(new_name)) + 1);
	if (!pokemon->name)
		return;
	pokemon->type_p = malloc(sizeof(char) * (strlen(type)) + 1);
	if (!pokemon->type_p) {
		free(pokemon->name);
		return;
	}
	strcpy(pokemon->name, new_name);
	strcpy(pokemon->type_p, type);
	pokemon->cp = cp + (pokemon->howCandy);
}
/*
 *@param Pokemon pokemon1, Pokemon pokemon2
 * return true if the two given pokemons are equal in all their
 * properties, else return false
 */
bool pokemonIsEqual(Pokemon pokemon1, Pokemon pokemon2) {
	if (!pokemon1 || !pokemon2)
		return false;
	if (strcmp(pokemon1->name, pokemon2->name) != 0)
		return false;
	if (strcmp(pokemon1->type_p, pokemon2->type_p) != 0)
		return false;
	if (pokemon1->cp != pokemon2->cp)
		return false;
	if (pokemon1->hp != pokemon2->hp)
		return false;
	if (pokemon1->level != pokemon2->level)
		return false;
	if (pokemon1->id != pokemon2->id)
		return false;
	if (pokemon1->howCandy != pokemon2->howCandy)
		return false;
	return true;
}
