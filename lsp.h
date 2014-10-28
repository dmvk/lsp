#ifndef LSP_H
#define LSP_H

typedef enum {
	OT_CONS,
	OT_INT,
	OT_SYMBOL,
	OT_TRUE,
	OT_NIL,
	OT_PRIMITIVE,
	OT_FUNCTION
} ObjectType;

struct object_t;

typedef struct object_t *(Primitive)(struct object_t *args);

typedef struct object_t {
	ObjectType type;
	union {
		struct {
			struct object_t *car;
			struct object_t *cdr;
		};
		int value;
		char *name;
		Primitive *function;
	};
} Object;

typedef struct hash_table_list_t {
	char *name;
	Object *object;
	struct hash_table_list_t *next;
} HashTableList;

typedef struct hash_table_t {
	int size;
	HashTableList **table;
} HashTable;

HashTable *env_init();
Object *read();
Object *eval(HashTable *, Object *);

HashTable *ht_create(int);
int ht_insert(HashTable *, char *, Object *);
Object *ht_lookup(HashTable *, char *);

#endif
