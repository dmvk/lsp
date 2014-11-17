#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "core.h"

#define SYMBOL_MAX_LENGTH 128

const char symbol_special_chars[] = "+-"; // todo

/**
 * PREDEFINED ATOMS
 */

Object *True = &(Object) { OT_TRUE };
Object *Nil = &(Object) { OT_NIL };

/**
 * HELPERS
 */

static Object *new_object(ObjectType t) {
	Object *o = (Object *) malloc(sizeof(Object));
	o->type = t;
	return o;
}

static Object *cons(Object *car, Object *cdr) {
	Object *o = new_object(OT_CONS);
	o->car = car;
	o->cdr = cdr;
	return o;
}

static int count(Object *list) {
	int cnt = 0;
	for (; list->type == OT_CONS; list = list->cdr) {
		cnt++;
	}
	return list == Nil ? cnt : -1;
}

static void err(char *msg, ...) {
	printf("%s\n", msg);
	exit(1);
}

/**
 * RECURSIVE DESCENT PARSER
 */

static int peek();
static Object *read_number();
static Object *read_symbol();
static Object *read_list();
static Object *read_quote();

Object *read() {
	for (;;) {
		int c = getchar();
		if (isspace(c)) {
			continue;
		}
		if (isdigit(c)) {
			ungetc(c, stdin);
			return read_number();
		}
		if (c == '-' && isdigit(peek())) {
			return read_number(0);
		}
		if (isalpha(c) || strchr(symbol_special_chars, c)) {
			ungetc(c, stdin);
			return read_symbol();
		}
		if (c == '(') {
			return read_list();
		}
		if (c == '\'') {
			return read_quote();
		}
		if (c == EOF) {
			return NULL;
		}
		err("Syntax error\n");
	}
}

// PRIVATE

static int peek() {
	int c = getchar();
	ungetc(c, stdin);
	return c;
}

static Object *read_number(int isPositive) {
	int v = 0; 
	while (isdigit(peek())) 
		v = v * 10 + (getchar() - '0');
	Object *o = new_object(OT_INT);
	o->value = isPositive ? v : -v;
	return o;
}

static Object *read_symbol() {
	char buf[SYMBOL_MAX_LENGTH + 1];
	int i = 0;
	while (isalnum(peek()) || strchr(symbol_special_chars, peek())) {
		if (i >= SYMBOL_MAX_LENGTH) {
			err("Symbol name is too long");
		}
		buf[i++] = getchar(); 
	}
	buf[i] = '\0';
	Object *o = new_object(OT_SYMBOL);
	o->name = malloc(strlen(buf) + 1);
	strcpy(o->name, buf);
	return o;
}

static Object *read_list() {
	Object *first = Nil;
	Object *last = Nil;
	for (;;) {
		int c = getchar();
		if (isspace(c)) {
			continue;
		}
		if (c == ')') {
			return first;
		}
		ungetc(c, stdin);
		if (first == Nil) {
			first = last = cons(read(), Nil);
		} else {
			last = last->cdr = cons(read(), Nil);
		}
	}
}

static Object *read_quote() {
	Object *o = new_object(OT_SYMBOL);
	o->name = "quote";
	return cons(o, cons(read(), Nil));
}

/**
 * EVAL
 */

static Object *apply(Env *, Object *, Object*);
static Object *env_lookup(Env *env, char *name);

Object *eval(Env *env, Object *o) {
	switch (o->type) {
		case OT_INT:
		case OT_TRUE:
		case OT_NIL:
			return o;
		case OT_CONS: {
			Object *fn = eval(env, o->car);
			Object *args = o->cdr;
			if (fn->type != OT_PRIMITIVE && fn->type != OT_FUNCTION) {
				err("The first element of list must be a function");
			}
			if (args->type != OT_NIL && args->type != OT_CONS) {
				err("Function argument must be a list");
			}
			return apply(env, fn, args);
		}
		case OT_SYMBOL: {
			return env_lookup(env, o->name);
		}
		default:
			return NULL;
	}
}

static Object *eval_args(Env *env, Object *o) {
	Object *first = Nil;
	Object *last = Nil;
	for (; o != Nil; o = o->cdr) {
		if (first == Nil) {
			first = last = cons(eval(env, o->car), Nil);
		} else {
			last = last->cdr = cons(eval(env, o->car), Nil);
		}
	}
	return first;
}

static Object *apply(Env *env, Object* o, Object *args) {
	if (o->type == OT_PRIMITIVE) {
		return o->function(env, args);
	}
	return NULL;
}

/**
 * Print
 */

void print(Object *o) {
	if (o == NULL)
		printf("NULL");
	switch(o->type) {
		case OT_INT:
			printf("%d", o->value);
			break;
		case OT_SYMBOL:
			printf("%s", o->name);
			break;
		case OT_CONS:
			printf("(");
			print(o->car);
			print(o->cdr);
			printf(")");
			break;
		case OT_NIL:
			printf("Nil");
			break;
		case OT_PRIMITIVE:
			printf("<primitive>");
			break;
		default:
			printf("unhandled");
	}
	printf(" ");
}

/**
 * Primitives 
 */

static Object *primitive_car(Env *env, Object *args) {
	args = eval_args(env, args);
	if (args->car->type != OT_CONS || args->cdr != Nil) {
		err("car accepts single list argument only");
	}
	return args->car->car;
}

static Object *primitive_cdr(Env *env, Object *args) {
	args = eval_args(env, args);
	if (args->car->type != OT_CONS|| args->cdr != Nil) {
		err("cdr accepts single list argument only");
	}
	return args->car->cdr;
}

static Object *primitive_cons(Env *env, Object *args) {
	if (count(args) != 2) {
		err("cons accepts two arguments only");
	}
	args = eval_args(env, args);
	args->cdr = args->cdr->car;
	return args;
}

static Object *primitive_minus(Env *env, Object *args) {
	int diff;
	int first = 1;
	for (args = eval_args(env, args); args != Nil; args = args->cdr) {
		if (args->car->type != OT_INT) {
			err("+ accepts only integers");
		}
		if (first) {
			diff = args->car->value;
			first = 0;
			if (args->cdr == Nil) {
				diff = -diff;
				break;
			} 
		} else {
			diff -= args->car->value;
		}
	}
	Object *o = new_object(OT_INT);
	o->value = diff;
	return o;
}

static Object *primitive_plus(Env *env, Object *args) {
	int sum = 0;
	for (args = eval_args(env, args); args != Nil; args = args->cdr) {
		if (args->car->type != OT_INT) {
			err("+ accepts only integers");
		}
		sum += args->car->value;
	}
	Object *o = new_object(OT_INT);
	o->value = sum;
	return o;
}

static Object *primitive_quote(Env *env, Object *args) {
	if (count(args) != 1) {
		err("quote accepts one argument only");
	}
	return args->car;
}

/**
 * ENV
 */

static void add_primitive(Env *env, char *name, Primitive *fn) {
	Object *o = new_object(OT_PRIMITIVE);
	o->function = fn;
	ht_insert(env->ht, name, o);
}

static Env *env_create() {
	Env *env = (Env *) malloc(sizeof(Env));
	env->ht = ht_create(128);
	return env;
}

Env *env_init() {
	Env *env = env_create();
	add_primitive(env, "car", primitive_car);
	add_primitive(env, "cdr", primitive_cdr);
	add_primitive(env, "cons", primitive_cons);
	add_primitive(env, "-", primitive_minus);
	add_primitive(env, "+", primitive_plus);
	add_primitive(env, "quote", primitive_quote);
	return env;
}

static Object *env_lookup(Env *env, char *name) {
	HashTableList *list = ht_lookup(env->ht, name);
	if (list == NULL && env->parent != NULL) {
		return env_lookup(env->parent, name);
	}
	if (list == NULL) {
		printf("Undefined symbol: %s\n", name);
		exit(1);
	}
	return list->data;
}
