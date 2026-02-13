#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "ERROR: n, m expected\n");
        return 1;
    }

    int n, m;

    n = atoi(argv[1]);
    m = atoi(argv[2]);

    if (argc == 4) {
        srand(atoi(argv[3]));
    } else {
        srand(time(0));
    }

    int rows = n, cols = m;

    for (int row = 0; row < rows; ++row) {
        if (rand() % 10 > 2) cols = ((rand() % (m - 2)) + 2);
        if (rand() % 20 > 15) printf("\n");
        for (int col = 0; col < cols; ++col) {
            if (col > 0) printf(" ");
            printf("%4d", (rand() % 1000) - 500);
        }
        printf("\n");
    }

    return 0;
}
