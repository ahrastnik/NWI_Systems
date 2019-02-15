#include "common.h"

int countChars(char *string, char chr) {
	unsigned int i, cnt = 0;
	for (i = 0; i < strlen(string); i++) {
		if (string[i] == chr) {
			cnt++;
		}
	}
	return cnt;
}

int containsChar(char *string, char chr) {
	unsigned int i;
	for (i = 0; i < strlen(string); i++) {
		if (string[i] == chr) {
			return 1;
		}
	}
	return 0;
}

int containsAnyChar(char *string, char *chrs) {
	unsigned int i, j, cnt = 0;
	for (i = 0; i < strlen(string); i++) {
		for (j = 0; j < strlen(chrs); j++) {
			if (string[i] == chrs[j]) {
				cnt++;
			}
		}
	}
	return cnt;
}

int getCharIndex(char *string, char chr) {
	unsigned int i;
	for (i = 0; i < strlen(string); i++) {
		if (string[i] == chr) {
			return i;
		}
	}
	return -1;
}

int stripChar(char * string, char chr) {
	unsigned int i, j = 0;
	char *tmp = NULL;
	tmp = malloc(strlen(string) + 1);
	if (tmp == NULL) return -1;

	for (i = 0; i < strlen(string); i++) {
		if (string[i] != chr) {
			tmp[j] = string[i];
			j++;
		}
	}
	tmp[j] = '\0';
	strcpy(string, tmp);
	free(tmp);
	return 0;
}

int getStringIndex(char *string, char **str_array) {
	unsigned int i = 0;
	while (str_array[i] != NULL) {
		if (strcmp(str_array[i], string) == 0) return i;
		i++;
	}
	return -1;
}

int isStringInArray(char *string, char **str_array) {
	unsigned int i = 0;
	while (str_array[i] != NULL) {
		if (strcmp(str_array[i], string) == 0) return 1;
		i++;
	}
	return 0;
}

int strToNum(char *string, int *num) {
	unsigned int i;
	int tmp;
	const char *nptr = string;
	char *endptr = NULL;
	// Reset security check
	errno = 0;

	if (strlen(string) > 2) {
		if (*string == '0' && *(string + 1) == 'b') {
			// Binary
			char *bin = NULL;
			bin = malloc(strlen(string) + 1);
			strcpy(bin, string);
			nptr = bin;
			// Remove 0b
			for (i = 0; i < strlen(bin) - 2; i++) {
				bin[i] = bin[i + 2];
			}
			bin[i] = '\0';
			// Convert to integer
			tmp = strtol(nptr, &endptr, 2);
			// Check if an error occured during conversion
			if (endptr == nptr || errno == ERANGE || errno == EINVAL) {
				errno = 0;
				if (bin != NULL) free(bin);
				return 0;
			}
			
			*num = tmp;
			if (bin != NULL) free(bin);
			return 1;
		}
	}
	
	tmp = (int)strtol(nptr, &endptr, 0);
	// Check if the number was correctly converted
	if (endptr == nptr || errno == ERANGE || errno == EINVAL) {
		return 0;
	}
	*num = tmp;

	return 1;
}

char *toLower(char *string) {
	unsigned int i;
	for (i = 0; i < strlen(string); i++) string[i] = tolower(string[i]);
	return string;
}

char *toUpper(char *string) {
	unsigned int i;
	for (i = 0; i < strlen(string); i++) string[i] = toupper(string[i]);
	return string;
}
