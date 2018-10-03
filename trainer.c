#include "trainer.h"
/*
 * the struct declaration for a Trainer
 */
struct trainer_t {

	char* name;
	List pokemons;
	Store items;
	double xp;
	char* locations;
	int money;
	int numPokemons;
};

static ListElement pokemonCopyForTrainerList(ListElement old_pokemon) {
	if (!old_pokemon)
		return NULL;
	Pokemon new_pokemon = pokemonCopy(old_pokemon);
	return new_pokemon;
}
static void pokemonDestroyForTrainerList(ListElement pokemon) {
	pokemonDestroy(pokemon);
}

/*
 * allocates a copy of current trainer
 */
Trainer copyTrainer(Trainer n) {
	if (!n) {
		return NULL;
	}
	Trainer copy = malloc(sizeof(*copy));
	if (!copy) {
		return NULL;
	}
	copy->locations = malloc(sizeof(char) * (strlen(n->locations) + 1));
	if (!copy->locations)
		free(copy);
	copy->name = malloc(sizeof(char) * (strlen(n->name) + 1));
	if (!(copy->name)) {
		free(copy->locations);
		free(copy);
	}
	strcpy(copy->name, n->name);
	strcpy(copy->locations, n->locations);
	copy->money = n->money;
	copy->xp = n->xp;
	copy->items = storeCopy(n->items);
	copy->pokemons = listCopy(n->pokemons);
	copy->numPokemons = (n->numPokemons);
	return copy;
}
/*
 * destroy the trainer, deallocates all the memory
 */
void destroyTrainer(Trainer n) {
	if (!n)
		return;
	free(n->locations);
	free(n->name);
	listDestroy(n->pokemons);
	destroyStore(n->items);
	free(n);
}
/*
 * creates a new trainer with his name, location where it's located and its budget
 */
Trainer trainerCreate(char* name, char* location, int money) {
	Trainer trainer = malloc(sizeof(*trainer));
	if (!trainer) {
		return NULL;
	}
	trainer->locations = malloc(sizeof(char) * strlen(location) + 1);
	trainer->name = malloc(sizeof(char) * strlen(name) + 1);
	if (!(trainer->locations) || !(trainer->name)) {
		free(trainer->name);
		free(trainer->locations);
		free(trainer);
		return NULL;
	}
	strcpy(trainer->name, name);
	strcpy(trainer->locations, location);
	trainer->items = storeCreate();
	if (!(trainer->items)) {
		free(trainer->name);
		free(trainer->locations);
		free(trainer);
		return NULL;
	}
	(trainer->money) = money;
	trainer->pokemons = listCreate(pokemonCopyForTrainerList,
			pokemonDestroyForTrainerList);
	if (!(trainer->pokemons)) {
		free(trainer->name);
		free(trainer->locations);
		destroyStore(trainer->items);
		free(trainer);
	}
	(trainer->numPokemons) = 0;
	(trainer->xp) = 1;
	return trainer;
}
/*
 * adds a pokemon to trainer
 */
TrainerResult trainerAddPokemon(Trainer trainer, Pokemon pokemon) {
	if (!trainer || !pokemon) {
		return TRAINER_NULL_ARGUMENT;
	}
	ListResult result = listInsertLast(trainer->pokemons, pokemon);
	if (result == LIST_NULL_ARGUMENT) {
		return TRAINER_NULL_ARGUMENT;
	}
	if (result == LIST_OUT_OF_MEMORY)
		return TRAINER_OUT_OF_MEMORY;
	Pokemon pok = listGetFirst(trainer->pokemons);
	int i = listGetSize(trainer->pokemons);
	while (i > 1) {
		pok = listGetNext(trainer->pokemons);
		i--;
	}
	trainerChangeNumPokemons(trainer);
	pokemonAddId(pok, (trainer->numPokemons));
	return TRAINER_SUCCESS;
}

/*
 * changes the amount of pokemons that trainer has
 */
void trainerChangeNumPokemons(Trainer trainer) {
	if (!trainer)
		return;
	(trainer->numPokemons) = (trainer->numPokemons) + 1;
}
/*
 * removing the pokemon from the trainer
 */
bool trainerRemovePokemon(Trainer trainer, Pokemon pokemon) {
	if (!trainer || !pokemon)
		return false;
	Pokemon poke_ptr = listGetFirst(trainer->pokemons);
	while (poke_ptr != NULL) {
		if (pokemonIsEqual(poke_ptr, pokemon)) {
			listRemoveCurrent(trainer->pokemons);
			return true;
		}
		poke_ptr = listGetNext(trainer->pokemons);
	}
	return false;
}

/*
 * changing the amount of money for trainer
 */
void trainerChangeMoney(Trainer trainer, int money) {
	if (!trainer) {
		return;
	}
	(trainer->money) += money;
}
/*
 * returns a location where trainer is located right now
 */
char* trainerGetLocation(Trainer trainer) {
	if (!trainer)
		return NULL;
	return trainer->locations;
}

/*
 * change the current location to destination for trainer
 */
void trainerChangeLocation(Trainer trainer, char* loc_name) {
	if (!trainer || !loc_name)
		return;

	char* new_location_name = malloc(sizeof(char) * strlen(loc_name) + 1);
	if (!new_location_name)
		return;
	free(trainer->locations);
	strcpy(new_location_name, loc_name);
	trainer->locations = new_location_name;
}

/*
 * returns the trainer's money
 */
int trainerGetMoney(Trainer trainer) {
	return (trainer->money);
}
/*
 * updating trainer's info after purchasing
 */
void trainerUpdatePurchase(Trainer trainer, int money_value, int value,
		char* item) {
	if (!trainer || !item)
		return;
	changeQuantityFromStorePurchase(trainer->items, item, value);
	trainerDeductionOfMoney(trainer, money_value);
}
/*
 * deducts the amount of money from trainer
 */
void trainerDeductionOfMoney(Trainer trainer, int money) {
	if (!trainer)
		return;
	(trainer->money) -= money;
}

/*
 * returns the pokemon from trainer by its ID
 */
Pokemon getPokemonByIDFromTrainer(Trainer trainer, int id) {
	List pokemons = trainer->pokemons;
	Pokemon pokemon;
	for (pokemon = listGetFirst(pokemons); pokemon != NULL; pokemon =
			listGetNext(pokemons)) {
		if (getIDfromPokemon(pokemon) == id) {
			return pokemon;
		}
	}
	return NULL;
}
/*
 * returns the XP from trainer
 */
double* trainerGetXPbyReference(Trainer trainer) {
	if (!trainer)
		return NULL;
	return &(trainer->xp);
}

/*
 * returns the amount of potion items from trainer
 */
int trainerGetPotionSize(Trainer trainer) {
	if (!trainer)
		return -1;
	return storeGetSizePotion(trainer->items);
}

/*
 * returns the amount of candy items from trainer
 */
int trainerGetCandySize(Trainer trainer) {
	if (!trainer)
		return -1;
	return storeGetSizeCandy(trainer->items);
}

/*
 * returns the store of trainer
 */
Store trainerGetItems(Trainer trainer) {
	if (!trainer)
		return NULL;
	return trainer->items;
}
/*
 * prints all the info about trainer
 */
void trainerPrint(Trainer trainer, FILE* output_f) {
	if (!trainer || !output_f)
		return;
	mtmPrintTrainerHeader(output_f, trainer->name, trainer->locations,
			(trainer->money), (trainer->xp));
	mtmPrintItemsHeaderForTrainer(output_f);
	storePrint(trainer->items, output_f);
	mtmPrintPokemonsHeaderForTrainer(output_f);
	int i = listGetSize(trainer->pokemons);
	Pokemon pokemon;
	if (i >= 1)
		pokemon = listGetFirst(trainer->pokemons);
	while (i >= 1) {

		pokemonPrint(pokemon, output_f);
		i--;
		pokemon = listGetNext(trainer->pokemons);
	}
}
