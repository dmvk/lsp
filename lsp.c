#include <stdio.h>
#include "lsp.h"

static void print(Sexpr *s) {
	if (s == NULL)
		printf("NULL");
	else if (s->type == NUM)
		printf("%d", s->num);
	else if (s->type == STR)
		printf("%s", s->str);
	else if (s->type == CONS) {
		printf("(");
		print(s->car);
		print(s->cdr);
		printf(")");
	}
	printf(" ");
}

int main(int argc, char *argv[]) {
	for (;;) {
		print(read_expr());
		printf("\n");
	}
	return 0;
}
