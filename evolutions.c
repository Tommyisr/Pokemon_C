#include "evolutions.h"

struct evolutions_t {
	char* EvoBefo;
	char* EvoAfter;
	int reqLev;

};

Evolutions evoCreate(char* before, char* after, int lvl) {

	Evolutions evo = malloc(sizeof(*evo));
	evo->EvoBefo = malloc(sizeof(char) * strlen(before) + 1);
	evo->EvoAfter = malloc(sizeof(char) * strlen(after) + 1);
	if (!evo || !(evo->EvoBefo) || !(evo->EvoAfter)) {
		return NULL;
	}

	strcpy(evo->EvoBefo, before);
	strcpy(evo->EvoAfter, after);
	evo->reqLev = lvl;
	return evo;
}

Evolutions copyEvo(Evolutions n) {
	if (!n) {
		return NULL;
	}
	int a = strlen(n->EvoBefo);
	int b = strlen(n->EvoAfter);
	Evolutions copy = malloc(sizeof(*copy));
	copy->EvoBefo = malloc(sizeof(char) * a + 1);
	copy->EvoAfter = malloc(sizeof(char) * b + 1);
	if (!(copy->EvoBefo) || !(copy->EvoAfter) || !copy) {
		free(copy->EvoBefo);
		free(copy->EvoAfter);
		free(copy);
		return NULL;
	}
	strcpy(copy->EvoBefo, n->EvoBefo);
	strcpy(copy->EvoAfter, n->EvoAfter);
	copy->reqLev = n->reqLev;
	return copy;
}

void freeEvo(Evolutions n) {
	if (!n)
		return;
	free(n->EvoBefo);
	free(n->EvoAfter);
	free(n);
}

int evolutionsGetLevel(Evolutions evolution) {
	if (!evolution)
		return -1;
	return evolution->reqLev;
}

char* evolutionGetNewName(Evolutions evolution) {
	if (!evolution)
		return NULL;
	return evolution->EvoAfter;
}
