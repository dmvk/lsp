#include <stdio.h>
#include "lsp.h"

static void print(Object *o) {
	if (o == NULL)
		printf("NULL");
	else if (o->type == OT_INT)
		printf("%d", o->value);
	else if (o->type == OT_SYMBOL)
		printf("%s", o->name);
	else if (o->type == OT_CONS) {
		printf("(");
		print(o->car);
		print(o->cdr);
		printf(")");
	} else if (o->type == OT_NIL) {
		printf("Nil");
	}
	printf(" ");
}

int main(int argc, char *argv[]) {
	for (;;) {
		print(eval(read()));
		printf("\n");
	}
	return 1;
}
