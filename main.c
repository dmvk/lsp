#include <stdio.h>
#include "core.h"

int main(int argc, char *argv[]) {
	Env *env = env_init();
	for (;;) {
		print(eval(env, read()));
		printf("\n");
	}
	return 1;
}
