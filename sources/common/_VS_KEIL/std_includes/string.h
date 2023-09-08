#pragma once


char   *strcat(char *, pchar );
char   *strcpy(void *dest, pchar src);
char   *strlcat(char *dest, pchar src, unsigned int size);
int     strcmp(const void *, const void *);
float   atof(char *);
void    memset(void *, int value, unsigned int size);
void    memcpy(void *dest, const void *src, unsigned int);
int     memcmp(void *, const void *, unsigned int);
char   *strchr(pchar str, int c);
char    toupper(char);
int     sscanf(void *, void *, ...);
unsigned int  strlen(pchar );
