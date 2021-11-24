#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	FILE *fptr;
	FILE *fptr2;

	char * line = NULL;
	size_t len = 0;
	char * line2 = NULL;
	size_t len2 = 0;

	if ((fptr = fopen(argv[1], "r")) == NULL) {
		printf("ERROR: File can not be opened\n");
		return 1;
	}

	if ((fptr2 = fopen(argv[2], "r")) == NULL) {
		printf("ERROR: File 2 can not be opened\n");
		return 1;
	}

	while (getline(&line, &len, fptr) != -1 && getline(&line2, &len2, fptr2) != -1) {
		// if (getline(&line2, &len2, fptr2) != -1) {
		// 	printf("not as many lines in f2\n");
		// }
		if (strcmp(line, line2) != 0) {
			printf("diff line:\n");
			printf("%s%s\n", line, line2);
		}
	}
}
