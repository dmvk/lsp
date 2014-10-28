#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lsp.h"

#define SYMBOL_MAX_LENGTH 128

/**
 * PREDEFINED ATOMS
 */

static Object *True = &(Object) { OT_TRUE };
static Object *Nil = &(Object) { OT_NIL };

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

static void err(char *msg) {
	printf("ERROR: %s\n", msg);
	exit(1);
}

/**
 * RECURSIVE DESCENT PARSER
 */

static int peek();
static Object *read_number();
static Object *read_symbol();
static Object *read_list();

Object *read() {
	for (;;) {
		int c = getchar();
		if (isspace(c))
			continue;
		if (isdigit(c)) {
			ungetc(c, stdin);
			return read_number();
		}
		if (isalpha(c)) {
			ungetc(c, stdin);
			return read_symbol();
		}
		if (c == '(') {
			return read_list();
		}
		if (c == EOF)
			return NULL;
		err("Syntax error\n");
	}
}

// PRIVATE

static int peek() {
	int c = getchar();
	ungetc(c, stdin);
	return c;
}

static Object *read_number() {
	int v = 0; 
	while (isdigit(peek())) 
		v = v * 10 + (getchar() - '0');
	Object *o = new_object(OT_INT);
	o->value = v;
	return o;
}

static Object *read_symbol() {
	char buf[SYMBOL_MAX_LENGTH + 1];
	int i = 0;
	while (isalpha(peek())) {
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
	Object *last;
	for (;;) {
		int c = getchar();
		if (isspace(c))
			continue;
		if (c == ')')
			return first;
		ungetc(c, stdin);
		if (last == NULL)
			first = last = cons(read(), Nil);
		else
			last = last->cdr = cons(read(), Nil);
	}
}

/**
 * EVAL
 */

static Object* apply(Object *, Object*);

Object *eval(Object *o) {
	switch (o->type) {
		case OT_INT:
		case OT_TRUE:
		case OT_NIL:
			return o;
		case OT_CONS: {
			Object *fn = o->car;	
			Object *args = o->cdr;
			if (fn->type != OT_PRIMITIVE && fn->type != OT_FUNCTION) {
				err("The first element of list must be a function");
			}
			if (fn->type != OT_NIL && fn->type != OT_CONS) {
				err("Function argument must be a list");
			}
			return apply(fn, args);
		}
		default:
			return NULL;
	}
}

Object* apply(Object* fn, Object *args) {
	if (fn->type == OT_PRIMITIVE) {
		return fn->function(args);
	}
	return NULL;
}
