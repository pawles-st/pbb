// zad. 1.
//
// generates 0-1 sequence with rand()

#include <stdio.h>
#include <stdlib.h>

int main() {
	srand(7);
	FILE *f = fopen("rand.txt", "w");
	for (int i = 0; i < 1100000; ++i) {
		fprintf(f, "%d", rand() % 2);
	}
	fclose(f);
}
