/*
 * store.h
 *
 *  Created on: Dec 23, 2016
 *      Author: Tommyisr
 */

#ifndef STORE_H_
#define STORE_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "set.h"
#include "map.h"
#include "print_utils.h"

/*
 * the struct declarationd for Store
 */
typedef struct store_t *Store;

/** Type of function for copying a data element of the map */
MapDataElement copyMapData(MapDataElement n);

/** Type of function for copying a key element of the map */
MapKeyElement copyMapKey(MapKeyElement n);

/** Type of function for deallocating a data element of the map */
void freeMapData(MapDataElement n);

/** Type of function for deallocating a key element of the map */
void freeMapKey(MapKeyElement n);

/** Type of function for deallocating a key element of the map */
int compareMapKeyEl(MapKeyElement a, MapKeyElement b);

/*	allocates a new Store struct
 * @return
 * NULL - if allocations if failed
 */
Store storeCreate();


/*	Deallocates an existing struct(Store). Clears all elements by using the
*  map-destroy functions.
* @param n - Target struct to be deallocated. If struct is NULL nothing will be
* 		done
 */
void destroyStore(Store n);


/*
 * adding an item to the Store.
 * @param
 * Store store, char* item, int value, int quantity
 */
void storeAdd(Store store, char* item, int value, int quantity);



/* creates a copy of target struct.
 * @param
 * a - target struct(store)
 * @return
 * NULL - if the arguments are null or allocation if failed
 * A struct(store) containing the same elements as target struct otherwise.
 */
Store storeCopy(Store a);




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
int storeGetQuantityByKey(Store store, char* type, int key) ;




/*
 * decreases the existing number of items by 1
 * @param
 * store - target struct
 * item - item that we need to find
 * key - value of item
 */
void changeQuantityFromStoreBuy(Store store, char* item, int key) ;



/*
 * increases the existing number of items by 1
 * @param
 * store - target struct
 * item - item that we need to find
 * key - value of item
 */
void changeQuantityFromStorePurchase(Store store, char* item, int key) ;



/*
 * return the number of potions at the store
 * @param
 * store - target struct
 * @return
 * -1 - argument is null
 * number of potions
 */
int storeGetSizePotion(Store store) ;



/*
 * return the number of candies at the store
 * @param
 * store - target struct
 * @return
 * -1 - argument is null
 * number of candies
 */
int storeGetSizeCandy(Store store) ;



/*	return the max value of certain item from all the items of this type at the store
 * @param
 * store - target struct
 * type - type of items we need to search
 * @return
 * 0 - arguments are null
 * max value of the item
 */
int storeGetMaxOutOfItem(Store store, char* type) ;



/*	finds the min value of the item that can heal a pokemon to 100 hp and removes it from the store
 * @param
 * store - target struct
 * type - type of items we need to search
 * @return
 * false - argumens are null or the are no such items that can heal a pokemon to 100 hp
 * true - we found an item with min value that can heal a pokemon to 100 hp
 */
bool storeFindMinToMaxHp(Store store, int delta) ;




/*
 * prints all the items that store has
 */
void storePrint(Store store, FILE* output_f) ;


#endif /* STORE_H_ */
