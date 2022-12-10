#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_LENGHT    (16)

int32_t getLine(char* buff, uint32_t size)
{
    int32_t len = -1;

    /* fgets() returns NULL on error or when end of file occurs while no
     * characters have been read. */
    if (fgets(buff, size, stdin) != NULL)
    {
        len = strnlen(buff, size);
        if (buff[len-1] == '\n')
        {
            buff[len-1] = '\0';
            len--;
        }
    }

    return len;
}

void printPixel(uint32_t cycle, uint32_t xreg)
{
    if ( (cycle = (cycle - 1) % 40) == 0 )
        printf("\n");

    if ( (cycle >= xreg - 1) && (cycle <= xreg + 1) )
        printf("#");
    else
        printf(".");
}

int32_t main()
{
    char line[MAX_INPUT_LINE_LENGHT];
    int32_t size;
    uint32_t part1 = 0;

    int32_t X = 1;
    uint32_t cycle = 1;
    char inst;
    int32_t arg;
    uint32_t checkpoint = 20;

    /* Read lines from stdin until EOF */
    while((size = getLine(line, MAX_INPUT_LINE_LENGHT)) > 0)
    {
        printPixel(cycle, X);
        if (sscanf(line, "%c%*s %d", &inst, &arg) == 2)
        {
            /* Add an additional cycle and print */
            cycle += 1;
            printPixel(cycle, X);
        }

        cycle += 1;

        if ( (cycle > checkpoint) && (checkpoint <= 220) )
        {
            part1 += (checkpoint * X);
            checkpoint += 40;
        }

        if (inst == 'a')
            X += arg;
    }

    /* Print the results for both parts of the challenge. */
    printf("\nCycle: %u | Reg X: %d\n", cycle, X);
    printf("Part 1: %u\n", part1);

    return 0;
}

