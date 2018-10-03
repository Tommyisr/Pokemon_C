/*
 * pokemon.h
 *
 *  Created on: 20 áãöî× 2016
 *      Author: Dvir
 */

#ifndef POKEMONGO_H_
#define POKEMONGO_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "map.h"
#include "print_utils.h"
#include "pokemon.h"
#include "store.h"
#include "trainer.h"
#include "location.h"
#include "evolutions.h"
typedef struct PokemonGo_s *PokemonGo;

typedef enum pokemonGoResult_e {
	POKEMONGO_SUCCESS,
	POKEMONGO_OUT_OF_MEMORY,
	POKEMONGO_NULL_ARGUMENT,
	POKEMONGO_FAIL,
	POKEMONGO_HAS_DESTROYED,
	POKEMONGO_TRAINER_DOES_NOT_EXIST,
	POKEMONGO_POKEMON_DOES_NOT_EXIST,
	POKEMONGO_ITEM_OUT_OF_STOCK,
	POKEMONGO_TRAINER_OUT_OF_MONEY,
	POKEMONGO_NO_AVAILABLE_ITEM_FOUND,
	POKEMONGO_HP_IS_AT_MAX,
	POKEMONGO_LOCATION_NO_TO_BE_FOUND
} PokemonGoResult;

Map pokemonGoGetPtrByStr(PokemonGo PG,char* str);
Store pokemonGoGetPtrByStrStore(PokemonGo PG,char* str);


/*
*this function creates all the relevent ADT of the game and initielizes the
*POKEMONGO struct
*/
PokemonGo pokemonGoCreate();
/*
 * this function destories the POKEMONGO type
 */
PokemonGoResult destroyGameADTS(PokemonGo pokemonGo);

/*
 * this is No.1 function
 * it adds a trainer to the game
 */
bool trainerAdd(PokemonGo PG, char* name, int budget,
		char* start_point, FILE* output_f);
/*
 * this is No.2 function
 * it sends the trainer to a different location in the world game
 */
bool trainerGo(PokemonGo PG, char* name, char* location_name,
		FILE* output_f);

/*
 * this is No.3 function
 * it adds an item to the game store
 */
PokemonGoResult storeAddItem(PokemonGo PG, char* item, int value, int quantity) ;
/*
 * this is No.4 function
 * it makes a trainer purchase an item from the store
 * it can be potion or candy
 */
PokemonGoResult trainerPurchase(PokemonGo PG, char* name_trainer, char* item,
		int value);
/*b
 * this is No.5 function
 * it makes a battel between 2 pokemon trainers and theirs pokemons
 */
PokemonGoResult battleFight(PokemonGo PG, char* trainer1_name, char* trainer2_name,
		int id1, int id2, FILE* file_o);
/*
 * this is No.6 function
 * it heals a pokemon by a trainer potion using the
 * lowest potion that can get the pokemon to 100 HP
 * oter wise uses the highest one
 */
PokemonGoResult pokemonHeal(PokemonGo PG, char* trainer_name, int id);
/*
 * this is No.7 function
 * it train a pokemon using candy - and updates the pokemon cp
 */
PokemonGoResult pokemonTrain(PokemonGo PG, char* trainer_name, int pokemon_id);
/*
 * this is No.8 function
 * it prints all the trainer details
 */
PokemonGoResult reportTrainer(PokemonGo PG, char* trainer_name, FILE* output_f);
/*
 * this is No.9 function
 * it prints all the world details
 */
PokemonGoResult reportLocations(PokemonGo PG, FILE* output_f);
/*
 * this is No.10 function
 * it prints all the store details
 */
PokemonGoResult reportStock(PokemonGo PG, FILE* output_f);

#endif /* POKEMONGO_H_ */
