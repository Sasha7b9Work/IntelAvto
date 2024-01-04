#pragma once

void *malloc(size_t);
void free(void *);
#define RAND_MAX 0x7fffffff
void srand(unsigned int);
unsigned int rand();
