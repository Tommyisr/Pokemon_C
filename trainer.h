/*
 * trainer.h
 *
 *  Created on: Dec 23, 2016
 *      Author: Tommyisr
 */

#ifndef TRAINER_H_
#define TRAINER_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "set.h"
#include "pokemon.h"
#include "map.h"
#include "print_utils.h"
#include "store.h"

/*
 * the struct declaration for a Trainer
 */
typedef struct trainer_t *Trainer;

/*
 * all the possibilities for a result as for Trainer
 */
typedef enum TrainerResult_e {
	TRAINER_SUCCESS,
	TRAINER_NULL_ARGUMENT,
	TRAINER_OUT_OF_MEMORY,
	TRAINER_INVALID_ARGUMENT,
	TRAINER_POKEMON_ALREADY_EXISTS,
	TRAINER_ITEM_OUT_OF_STOCK,
	TRAINER_NO_MONEY
} TrainerResult;

/*
 * allocates a copy of current trainer
 */
Trainer copyTrainer(Trainer n);

/*
 * destroy the trainer, deallocates all the memory
 */
void destroyTrainer(Trainer n);

/*
 * creates a new trainer with his name, location where it's located and its budget
 */
Trainer trainerCreate(char* name, char* location, int money);

/*
 * adds a pokemon to trainer
 */
TrainerResult trainerAddPokemon(Trainer trainer, Pokemon pokemon);

/*
 * removing the pokemon from the trainer
 */
bool trainerRemovePokemon(Trainer trainer, Pokemon pokemon);

/*
 * changing the amount of money for trainer
 */
void trainerChangeMoney(Trainer trainer, int money);

/*
 * returns a location where trainer is located right now
 */
char* trainerGetLocation(Trainer trainer);

/*
 * change the current location to destination for trainer
 */
void trainerChangeLocation(Trainer trainer, char* loc_name);

/*
 * returns the trainer's money
 */
int trainerGetMoney(Trainer trainer);

/*
 * updating trainer's info after purchasing
 */
void trainerUpdatePurchase(Trainer trainer, int money_value, int value,
		char* item);

/*
 * deducts the amount of money from trainer
 */
void trainerDeductionOfMoney(Trainer trainer, int money);

/*
 * returns the pokemon from trainer by its ID
 */
Pokemon getPokemonByIDFromTrainer(Trainer trainer, int id);

/*
 * returns the XP from trainer
 */
double* trainerGetXPbyReference(Trainer trainer);

/*
 * returns the amount of potion items from trainer
 */
int trainerGetPotionSize(Trainer trainer);
/*
 * returns the amount of candy items from trainer
 */
int trainerGetCandySize(Trainer trainer);

/*
 * returns the store of trainer
 */
Store trainerGetItems(Trainer trainer);

/*
 * prints all the info about trainer
 */
void trainerPrint(Trainer trainer, FILE* output_f);

MapKeyElement copyMapKeyElTrainer(MapKeyElement n);

/*
 * changes the amount of pokemons that trainer has
 */
void trainerChangeNumPokemons(Trainer trainer);

#endif /* TRAINER_H_ */
