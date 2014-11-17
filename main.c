#include <stdio.h>
#include "core.h"

int main(int argc, char *argv[]) {
	Env *env = env_init();
	for (;;) {
		Object *o = eval(env, read());
		if (o == NULL) {
			printf("\n");
			return 0;
		}
		print(o);
		printf("\n");
	}
	return 1;
}
