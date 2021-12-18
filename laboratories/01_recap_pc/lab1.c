//implementarea se bazeaza pe Smenul lui Mars conform:
//https://www.infoarena.ro/multe-smenuri-de-programare-in-cc-si-nu-numai

#include <stdio.h>
#include <stdlib.h>
#define NMAX 100

int main() {
	int nl, nc, a, b,i, j;
	scanf( "%d%d", &nl, &nc );
	int **mat;
	mat = calloc( nl + 1, sizeof(int*));
	if (mat == NULL) {
		fprintf(stderr, "Eroare la alocare...\n");
		return 1;
	}
	for (i = 0; i < nl + 1; i++) {
		mat[i] = calloc( nc + 1, sizeof(int));
        if (mat[i] == NULL) {
		    fprintf(stderr, "Eroare la alocare...\n");
		    return 2;
        }
	}
	while(scanf("%d%d", &a, &b) == 2) {
		a--;
		b--;
		mat[0][0] += 1;
		mat[a + 1][0] += -1;
		mat[0][b + 1] += -1;
		mat[a + 1][b + 1] += 1;
	}
	for (i = 1; i < nl + 1; i++)
		mat[i][0] += mat[i - 1][0];
	for (j = 1; j < nc + 1; j++)
		mat[0][j] += mat[0][j - 1];
	for (i = 1; i < nl; i++)
		for (j = 1; j < nc; j++)
			mat[i][j] += mat[i - 1][j] + mat[i][j - 1] - mat[i - 1][j - 1];
	for (i = 0; i < nl; i++) {
        for (j = 0; j < nc; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
	for (i = 0; i < nl + 1; i++) {
		free(mat[i]);
	}
	free(mat);
	return 0;
}
