#include "pokemonGo.h"

struct PokemonGo_s {
	Map pokedex;
	Map evolutions;
	Map world;
	Map trainers;
	Store store;
};

static void pokemonDestroyForMain(MapDataElement pokemon);
static void evolutionDestroyForMain(MapDataElement evolution);
static void locationDestroyForMain(MapDataElement location);
static void trainerDestroyForMain(MapDataElement trainer);
static MapDataElement pokemonCopyForMain(MapDataElement pokemon);
static MapDataElement evolutionCopyForMain(MapDataElement evolution);
static MapDataElement locationCopyForMain(MapDataElement location);
static MapDataElement trainerCopyForMain(MapDataElement trainer);
static void freeMapKeyString(MapKeyElement n);
static int compareKeyElement(MapKeyElement element1, MapKeyElement element2);
static MapKeyElement copyKeyString(MapKeyElement name);
static void destroyBasicADTelements(Map pokedex, Map evolutions, Map world,
		Map trainers, Store store);
static bool huntPokemon(Location loc, Pokemon poc, Trainer trainer);
static bool PokemonEvolution(Pokemon pokemon, PokemonGo PG);
static int pokedexGetCP(PokemonGo PG, char* name);
static char* pokedexGetNewType(PokemonGo PG, char* name);
/*
 * this function destroyes a pokemon as a Data element
 */
static void pokemonDestroyForMain(MapDataElement pokemon) {
	pokemonDestroy(pokemon);
}
/*
 * this function destroyes an evolution object as a Data element
 */
static void evolutionDestroyForMain(MapDataElement evolution) {
	freeEvo(evolution);
}
/*
 * this function destroyes a location object as a Data element
 */
static void locationDestroyForMain(MapDataElement location) {
	locationDestroy(location);
}
/*
 * this function destroyes a trainer object as a Data element
 */
static void trainerDestroyForMain(MapDataElement trainer) {
	destroyTrainer(trainer);
}
/*
 * this function copies a pokemon object as a Data element
 */
static MapDataElement pokemonCopyForMain(MapDataElement pokemon) {
	return pokemonCopy(pokemon);
}
/*
 * this function copies an evolution object as a Data element
 */
static MapDataElement evolutionCopyForMain(MapDataElement evolution) {
	return copyEvo(evolution);
}
/*
 * this function copies a location object as a Data element
 */
static MapDataElement locationCopyForMain(MapDataElement location) {
	return locationCopy(location);
}
/*
 * this function copies a trainer object as a Data element
 */
static MapDataElement trainerCopyForMain(MapDataElement trainer) {
	return copyTrainer(trainer);
}
/*
 * free a key in map as a char*(string)
 */
static void freeMapKeyString(MapKeyElement n) {
	free(n);
}
/*
 * compares 2 keyElements as from type char*
 */
static int compareKeyElement(MapKeyElement element1, MapKeyElement element2) {
	if (!element1 || !element2)
		return -1;
	if (strcmp((char*) element1, (char*) element2) == 0) {
		return 0;
	} else
		return strcmp((char*) element1, (char*) element2) > 0 ? 1 : -1;
}
/*
 * copy a key element from char* type
 */
static MapKeyElement copyKeyString(MapKeyElement name) {
	if (!name)
		return NULL;
	char* new_str = malloc(sizeof(char) * strlen(name) + 1);
	if (!new_str)
		return NULL;
	strcpy(new_str, (char*) name);
	return new_str;
}
/*
 * this function creates all the relevent ADT of the game and initielizes the
 * POKEMONGO struct
 */
PokemonGo pokemonGoCreate() {
	PokemonGo PG = malloc(sizeof(*PG));
	Map pokedex = mapCreate(copyKeyString, pokemonCopyForMain, freeMapKeyString,
			pokemonDestroyForMain, compareKeyElement);
	Map evolutions = mapCreate(copyKeyString, evolutionCopyForMain,
			freeMapKeyString, evolutionDestroyForMain, compareKeyElement);
	Map world = mapCreate(copyKeyString, locationCopyForMain, freeMapKeyString,
			locationDestroyForMain, compareKeyElement);
	Map trainers = mapCreate(copyKeyString, trainerCopyForMain,
			freeMapKeyString, trainerDestroyForMain, compareKeyElement);
	Store store = storeCreate();
	if (!pokedex || !evolutions || !world || !store || !trainers) {
		destroyBasicADTelements(pokedex, evolutions, world, trainers, store);
		free(PG);
		return NULL;
	}
	PG->pokedex = mapCopy(pokedex);
	PG->evolutions = mapCopy(evolutions);
	PG->world = mapCopy(world);
	PG->trainers = mapCopy(trainers);
	PG->store = storeCopy(store);
	if (!PG->pokedex || !PG->evolutions || !PG->world || !PG->store
			|| !PG->trainers) {
		destroyGameADTS(PG);
		destroyBasicADTelements(pokedex, evolutions, world, trainers, store);
		return NULL;
	}
	destroyBasicADTelements(pokedex, evolutions, world, trainers, store);
	return PG;
}
/*
 * this function destroies all the inner pokemon go ADT all the pokemonGo ADT's like the pokodex, map,
 * trainers and store
 */
static void destroyBasicADTelements(Map pokedex, Map evolutions, Map world,
		Map trainers, Store store) {
	mapDestroy(pokedex);
	mapDestroy(evolutions);
	mapDestroy(world);
	mapDestroy(trainers);
	destroyStore(store);
}
/*
 * this function destories the POKEMONGO type
 */
PokemonGoResult destroyGameADTS(PokemonGo pokemonGo) {
	mapDestroy(pokemonGo->pokedex);
	mapDestroy(pokemonGo->evolutions);
	mapDestroy(pokemonGo->world);
	mapDestroy(pokemonGo->trainers);
	destroyStore(pokemonGo->store);
	free(pokemonGo);
	return POKEMONGO_HAS_DESTROYED;
}
/*
 * this function gets a pointer to the map in the PG by its name
 */
Store pokemonGoGetPtrByStrStore(PokemonGo PG, char* str) {
	if (!PG || !str)
		return NULL;
	return PG->store;
}

Map pokemonGoGetPtrByStr(PokemonGo PG, char* str) {
	if (!PG || !str)
		return NULL;
	if (strcmp(str, "pokedex") == 0)
		return (PG->pokedex);
	else if (strcmp(str, "evolutions") == 0)
		return (PG->evolutions);
	else if (strcmp(str, "world") == 0)
		return (PG->world);
	else if (strcmp(str, "trainers") == 0)
		return (PG->trainers);
	else
		return NULL;
}

/*
 * this is No.1 function
 * it adds a trainer to the game
 */
bool trainerAdd(PokemonGo PG, char* name, int budget, char* start_point,
		FILE* output_f) {
	if (budget < 0) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_ARGUMENT);
		return false;
	}
	if (mapContains(PG->trainers, name)) {
		mtmPrintErrorMessage(stderr, MTM_TRAINER_NAME_ALREADY_EXISTS);
		return false;
	}
	if (!mapContains(PG->world, start_point)) {
		mtmPrintErrorMessage(stderr, MTM_LOCATION_DOES_NOT_EXIST);
		return false;
	}
	Trainer trainer = trainerCreate(name, start_point, budget);
	if (!trainer)
		return false;
	Location loc = mapGet(PG->world, start_point);
	Pokemon poc = locationGetFirstPokemon(loc);
	if (!huntPokemon(loc, poc, trainer)) {
		mtmPrintCatchResult(output_f, name, NULL, start_point);
		mapPut(PG->trainers, name, trainer);
		destroyTrainer(trainer);
		return false;
	}
	mtmPrintCatchResult(output_f, name, pokemonGetName(poc), start_point);
	locationRemovePokemon(loc, poc);
	mapPut(PG->trainers, name, trainer);
	destroyTrainer(trainer);
	return true;
}
/*
 * this is No.2 function
 * it sends the trainer to a different location in the world game
 */
bool trainerGo(PokemonGo PG, char* name, char* location_name, FILE* output_f) {
	if (!mapContains(PG->trainers, name)) {
		mtmPrintErrorMessage(stderr, MTM_TRAINER_DOES_NOT_EXIST);
		return false;
	}
	if (!mapContains(PG->world, location_name)) {
		mtmPrintErrorMessage(stderr, MTM_LOCATION_DOES_NOT_EXIST);
		return false;
	}
	Trainer trainer = mapGet(PG->trainers, name);
	char* trainer_loc_name = trainerGetLocation(trainer);
	if (strcmp(trainer_loc_name, location_name) == 0) {
		mtmPrintErrorMessage(stderr, MTM_TRAINER_ALREADY_IN_LOCATION);
		return false;
	}
	Location locTrainer = mapGet(PG->world, trainer_loc_name);
	Location locDestination = mapGet(PG->world, location_name);
	if (!locationIsNearMe(locTrainer, locDestination)) {
		mtmPrintErrorMessage(stderr, MTM_LOCATION_IS_NOT_REACHABLE);
		return false;
	}
	Pokemon poc = locationGetFirstPokemon(locDestination);
	if (!poc) {
		trainerChangeLocation(trainer, location_name);
		mtmPrintCatchResult(output_f, name, NULL, location_name);
		return false;
	}
	if (!huntPokemon(locDestination, poc, trainer))
		return false;
	trainerChangeLocation(trainer, location_name);
	mtmPrintCatchResult(output_f, name, pokemonGetName(poc), location_name);
	locationRemovePokemon(locDestination, poc);
	return true;
}
/*
 * this function hunts pokemon for a trainer at his location
 */
static bool huntPokemon(Location loc, Pokemon poc, Trainer trainer) {
	if (!pokemonIsAbleToBeHunted(poc)) {
		return false;
	}
	trainerAddPokemon(trainer, poc);
	int pokecoin = pokemonIsTypeSpecial(poc);
	trainerChangeMoney(trainer, pokecoin);
	return true;
}
/*
 * this is No.3 function
 * it adds an item to the game store
 */
PokemonGoResult storeAddItem(PokemonGo PG, char* item, int value, int quantity) {
	if ((strcmp(item, "potion") != 0 && strcmp(item, "candy") != 0)
			|| value <= 0 || quantity <= 0) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_ARGUMENT);
		return POKEMONGO_NULL_ARGUMENT;
	}
	storeAdd(PG->store, item, value, quantity);
	return POKEMONGO_SUCCESS;
}
/*
 * this is No.4 function
 * it makes a trainer purchase an item from the store
 * it can be potion or candy
 */
PokemonGoResult trainerPurchase(PokemonGo PG, char* name_trainer, char* item,
		int value) {
	if ((strcmp(item, "potion") != 0 && strcmp(item, "candy") != 0)
			|| value <= 0) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_ARGUMENT);
		return POKEMONGO_NULL_ARGUMENT;
	}
	Trainer trainer = mapGet(PG->trainers, name_trainer);
	if (!trainer) {
		mtmPrintErrorMessage(stderr, MTM_TRAINER_DOES_NOT_EXIST);
		return POKEMONGO_TRAINER_DOES_NOT_EXIST;
	}
	int quantity = storeGetQuantityByKey(PG->store, item, value);
	if (quantity == -1) {
		mtmPrintErrorMessage(stderr, MTM_ITEM_OUT_OF_STOCK);
		return POKEMONGO_ITEM_OUT_OF_STOCK;
	}
	int money_trainer = trainerGetMoney(trainer);
	int money_value = value;
	if (strcmp(item, "candy") == 0) {
		money_value = value * 2;
	}
	if (money_trainer < money_value) {
		mtmPrintErrorMessage(stderr, MTM_BUDGET_IS_NOT_SUFFICIENT);
		return POKEMONGO_TRAINER_OUT_OF_MONEY;
	}
	changeQuantityFromStoreBuy(PG->store, item, value);
	trainerUpdatePurchase(trainer, money_value, value, item);
	return POKEMONGO_SUCCESS;
}
/*
 * this is No.5 function
 * it makes a battel between 2 pokemon trainers and theirs pokemons
 */
PokemonGoResult battleFight(PokemonGo PG, char* trainer1_name,
		char* trainer2_name, int id1, int id2, FILE* file_o) {
	bool is_pokemon1_dead = false, is_pokemon2_dead = false;
	if (strcmp(trainer1_name, trainer2_name) == 0) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_ARGUMENT);
		return POKEMONGO_NULL_ARGUMENT;
	}
	Trainer trainer1 = mapGet(PG->trainers, trainer1_name);
	Trainer trainer2 = mapGet(PG->trainers, trainer2_name);
	if (!trainer1 || !trainer2) {
		mtmPrintErrorMessage(stderr, MTM_TRAINER_DOES_NOT_EXIST);
		return POKEMONGO_TRAINER_DOES_NOT_EXIST;
	}
	Pokemon poke1 = getPokemonByIDFromTrainer(trainer1, id1);
	Pokemon poke2 = getPokemonByIDFromTrainer(trainer2, id2);
	if (!poke1 || !poke2) {
		mtmPrintErrorMessage(stderr, MTM_POKEMON_DOES_NOT_EXIST);
		return POKEMONGO_POKEMON_DOES_NOT_EXIST;
	}
	Pokemon pok1_old = pokemonCopy(poke1);
	if (!pok1_old) {
		mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
		return POKEMONGO_OUT_OF_MEMORY;
	}
	Pokemon pok2_old = pokemonCopy(poke2);
	if (!pok2_old) {
		free(pok2_old);
		mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
		return POKEMONGO_OUT_OF_MEMORY;
	}
	double xp1_old = *(trainerGetXPbyReference(trainer1));
	double xp2_old = *(trainerGetXPbyReference(trainer2));
	double hold1 = pokemonCurrHP(poke1), hold2 = pokemonCurrHP(poke2);
	pokemonAfterATotalFight(poke1, poke2, trainerGetXPbyReference(trainer1),
			trainerGetXPbyReference(trainer2));
	double hp1 = pokemonCurrHP(poke1);
	if (hp1 <= 0) {
		trainerRemovePokemon(trainer1, poke1);
		is_pokemon1_dead = true;
	}
	double hp2 = pokemonCurrHP(poke2);
	if (hp2 <= 0) {
		trainerRemovePokemon(trainer2, poke2);
		is_pokemon2_dead = true;
	}
	while (PokemonEvolution(poke1, PG))
		;
	while (PokemonEvolution(poke2, PG))
		;
	mtmPrintBattle(file_o, trainer1_name, trainer2_name,
			pokemonGetName(pok1_old), pokemonGetName(pok2_old),
			pokemonGetCP(pok1_old), pokemonGetCP(pok2_old), hold1, hold2, hp1,
			hp2, pokemonGetLevel(pok1_old), pokemonGetLevel(pok2_old),
			pokemonGetLevel(poke1), pokemonGetLevel(poke2), xp1_old, xp2_old,
			*(trainerGetXPbyReference(trainer1)),
			*(trainerGetXPbyReference(trainer2)), is_pokemon1_dead,
			is_pokemon2_dead);
	pokemonDestroy(pok1_old);
	pokemonDestroy(pok2_old);
	return POKEMONGO_SUCCESS;
}
/*
 * this function makes a pokemon envolved by its level
 */
static bool PokemonEvolution(Pokemon pokemon, PokemonGo PG) {
	if (!pokemon || !PG->pokedex || !PG->evolutions)
		return false;
	char* pokemon_name = pokemonGetName(pokemon);
	int pokemon_level = pokemonGetLevel(pokemon);
	Evolutions evo = mapGet(PG->evolutions, pokemon_name);
	int lvl = evolutionsGetLevel(evo);
	if (lvl == -1)
		return false;
	if (pokemon_level >= lvl) {
		char* name_after = evolutionGetNewName(evo);
		if (!name_after)
			return false;
		int cp = pokedexGetCP(PG, name_after);
		if (cp == -1)
			return false;
		char* type_after = pokedexGetNewType(PG, name_after);
		pokemonUpdateAfterEvolution(pokemon, cp, name_after, type_after);
		return true;
	}
	return false;

}
/*
 * this function returns the pokemon cp according to the pokodex
 */
static int pokedexGetCP(PokemonGo PG, char* name) {
	if (!PG->pokedex || !name)
		return -1;
	Pokemon poke = mapGet(PG->pokedex, name);
	if (!poke)
		return -1;
	return pokemonGetCP(poke);
}
/*
 * this function returns the pokemon new type according to the pokodex
 */
static char* pokedexGetNewType(PokemonGo PG, char* name) {
	if (!PG->pokedex || !name)
		return NULL;
	Pokemon poke = mapGet(PG->pokedex, name);
	if (!poke)
		return NULL;
	return pokemonGetTypes(poke);
}
/*
 * this is No.6 function
 * it heals a pokemon by a trainer potion using the
 * lowest potion that can get the pokemon to 100 HP
 * oter wise uses the highest one
 */
PokemonGoResult pokemonHeal(PokemonGo PG, char* trainer_name, int id) {
	if (!PG->trainers || !trainer_name)
		return POKEMONGO_NULL_ARGUMENT;
	Trainer trainer = mapGet(PG->trainers, trainer_name);
	if (!trainer) {
		mtmPrintErrorMessage(stderr, MTM_TRAINER_DOES_NOT_EXIST);
		return POKEMONGO_TRAINER_DOES_NOT_EXIST;
	}
	Pokemon poke = getPokemonByIDFromTrainer(trainer, id);
	if (!poke) {
		mtmPrintErrorMessage(stderr, MTM_POKEMON_DOES_NOT_EXIST);
		return POKEMONGO_POKEMON_DOES_NOT_EXIST;
	}
	if (trainerGetPotionSize(trainer) < 1) {
		mtmPrintErrorMessage(stderr, MTM_NO_AVAILABLE_ITEM_FOUND);
		return POKEMONGO_NO_AVAILABLE_ITEM_FOUND;
	}
	if (pokemonCurrHP(poke) >= 100) {
		mtmPrintErrorMessage(stderr, MTM_HP_IS_AT_MAX);
		return POKEMONGO_HP_IS_AT_MAX;
	}
	double delta = MAX_HP - pokemonCurrHP(poke);
	Store store = trainerGetItems(trainer);
	if (!store)
		return POKEMONGO_OUT_OF_MEMORY;
	int key;
	if (storeFindMinToMaxHp(store, delta)) {
		pokemonGetPotion(poke, delta);
	} else {
		key = storeGetMaxOutOfItem(store, "potion");
		pokemonGetPotion(poke, key);
		changeQuantityFromStoreBuy(store, "potion", key);

	}
	return POKEMONGO_SUCCESS;
}
/*
 * this is No.7 function
 * it train a pokemon using candy - and updates the pokemon cp
 */
PokemonGoResult pokemonTrain(PokemonGo PG, char* trainer_name, int pokemon_id) {
	if (!PG->trainers || !trainer_name)
		return POKEMONGO_NULL_ARGUMENT;
	Trainer trainer = mapGet(PG->trainers, trainer_name);
	if (!trainer) {
		mtmPrintErrorMessage(stderr, MTM_TRAINER_DOES_NOT_EXIST);
		return POKEMONGO_OUT_OF_MEMORY;
	}
	Pokemon poke = getPokemonByIDFromTrainer(trainer, pokemon_id);
	if (!poke) {
		mtmPrintErrorMessage(stderr, MTM_POKEMON_DOES_NOT_EXIST);
		return POKEMONGO_OUT_OF_MEMORY;
	}
	if (trainerGetCandySize(trainer) < 1) {
		mtmPrintErrorMessage(stderr, MTM_NO_AVAILABLE_ITEM_FOUND);
		return POKEMONGO_NO_AVAILABLE_ITEM_FOUND;
	}
	Store store = trainerGetItems(trainer);
	if (!store)
		return POKEMONGO_ITEM_OUT_OF_STOCK;
	int key = storeGetMaxOutOfItem(store, "candy");
	pokemonGetCandy(poke, key);
	changeQuantityFromStoreBuy(store, "candy", key);
	return POKEMONGO_SUCCESS;
}
/*
 * this is No.8 function
 * it prints all the trainer details
 */
PokemonGoResult reportTrainer(PokemonGo PG, char* trainer_name, FILE* output_f) {
	if (!PG->trainers || !trainer_name)
		return POKEMONGO_NULL_ARGUMENT;
	Trainer trainer = mapGet(PG->trainers, trainer_name);
	if (!trainer) {
		mtmPrintErrorMessage(stderr, MTM_TRAINER_DOES_NOT_EXIST);
		return POKEMONGO_TRAINER_DOES_NOT_EXIST;
	}
	trainerPrint(trainer, output_f);
	return POKEMONGO_SUCCESS;
}
/*
 * this is No.9 function
 * it prints all the world details
 */
PokemonGoResult reportLocations(PokemonGo PG, FILE* output_f) {
	if (!PG->world || !output_f)
		return POKEMONGO_NULL_ARGUMENT;
	mtmPrintLocationsHeader(output_f);
	char* location_name = mapGetFirst(PG->world);
	if (!location_name)
		return POKEMONGO_LOCATION_NO_TO_BE_FOUND;
	Location location = mapGet(PG->world, location_name);
	if (!location)
		return POKEMONGO_LOCATION_NO_TO_BE_FOUND;
	while (location_name != NULL || location != NULL) {
		locationPrint(location, output_f);
		location_name = mapGetNext(PG->world);
		location = mapGet(PG->world, location_name);
	}
	return POKEMONGO_SUCCESS;
}
/*
 * this is No.10 function
 * it prints all the store details
 */
PokemonGoResult reportStock(PokemonGo PG, FILE* output_f) {
	if (!PG->store || !output_f)
		return POKEMONGO_NULL_ARGUMENT;
	mtmPrintStockHeader(output_f);
	storePrint(PG->store, output_f);
	return POKEMONGO_SUCCESS;
}

