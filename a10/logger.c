#include <stdio.h>
#include <unistd.h>

void logger(const char* msg) {
	fprintf(stderr, "%4d: %s\n", getpid(), msg);
}
