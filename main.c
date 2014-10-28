#include <stdio.h>
#include "core.h"

extern Object *True;
extern Object *Nil;

int main(int argc, char *argv[]) {
	HashTable *env = env_init();
	for (;;) {
		print(eval(env, read()));
		printf("\n");
	}
	return 1;
}
