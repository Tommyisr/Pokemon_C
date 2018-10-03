#include "store.h"
struct store_t {
	Map Potion;
	Map Candy;

};

/* creates a copy of target struct.
 * @param
 * a - target struct(store)
 * @return
 * NULL - if the arguments are null or allocation if failed
 * A struct(store) containing the same elements as target struct otherwise.
 */
Store storeCopy(Store a) {
	if (!a) {
		return NULL;
	}
	Store copy = malloc(sizeof(*copy));
	if (!copy) {
		return NULL;
	}
	copy->Candy = mapCopy(a->Candy);
	copy->Potion = mapCopy(a->Potion);
	return copy;

}
/** Type of function for copying a data element of the map */
MapDataElement copyMapData(MapDataElement n) {
	if (!n) {
		return NULL;
	}
	int* copy = malloc(sizeof(*copy));
	if (!copy) {
		return NULL;
	}
	*copy = *(int*) n;
	return copy;

}
/** Type of function for copying a key element of the map */
MapKeyElement copyMapKey(MapKeyElement n) {
	if (!n) {
		return NULL;
	}
	int* copy = malloc(sizeof(*copy));
	if (!copy) {
		return NULL;
	}
	*copy = *(int*) n;
	return copy;

}
/** Type of function for deallocating a data element of the map */
void freeMapData(MapDataElement n) {
	if (!n)
		return;
	free(n);
}
/** Type of function for deallocating a key element of the map */
void freeMapKey(MapKeyElement n) {
	if (!n)
		return;
	free(n);
}
/** Type of function for deallocating a key element of the map */
int compareMapKeyEl(MapKeyElement a, MapKeyElement b) {
	int* int_1 = (int*) a;
	int* int_2 = (int*) b;
	if (*int_1 > *int_2) {
		return 1;
	} else if (*int_2 > *int_1) {
		return -1;
	} else {
		return 0;
	}
}
/*	allocates a new Store struct
 * @return
 * NULL - if allocations if failed
 */
Store storeCreate() {
	Store store = malloc(sizeof(*store));
	if (!store) {
		return NULL;
	}
	store->Candy = mapCreate(copyMapData, copyMapData, freeMapData, freeMapData,
			compareMapKeyEl);
	store->Potion = mapCreate(copyMapData, copyMapData, freeMapData,
			freeMapData, compareMapKeyEl);
	return store;
}
/*	Deallocates an existing struct(Store). Clears all elements by using the
 *  map-destroy functions.
 * @param n - Target struct to be deallocated. If struct is NULL nothing will be
 * 		done
 */
void destroyStore(Store n) {
	mapDestroy(n->Potion);
	mapDestroy(n->Candy);
	free(n);
}
/*
 * adding an item to the Store.
 * @param
 * Store store, char* item, int value, int quantity
 */
void storeAdd(Store store, char* type, int value, int quantity) {
	if (!store || !type)
		return;
	int* value1 = malloc(sizeof(int));
	if (!value1)
		return;
	*value1 = value;
	int* quantityPtr = malloc(sizeof(int));
	if (!quantityPtr) {
		free(value1);
		return;
	}
	*quantityPtr = quantity;
	if (strcmp(type, "potion") == 0) {
		if (mapContains(store->Potion, value1) == true) {
			*quantityPtr += *(int*) (mapGet(store->Potion, value1));
		}
		mapPut(store->Potion, value1, quantityPtr);
	} else if (strcmp(type, "candy") == 0) {
		if (mapContains(store->Candy, value1) == true) {
			*quantityPtr += *(int*) mapGet(store->Candy, value1);
		}
		mapPut(store->Candy, value1, quantityPtr);
	}
	free(quantityPtr);
	free(value1);
}
/*
 * returns the quantity of the items with the certain value.
 *  @param
 *  store - target struct
 *  type - type of item we need to find
 *  key - value of item we need to find
 *  @return
 *  -1 - if arguments are null or allocation if failed, or the are no items with such value
 *  a quantity of the items
 */
int storeGetQuantityByKey(Store store, char* type, int key) {
	if (!store || !type)
		return -1;
	int* key1 = malloc(sizeof(int));
	if (!key1)
		return -1;
	*key1 = key;
	int a;
	if (strcmp(type, "potion") == 0) {
		if (!mapGet(store->Potion, key1)) {
			free(key1);
			return -1;
		}
		a = *(int*) mapGet(store->Potion, key1);
	}
	if (strcmp(type, "candy") == 0) {
		if (!mapGet(store->Candy, key1)) {
			free(key1);
			return -1;
		}
		a = *(int*) mapGet(store->Candy, key1);
	}
	free(key1);
	return a;
}

/*
 * decreases the existing number of items by 1
 * @param
 * store - target struct
 * item - item that we need to find
 * key - value of item
 */
void changeQuantityFromStoreBuy(Store store, char* item, int key) {
	if (!store || !item)
		return;
	int* key1 = malloc(sizeof(int));
	if (!key1)
		return;
	*key1 = key;
	int quantity;
	if (strcmp(item, "potion") == 0) {
		if (!(mapGet(store->Potion, key1))) {
			free(key1);
			return;
		}
		quantity = *(int*) mapGet(store->Potion, key1);
		if (quantity != 0)
			quantity--;
		if (quantity == 0) {
			mapRemove(store->Potion, key1);
			free(key1);
			return;
		}
		mapPut(store->Potion, key1, &quantity);
	} else {
		if (!(mapGet(store->Candy, key1))) {
			free(key1);
			return;
		}
		quantity = *(int*) mapGet(store->Candy, key1);
		if (quantity != 0)
			quantity--;
		if (quantity == 0) {
			mapRemove(store->Candy, key1);
			free(key1);
			return;
		}
		mapPut(store->Candy, key1, &quantity);
	}
	free(key1);
}
/*
 * increases the existing number of items by 1
 * @param
 * store - target struct
 * item - item that we need to find
 * key - value of item
 */
void changeQuantityFromStorePurchase(Store store, char* item, int key) {
	if (!store || !item)
		return;
	int* key1 = malloc(sizeof(int));
	if (!key1)
		return;
	*key1 = key;
	int quantity = 0;
	if (strcmp(item, "potion") == 0) {
		if ((mapGet(store->Potion, key1)))
			quantity = *(int*) mapGet(store->Potion, key1);
		quantity++;
		mapPut(store->Potion, key1, &quantity);
	} else {
		if ((mapGet(store->Candy, key1)))
			quantity = *((int*) mapGet(store->Candy, key1));
		quantity++;
		mapPut(store->Candy, key1, &quantity);
	}
	free(key1);
}

/*
 * return the number of potions at the store
 * @param
 * store - target struct
 * @return
 * -1 - argument is null
 * number of potions
 */
int storeGetSizePotion(Store store) {
	if (!store)
		return -1;
	return mapGetSize(store->Potion);
}

/*
 * return the number of candies at the store
 * @param
 * store - target struct
 * @return
 * -1 - argument is null
 * number of candies
 */
int storeGetSizeCandy(Store store) {
	if (!store)
		return -1;
	return mapGetSize(store->Candy);
}

/*	return the max value of certain item from all the items of this type at the store
 * @param
 * store - target struct
 * type - type of items we need to search
 * @return
 * 0 - arguments are null
 * max value of the item
 */
int storeGetMaxOutOfItem(Store store, char* type) {
	if (!store || !type)
		return 0;
	int numMax = 0;
	if (strcmp(type, "potion") == 0) {
		int* potion_ptr = mapGetFirst(store->Potion);
		if (!potion_ptr)
			return 0;
		int i = mapGetSize(store->Potion);
		while (i > 1) {
			potion_ptr = mapGetNext(store->Potion);
			i--;
		}
		numMax = *potion_ptr;
	} else {
		int* potion_ptr = mapGetFirst(store->Candy);
		if (!potion_ptr)
			return 0;
		int i = mapGetSize(store->Candy);
		while (i > 1) {
			potion_ptr = mapGetNext(store->Candy);
			i--;
		}
		numMax = *potion_ptr;
	}
	return numMax;
}
/*	finds the min value of the item that can heal a pokemon to 100 hp and removes it from the store
 * @param
 * store - target struct
 * type - type of items we need to search
 * @return
 * false - argumens are null or the are no such items that can heal a pokemon to 100 hp
 * true - we found an item with min value that can heal a pokemon to 100 hp
 */
bool storeFindMinToMaxHp(Store store, int delta) {
	if (!store)
		return false;
	int* quantity;
	int* potion_ptr = mapGetFirst(store->Potion);
	if (!potion_ptr)
		return false;
	int i = mapGetSize(store->Potion);
	while (i > 1) {
		if (*potion_ptr >= delta) {
			quantity = mapGet(store->Potion, potion_ptr);
			(*quantity)--;
			changeQuantityFromStoreBuy(store, "potion", *potion_ptr);
			return true;
		}
		i--;
		potion_ptr = mapGetNext(store->Potion);
	}
	return false;
}

/*
 * prints all the items that store has
 */
void storePrint(Store store, FILE* output_f) {
	if (!store || !output_f)
		return;

	int* key = mapGetFirst(store->Candy);
	int i = mapGetSize(store->Candy);
	int* data;

	if (key) {
		while (i >= 1) {
			data = mapGet(store->Candy, key);
			if (key && data) {
				mtmPrintItem(output_f, "candy", *key, *data);
				key = mapGetNext(store->Candy);
			}
			i--;
		}
	}
	key = mapGetFirst(store->Potion);
	i = mapGetSize(store->Potion);

	if (!key)
		return;
	while (i >= 1) {
		data = mapGet(store->Potion, key);
		if (!key || !data)
			return;
		mtmPrintItem(output_f, "potion", *key, *data);
		key = mapGetNext(store->Potion);
		i--;
	}

}
