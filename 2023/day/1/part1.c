#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    FILE *stream = stdin;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    uint32_t x = 0, y = 0, sum = 0;
    while ((nread = getline(&line, &len, stream)) != -1) {
        ssize_t i = 0;

        x = y = 0;

        for (i = 0; i < nread; i++) {
            if (isdigit(line[i])) {
                x = y = line[i] - '0';
                break;
            }
        }

        for (i = 0; i < nread; i++) {
            if (isdigit(line[i])) {
                y = line[i] - '0';
            }
        }

        sum += 10 * x + y;
    }

    printf("%u\n", sum);

    return EXIT_SUCCESS;
}
