#ifndef CORE_H 
#define CORE_H 

#include "hashtable.h"

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

/**
 * INTERFACE
 */

HashTable *env_init();
Object *read();
Object *eval(HashTable *, Object *);
void print(Object *);

/**
 * PREDEFINED ATOMS
 */

extern Object *True;
extern Object *Nil;

#endif
