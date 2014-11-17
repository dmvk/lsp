#ifndef CORE_H 
#define CORE_H 

#include "hashtable.h"

/**
 * ENV
 */

typedef struct env_t {
	HashTable *ht;
	struct env_t *parent;
} Env;

/**
 * LIST STRUCTURED MEMORY
 */

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

typedef struct object_t *(Primitive)(Env *, struct object_t *);

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

/**
 * INTERFACE
 */

Env *env_init();
Object *read();
Object *eval(Env *, Object *);
void print(Object *);

#endif
