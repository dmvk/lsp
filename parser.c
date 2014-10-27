#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lsp.h"

static int peek();
static Sexpr *read_num();
static Sexpr *read_str();
static Sexpr *read_list();

Sexpr *read_expr() {
	for (;;) {
		int c = getchar();
		if (isspace(c))
			continue;
		if (isdigit(c)) {
			ungetc(c, stdin);
			return read_num();
		}
		if (isalpha(c)) {
			ungetc(c, stdin);
			return read_str();
		}
		if (c == '(') {
			return read_list();
		}
		if (c == EOF)
			return NULL;
		printf("Syntax error\n");
		exit(1);
	}
}

static int peek() {
	int c = getchar();
	ungetc(c, stdin);
	return c;
}

static Sexpr *read_num() {
	int num = 0; 
	while (isdigit(peek())) 
		num = num * 10 + (getchar() - '0');
	Sexpr *s = (Sexpr *) malloc(sizeof(Sexpr));
	s->type = NUM;
	s->num = num;
	return s;
}

static Sexpr *read_str() {
	char buf[100];
	int i = 0;
	while (isalpha(peek()))
		buf[i++] = getchar(); 
	buf[i] = '\0';
	Sexpr *s = (Sexpr *) malloc(sizeof(Sexpr));
	s->type = STR;
	s->str = malloc(strlen(buf) + 1);
	strcpy(s->str, buf);
	return s;
}

static Sexpr *cons(Sexpr *car, Sexpr *cdr) {
	Sexpr *s = (Sexpr *) malloc(sizeof(Sexpr));
	s->type = CONS;
	s->car = car;
	s->cdr = cdr;
	return s;
}

/**
 * '(' already read
 */
static Sexpr *read_list() {
	Sexpr *first = NULL;
	Sexpr *head = NULL;
	for (;;) {
		int c = getchar();
		if (isspace(c))
			continue;
		if (c == ')')
			return first;
		ungetc(c, stdin);
		if (head == NULL)
			first = head = cons(read_expr(), NULL);
		else
			head = head->cdr = cons(read_expr(), NULL);
	}
}
