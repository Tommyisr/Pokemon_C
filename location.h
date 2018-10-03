/*
 * location.h
 *
 *  Created on: 20 בדצמ× 2016
 *      Author: Dvir
 */
#ifndef LOCATION_H_
#define LOCATION_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "pokemon.h"
#include "set.h"
#include "list.h"
#include "print_utils.h"

/*
 * defining the location of on a map
 */
typedef struct Location_s *Location;

typedef enum LocationResult_e {
	LOCATION_SUCCESS,
	LOCATION_LIST_EMPTY,
	LOCATION_LIST_INVALID,
	LOCATION_POKEMON_DOES_NOT_EXIST,
	LOCATION_NAME_EXIST,
	LOCATION_TYPE_ALREADY_EXISTS,
	LOCATION_OUT_OF_MEMORY,
	LOCATION_NULL_ARGUMENT,
	LOCATION_ITEM_DOES_NOT_EXIST,
	LOCATION_IS_UP_TO_DATE,
	LOCATION_FAIL
} LocationResult;

/*
 * @param char* name
 * creates a single location. return NULL if the given name
 * is NULL or if memory allocation has failed
 */
Location locationCreate(char* name);
/*
 * @param Location location, Pokemon pokemon
 * add pokemon to location's pokemon list
 * @return
 * LOCATION_NULL_ARGUMENT - if one of the argument is NULL or the
 * insert has failed due to a NULL list
 * LOCATION_OUT_OF_MEMORY - the allocation for a new pokemon has failed
 * LOCATION_SUCCESS - if the pokemon adding is a success
 */
LocationResult locationAddPokemon(Location location, Pokemon pokemon);
/*
 * @param Location location
 * copy a location to a new location
 * if it fails return NULL and free all the allocated memory.
 * Otherwise return the new copy of a location
 */
Location locationCopy(Location location);
/*@param Location location
 * destroy a location struct and free all its memory
 */
void locationDestroy(Location location);
/*
 * @param Location location
 * returns the first pokemon in the location pokemon list
 */
Pokemon locationGetFirstPokemon(Location location);
/*
 * @param Location location, Pokemon pokemon
 * remove pokemon from location
 */
LocationResult locationRemovePokemon(Location location, Pokemon pokemon);
/*
 * @param Location location, char* name_location
 * add nearby location.
 * @return
 * LOCATION_NULL_ARGUMENT - if one of the argument is NULL
 * LOCATION_OUT_OF_MEMORY - if the memory allocation has failed
 * LOCATION_NAME_EXIST - the new location is already exists
 * LOCATION_SUCCESS - the new location has been added
 */
LocationResult locationAddNearLocation(Location location, char* name_location);
/*
 * @param Location location, char* name_location
 * remove nearby location.
 * @return
 * LOCATION_NULL_ARGUMENT - if one of the argument is NULL
 * LOCATION_ITEM_DOES_NOT_EXIST - if the requested location does not exists
 * LOCATION_SUCCESS - the new location has been removed
 */
LocationResult locationAddRemoveLocation(Location location, char* name_location);
/*
 * @param Location my_location, Location distanced_location
 * gets a location name and checks if that location is near me
 * returns true if it is, false otherwise
 */
bool locationIsNearMe(Location my_location, Location distanced_location);
/*
 * @param Location location, Pokemon pokemon
 * gets true if a pokemon is free in that location
 * false - if it gets NULL as an argument or the pokemon
 * is not in that location
 */
bool locationIsPokemonIn(Location location, Pokemon pokemon);
/*
 * @param Location location, FILE* output_f
 * print the given location to the given out-put channel
 * using the function mtmPrintLocation
 */
void locationPrint(Location location, FILE* output_f);

#endif /* LOCATION_H_ */
