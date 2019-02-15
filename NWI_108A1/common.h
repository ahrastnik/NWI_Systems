#ifndef COMMON
#define COMMON

#include <stdio.h>
#include <string.h>

// String Functions
int countChars(char *string, char chr);
int containsChar(char *string, char chr);
int containsAnyChar(char *string, char *chrs);
int getCharIndex(char *string, char chr);
int stripChar(char *string, char chr);

int getStringIndex(char *string, char **str_array);
int isStringInArray(char *string, char **str_array);
int strToNum(char *string, int *num);
char *toLower(char *string);
char *toUpper(char *string);

#endif // !COMMON
