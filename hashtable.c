#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

/**
 * @see http://www.cse.yorku.ca/~oz/hash.html
 */
static unsigned long hash(char *str, int size)
{
	unsigned long hash = 5381;
	int c;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash % size;
}

HashTable *ht_create(int size) {
	HashTable *ht = malloc(sizeof(HashTable));
	ht->table = malloc(sizeof(HashTableList) * size);
	for (int i = 0; i < size; i++)
		ht->table[i] = NULL;
	ht->size = size;
	return ht;
}

int ht_insert(HashTable *ht, char *name, void *data) {
	unsigned long index;
	if (ht_lookup(ht, name) != NULL)
		return 1;
	index = hash(name, ht->size); 
	HashTableList *new = (HashTableList *) malloc(sizeof(HashTableList));
	new->name = strdup(name);
	new->data = data;
	new->next = ht->table[index];
	ht->table[index] = new;
	return 0;
}

HashTableList *ht_lookup(HashTable *ht, char *name) {
	HashTableList *list;
	for (list = ht->table[hash(name, ht->size)]; list != NULL; list = list->next)
		if (strcmp(name, list->name) == 0)
			return list;
	return NULL;
}
