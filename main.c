#include <stdio.h>
#include "lsp.h"

static void print(Object *o) {
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

int main(int argc, char *argv[]) {
	HashTable *env = env_init();
	for (;;) {
		print(eval(env, read()));
		printf("\n");
	}
	return 1;
}
