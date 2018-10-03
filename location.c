/*
 * location.c
 *
 *  Created on: 20 בדצמ× 2016
 *      Author: Dvir
 */

#include "location.h"

/*
 * struct of the location type
 */

struct Location_s {
	char* name;
	List pokemons_l;
	Set locations_names;
};
static SetElement locationsNameCopy(SetElement element);
static void locationsNameFree(SetElement element);
static int locationsNameCompare(SetElement element1, SetElement element2);
static ListElement pokemonCopy_l(ListElement pokemon);
static void pokemonFree_l(ListElement pokemon);

/*
 * @param SetElement element
 * copy a SetElement which is a char* element
 * and return it. if memory allocation failed it returns NULL
 */
static SetElement locationsNameCopy(SetElement element) {
	char* name_copy = malloc(sizeof(char) * strlen(element) + 1);
	if (!name_copy)
		return NULL;
	strcpy(name_copy, (element));
	return name_copy;
}
/*
 * @param SetElement element
 * free all the allocated memory for a set element which is type char*
 */
static void locationsNameFree(SetElement element) {
	free(element);
}
/*
 * @param SetElement element1, SetElement element2
 * compares between two set elements from a char* type using strcmp
 */
static int locationsNameCompare(SetElement element1, SetElement element2) {
	return strcmp((char*) (element1), (char*) (element2));
}
/*
 * @param ListElement pokemon
 * an external function to the List struct that copy a pokemon
 * returns NULL if the given pokemon is NULL
 */
static ListElement pokemonCopy_l(ListElement pokemon) {
	if (!pokemon)
		return NULL;
	Pokemon new_pokemon = pokemonCopy(pokemon);
	return new_pokemon ? new_pokemon : NULL;
}
/*
 * @param ListElement pokemon
 * an external function to the List struct that free all the memory
 * of a pokemon
 */
static void pokemonFree_l(ListElement pokemon) {
	pokemonDestroy(pokemon);
}
/*
 * @param char* name
 * creates a single location. return NULL if the given name
 * is NULL or if memory allocation has failed
 */
Location locationCreate(char* name) {
	if (!name)
		return NULL;
	Location new_location = malloc(sizeof(*new_location));
	if (!new_location)
		return NULL;
	new_location->name = malloc(sizeof(char) * strlen(name) + 1);
	if (!new_location->name) {
		free(new_location);
		return NULL;
	}
	strcpy(new_location->name, name); 	 //adding the name for a location
	new_location->pokemons_l = listCreate(pokemonCopy_l, pokemonFree_l);
	if (!new_location->pokemons_l) {
		free(new_location->name);
		free(new_location);
		return NULL;
	}
	new_location->locations_names = setCreate(locationsNameCopy,
			locationsNameFree, locationsNameCompare);
	if (!new_location->locations_names) {
		free(new_location->name);
		listDestroy(new_location->pokemons_l);
		free(new_location);
		return NULL;
	}
	return new_location;
}
/*
 * @param Location location, Pokemon pokemon
 * add pokemon to location's pokemon list
 * @return
 * LOCATION_NULL_ARGUMENT - if one of the argument is NULL or the
 * insert has failed due to a NULL list
 * LOCATION_OUT_OF_MEMORY - the allocation for a new pokemon has failed
 * LOCATION_SUCCESS - if the pokemon adding is a success
 */
LocationResult locationAddPokemon(Location location, Pokemon pokemon) {
	if (!location || !pokemon)
		return LOCATION_NULL_ARGUMENT;
	ListResult result = listInsertLast(location->pokemons_l, pokemon);
	if (result == LIST_NULL_ARGUMENT)
		return LOCATION_NULL_ARGUMENT;
	if (result == LIST_OUT_OF_MEMORY)
		return LOCATION_OUT_OF_MEMORY;
	return LOCATION_SUCCESS;
}
/*
 * @param Location location
 * copy a location to a new location
 * if it fails return NULL and free all the allocated memory.
 * Otherwise return the new copy of a location
 */
Location locationCopy(Location location) {
	if (!location)
		return NULL;
	Location new_location = malloc(sizeof(*new_location));
	if (!new_location)
		return NULL;
	new_location->name = malloc(sizeof(strlen(location->name) + 1));
	if (!new_location->name) {
		free(new_location);
		return NULL;
	}
	strcpy(new_location->name, location->name);
	new_location->pokemons_l = listCopy(location->pokemons_l);
	if (!new_location->pokemons_l) {
		free(new_location->name);
		free(new_location);
		return NULL;
	}
	new_location->locations_names = setCopy(location->locations_names);
	if (!new_location->locations_names) {
		free(new_location->name);
		listDestroy(new_location->pokemons_l);
		free(new_location);
		return NULL;
	}
	return new_location;
}
/*@param Location location
 * destroy a location struct and free all its memory
 */
void locationDestroy(Location location) {
	if (!location)
		return;
	free(location->name);
	listDestroy(location->pokemons_l);
	setDestroy(location->locations_names);
	free(location);
	location = NULL;
}
/*
 * @param Location location
 * returns the first pokemon in the location pokemon list
 */
Pokemon locationGetFirstPokemon(Location location) {
	if (!location)
		return NULL;
	Pokemon first = listGetFirst(location->pokemons_l);
	if (!first)
		return NULL;
	return first;
}
/*
 * @param Location location, Pokemon pokemon
 * remove pokemon from location
 */
LocationResult locationRemovePokemon(Location location, Pokemon pokemon) {
	if (!location || !pokemon)
		return LOCATION_NULL_ARGUMENT;
	Pokemon ptr = listGetFirst(location->pokemons_l);
	int num = listGetSize(location->pokemons_l);
	if (num <= 0)
		return LOCATION_LIST_EMPTY;
	while (num > 0) {
		if (pokemonCompareNames(pokemon, ptr) == 0) {
			ListResult result = listRemoveCurrent(location->pokemons_l);
			if (result == LIST_NULL_ARGUMENT)
				return LOCATION_NULL_ARGUMENT;
			else if (result == LIST_INVALID_CURRENT)
				return LOCATION_LIST_INVALID;
			return LOCATION_SUCCESS;
		}
		ptr = listGetNext(location->pokemons_l);
		num--;
	}
	return LOCATION_POKEMON_DOES_NOT_EXIST;
}
/*
 * @param Location location, char* name_location
 * add nearby location.
 * @return
 * LOCATION_NULL_ARGUMENT - if one of the argument is NULL
 * LOCATION_OUT_OF_MEMORY - if the memory allocation has failed
 * LOCATION_NAME_EXIST - the new location is already exists
 * LOCATION_SUCCESS - the new location has been added
 */
LocationResult locationAddNearLocation(Location location, char* name_location) {
	if (!location || !name_location)
		return LOCATION_NULL_ARGUMENT;
	SetResult result = setAdd(location->locations_names, name_location);
	if (result == SET_NULL_ARGUMENT)
		return LOCATION_NULL_ARGUMENT;
	if (result == SET_OUT_OF_MEMORY)
		return LOCATION_OUT_OF_MEMORY;
	if (result == SET_ITEM_ALREADY_EXISTS)
		return LOCATION_NAME_EXIST;
	return LOCATION_SUCCESS;
}
/*
 * @param Location location, char* name_location
 * remove nearby location.
 * @return
 * LOCATION_NULL_ARGUMENT - if one of the argument is NULL
 * LOCATION_ITEM_DOES_NOT_EXIST - if the requested location does not exists
 * LOCATION_SUCCESS - the new location has been removed
 */
LocationResult locationAddRemoveLocation(Location location, char* name_location) {
	if (!location || !name_location)
		return LOCATION_NULL_ARGUMENT;
	SetResult result = setRemove(location->locations_names, name_location);
	if (result == SET_NULL_ARGUMENT)
		return LOCATION_NULL_ARGUMENT;
	if (result == SET_ITEM_DOES_NOT_EXIST)
		return LOCATION_ITEM_DOES_NOT_EXIST;
	return LOCATION_SUCCESS;
}
/*
 * @param Location my_location, Location distanced_location
 * gets a location name and checks if that location is near me
 * returns true if it is, false otherwise
 */
bool locationIsNearMe(Location my_location, Location distanced_location) {
	if (!my_location || !distanced_location)
		return false;
	return setIsIn(my_location->locations_names, distanced_location->name);
}
/*
 * @param Location location, Pokemon pokemon
 * gets true if a pokemon is free in that location
 * false - if it gets NULL as an argument or the pokemon
 * is not in that location
 */
bool locationIsPokemonIn(Location location, Pokemon pokemon) {
	if (!location || !pokemon)
		return false;
	Pokemon ptr_pok = listGetFirst(location->pokemons_l);
	while (ptr_pok != NULL) {
		if (pokemonCompareNames(ptr_pok, pokemon) == 0)
			return true;
		ptr_pok = listGetNext(location->pokemons_l);
	}
	return false;

}

/*
 * @param Location location, FILE* output_f
 * print the given location to the given out-put channel
 * using the function mtmPrintLocation
 */
void locationPrint(Location location, FILE* output_f) {
	if (!location || !output_f)
		return;
	mtmPrintLocation(output_f, location->name,
			pokemonGetName(listGetFirst(location->pokemons_l)));

}
