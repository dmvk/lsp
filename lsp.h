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

struct object_t {
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
};

struct env_t {
	struct object_t *vars; 
	struct env_t *up;
};

typedef struct object_t Object;
typedef struct env_t Env;

Object *read();
Object *eval(Object *);
