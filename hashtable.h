#ifndef HASHTABLE_H
#define HASHTABLE_H 

typedef struct hash_table_list_t {
	char *name;
	void *data;
	struct hash_table_list_t *next;
} HashTableList;

typedef struct hash_table_t {
	int size;
	HashTableList **table;
} HashTable;

HashTable *ht_create(int);
int ht_insert(HashTable *, char *, void *);
HashTableList *ht_lookup(HashTable *, char *);

#endif
