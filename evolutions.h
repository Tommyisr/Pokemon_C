/*
 * evolutions.h
 *
 *  Created on: Dec 23, 2016
 *      Author: Tommyisr
 */

#ifndef EVOLUTIONS_H_
#define EVOLUTIONS_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
 * struct for evolutions
 */
typedef struct evolutions_t *Evolutions;

/*@param
 before - name before evolution
 after - name afer evoltution
 lvl - level required for evolution
 creating a struct for evolutions
 returns NULL if arguments are NULL or allocation is failed
 */
Evolutions evoCreate(char* before, char* after, int lvl);

/*@param
 * n - struct of evolutions
 * copying an Evolutions by allocating memory for new one
 * returns NULL if arguments are NULL or allocation is failed
 */
Evolutions copyEvo(Evolutions n);

/* @param
 * n - struct of evolutions
 * destroying an Evolution
 */
void freeEvo(Evolutions n);

/* @param
 * evolution - struct of evolutions
 * returns the level required for evolution
 * returns -1 if arguments are NULL
 */
int evolutionsGetLevel(Evolutions evolution);

/* @param
 * evolution - struct of evolutions
 * returns the name of pokemon after evolution
 * returns NULL if arguments are NULL
 */
char* evolutionGetNewName(Evolutions evolution);

#endif /* EVOLUTIONS_H_ */
